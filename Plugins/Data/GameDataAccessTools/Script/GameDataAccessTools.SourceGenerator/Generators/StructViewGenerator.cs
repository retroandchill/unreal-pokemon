using System.Collections.Immutable;
using System.Reflection;
using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using GameAccessTools.SourceGenerator.Properties;
using GameAccessTools.SourceGenerator.Utilities;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Types;

namespace GameAccessTools.SourceGenerator.Generators;

[Generator]
public class StructViewGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var declaredStructs = context
            .SyntaxProvider.CreateSyntaxProvider(
                (n, _) => n is StructDeclarationSyntax or RecordDeclarationSyntax,
                (ctx, _) =>
                {
                    var typeSymbol = ctx.SemanticModel.GetDeclaredSymbol(ctx.Node);
                    if (typeSymbol is not INamedTypeSymbol { IsValueType: true } structType)
                    {
                        return (null, ctx.SemanticModel);
                    }

                    return (
                        typeSymbol.HasAttribute<CreateStructViewAttribute>() ? structType : null,
                        ctx.SemanticModel
                    );
                }
            )
            .Where(x => x.Item1 is not null);

        var assemblyAttributes = context.CompilationProvider.SelectMany(
            (compilation, _) =>
            {
                // Get all assembly attributes
                return compilation
                    .Assembly.GetCreateStructViewInfos()
                    .Select(x => x.Type)
                    .OfType<INamedTypeSymbol>();
            }
        );

        context.RegisterSourceOutput(
            declaredStructs,
            (
                (productionContext, tuple) =>
                {
                    var (structType, semanticModel) = tuple;
                    CreateStructViews(productionContext, structType!, semanticModel);
                }
            )
        );
        context.RegisterSourceOutput(
            assemblyAttributes,
            (
                (productionContext, structType) =>
                {
                    CreateStructViews(productionContext, structType);
                }
            )
        );
    }

    private static void CreateStructViews(
        SourceProductionContext context,
        INamedTypeSymbol structType,
        SemanticModel? semanticModel = null
    )
    {
        var isValidType = true;
        if (
            !structType
                .GetAttributes()
                .Any(a =>
                    a.AttributeClass?.ToDisplayString() == SourceContextNames.UStructAttribute
                )
        )
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA1001",
                        "Struct reference holder must be annotated with UStruct",
                        "{0} must be annotated with UStruct",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structType.Locations.First(),
                    structType.Name
                )
            );
            isValidType = false;
        }

        if (!isValidType)
        {
            return;
        }


        HashSet<string> usedNamespaces =
        [
            "using LanguageExt;",
            "using UnrealSharp;",
            "using UnrealSharp.Core;",
            "using UnrealSharp.Core.Marshallers;",
            "using UnrealSharp.CoreUObject;",
            "using UnrealSharp.Interop;",
            "using GameDataAccessTools.Core;",
            "using GameDataAccessTools.Core.Views;",
            "using GameDataAccessTools.Core.Marshallers;",
            "using GameDataAccessTools.Core.Utilities;"
        ];

        foreach (var ns in structType.DeclaringSyntaxReferences
                     .Select(x => x.GetSyntax())
                     .OfType<StructDeclarationSyntax>()
                     .Select(x => x.SyntaxTree)
                     .Select(syntaxTree => syntaxTree.GetRoot())
                     .OfType<CompilationUnitSyntax>()
                     .Select(x => x.Usings)
                     .Select(x => x.ToString()))
        {
            usedNamespaces.Add(ns);
        }

        var templateParams = new
        {
            Usings = usedNamespaces
                .Select(x => new UsingDeclaration(x))
                .ToImmutableArray(),
            Namespace = structType.ContainingNamespace.ToDisplayString(),
            StructName = structType.Name,
            EngineName = structType.Name[1..],
            Properties = structType
                .GetMembers()
                .Where(s => !s.IsStatic)
                .Where(s =>
                    s
                        is IFieldSymbol { IsImplicitlyDeclared: false }
                            or IPropertySymbol { GetMethod: not null }
                )
                .Select(x => CreatePropertyInfo(x, structType, semanticModel))
                .ToImmutableArray(),
            Methods = structType
                .GetMembers()
                .Where(s => !s.IsStatic)
                .OfType<IMethodSymbol>()
                .Where(x => x.MethodKind == MethodKind.Ordinary)
                .Select(x => TranslateMethod(x, structType, semanticModel))
                .Where(x => x is not null)
                .ToImmutableArray()
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{templateParams.EngineName}ViewExtensions.g.cs",
            handlebars.Compile(SourceTemplates.StructViewExtensionsTemplate)(templateParams)
        );
    }

    private static AccessiblePropertyInfo CreatePropertyInfo(
        ISymbol member,
        INamedTypeSymbol structType,
        SemanticModel? semanticModel
    )
    {
        var baseInfo = member.GetPropertyInfo();
        if (
            member is not IPropertySymbol propertySymbol
            || propertySymbol.GetMethod is null
            || semanticModel is null
        )
            return baseInfo;

        // Get the property syntax
        var syntaxRef = propertySymbol.DeclaringSyntaxReferences.FirstOrDefault();
        if (syntaxRef?.GetSyntax() is not PropertyDeclarationSyntax propertySyntax)
            return baseInfo;

        // Create rewriter with semantic model
        var rewriter = new MemberAccessRewriter(semanticModel, structType, propertySymbol.Name);

        var isExpressionBody = false;
        string getterBody;
        if (propertySyntax.ExpressionBody != null)
        {
            // Handle expression-bodied property
            var rewrittenExpression = rewriter.Visit(propertySyntax.ExpressionBody.Expression);
            getterBody = rewrittenExpression.ToString();
            isExpressionBody = true;
        }
        else if (propertySyntax.AccessorList != null)
        {
            var getAccessor = propertySyntax.AccessorList.Accessors.FirstOrDefault(a =>
                a.Keyword.Text == "get"
            );

            if (getAccessor == null)
                return baseInfo;

            if (getAccessor.ExpressionBody != null)
            {
                // Handle expression-bodied accessor
                var rewrittenExpression = rewriter.Visit(getAccessor.ExpressionBody.Expression);
                getterBody = rewrittenExpression.ToString();
                isExpressionBody = true;
            }
            else if (getAccessor.Body != null)
            {
                // Handle block body
                var rewrittenBlock = rewriter.Visit(getAccessor.Body);

                getterBody = IndentBody(rewrittenBlock);
            }
            else
            {
                return baseInfo;
            }
        }
        else
        {
            return baseInfo;
        }

        baseInfo = baseInfo with
        {
            GetterBody = getterBody,
            HasBackingField = structType
                .GetMembers()
                .OfType<IFieldSymbol>()
                .Any(f => SymbolEqualityComparer.Default.Equals(f.AssociatedSymbol, member)),
            IsExpressionBody = isExpressionBody
        };

        return baseInfo;
    }

    private static string IndentBody(SyntaxNode rewrittenBlock)
    {
        string getterBody;
        // This is a hack to get around the fact that extension blocks will have one additional level of
        // indentation compared to a top-level struct declaration, so indenting every line after the first
        // by 4 spaces will make the generated code look better.
        getterBody = string.Join("\n", rewrittenBlock.ToString().Split('\n')
            .Select((x, i) => i == 0 ? x : $"    {x}"));
        return getterBody;
    }

    private static TranslatedMethod? TranslateMethod(IMethodSymbol methodSymbol, 
                                                     INamedTypeSymbol structType,
                                                     SemanticModel? semanticModel)
    {
        if (semanticModel is null)
        {
            return null;
        }

        // Get the property syntax
        var methodSyntax = methodSymbol.DeclaringSyntaxReferences
            .Select(x => x.GetSyntax())
            .OfType<MethodDeclarationSyntax>()
            .FirstOrDefault(x => x.ExpressionBody is not null || x.Body is not null);
        if (methodSyntax is null) return null;

        try
        {
            // Create rewriter with semantic model
            var rewriter = new MemberAccessRewriter(semanticModel, structType);

            string methodBody;
            if (methodSyntax.ExpressionBody is not null)
            {
                var rewrittenExpression = rewriter.Visit(methodSyntax.ExpressionBody.Expression);
                methodBody = rewrittenExpression.ToString();
                return new TranslatedMethod(methodSyntax.Modifiers.ToString(),
                    methodSyntax.ReturnType.ToString(),
                    methodSyntax.Identifier.ToString(),
                    methodSyntax.TypeParameterList?.ToString(),
                    methodSyntax.ParameterList.ToString(),
                    methodSyntax.ConstraintClauses.ToString(),
                    methodBody,
                    true);
            }

            if (methodSyntax.Body is null) return null;

            // Handle block body
            var rewrittenBlock = rewriter.Visit(methodSyntax.Body);
            methodBody = IndentBody(rewrittenBlock);
            return new TranslatedMethod(methodSyntax.Modifiers.ToString(),
                methodSyntax.ReturnType.ToString(),
                methodSyntax.Identifier.ToString(),
                methodSyntax.TypeParameterList?.ToString(),
                methodSyntax.ParameterList.ToString(),
                methodSyntax.ConstraintClauses.ToString(),
                methodBody);

            // We shouldn't get here but just to be safe
        } catch (Exception)
        {
            return null;
        }
    }
}
