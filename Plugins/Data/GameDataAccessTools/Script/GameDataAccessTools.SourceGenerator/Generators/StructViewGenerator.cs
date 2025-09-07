using System.Collections.Immutable;
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
                        return null;
                    }

                    return typeSymbol.HasAttribute<CreateStructViewAttribute>() ? structType : null;
                }
            )
            .Where(x => x is not null);

        var assemblyAttributes = context.CompilationProvider.SelectMany(
            (compilation, _) =>
            {
                // Get all assembly attributes
                return compilation
                    .Assembly.GetAttributes()
                    .GetCreateStructViewInfos()
                    .Select(x => x.Type)
                    .OfType<INamedTypeSymbol>();
            }
        );

        context.RegisterSourceOutput(declaredStructs, CreateStructViews!);
        context.RegisterSourceOutput(assemblyAttributes, CreateStructViews);
    }

    private static void CreateStructViews(
        SourceProductionContext context,
        INamedTypeSymbol structType
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
                    s is IFieldSymbol { IsImplicitlyDeclared: false }
                    || s is IPropertySymbol p && p.IsAutoProperty()
                )
                .Select(x => x.GetPropertyInfo())
                .ToImmutableArray(),
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{templateParams.EngineName}ViewExtensions.g.cs",
            handlebars.Compile(SourceTemplates.StructViewExtensionsTemplate)(templateParams)
        );
    }
}
