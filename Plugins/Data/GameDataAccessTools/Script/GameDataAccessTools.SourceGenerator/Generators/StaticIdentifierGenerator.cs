using System.Collections.Immutable;
using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using GameAccessTools.SourceGenerator.Properties;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using RhoMicro.CodeAnalysis.Library.Extensions;

namespace GameAccessTools.SourceGenerator.Generators;

internal readonly record struct IdentifierPropertyInfo(
    IPropertySymbol PropertySymbol,
    INamedTypeSymbol ExtendedClass,
    INamedTypeSymbol OuterClass
);

[Generator]
public class StaticIdentifierGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var declaredStructs = context
            .SyntaxProvider.CreateSyntaxProvider(
                (n, _) => n is ClassDeclarationSyntax,
                (ctx, _) =>
                {
                    var propertyNode = (ClassDeclarationSyntax)ctx.Node;
                    var classSymbol = ctx.SemanticModel.GetDeclaredSymbol(propertyNode);
                    if (classSymbol is not INamedTypeSymbol syntax)
                    {
                        return null;
                    }

                    return classSymbol.HasAttribute(typeof(StaticIdentifierAttribute<>))
                        ? syntax
                        : null;
                }
            )
            .Where(x => x is not null);

        context.RegisterSourceOutput(declaredStructs, GenerateIdentifiers!);
    }

    private static void GenerateIdentifiers(
        SourceProductionContext context,
        INamedTypeSymbol classSymbol
    )
    {
        var isValidType = true;
        if (!classSymbol.IsStatic)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3001",
                        "Identifier extender must be static",
                        "{0} must be static",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    classSymbol.Locations.First(),
                    classSymbol.Name
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
            Namespace = classSymbol.ContainingNamespace.ToDisplayString(),
            ClassName = classSymbol.Name,
            Extensions = classSymbol
                .GetAttributes()
                .Where(x => x.AttributeClass?.Name == "StaticIdentifierAttribute")
                .Select(info =>
                    (
                        Extended: info.AttributeClass!.TypeArguments[0],
                        Properties: info.ConstructorArguments[0].Values
                    )
                )
                .Select(x => new
                {
                    ExtendedClass = x.Extended.ToDisplayString(),
                    EngineName = x.Extended.Name[1..],
                    Properties = x
                        .Properties.Select(t => t.Value)
                        .OfType<string>()
                        .Select(t => new { Identifier = t })
                        .ToImmutableArray(),
                }),
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{classSymbol.Name}.Identifiers.g.cs",
            handlebars.Compile(SourceTemplates.StaticIdentifiersTemplate)(templateParams)
        );
    }
}
