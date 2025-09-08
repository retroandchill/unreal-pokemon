using System.Collections.Immutable;
using System.Reflection;
using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using GameAccessTools.SourceGenerator.Properties;
using GameAccessTools.SourceGenerator.Utilities;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
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

        var templateParams = new
        {
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

        string getterBody;
        if (propertySyntax.ExpressionBody != null)
        {
            // Handle expression-bodied property
            var rewrittenExpression = rewriter.Visit(propertySyntax.ExpressionBody.Expression);
            getterBody = $"     => {rewrittenExpression};";
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
                getterBody = $"=> {rewrittenExpression};";
            }
            else if (getAccessor.Body != null)
            {
                // Handle block body
                var rewrittenBlock = rewriter.Visit(getAccessor.Body);
                getterBody = rewrittenBlock.ToString();
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
        };

        return baseInfo;
    }
}
