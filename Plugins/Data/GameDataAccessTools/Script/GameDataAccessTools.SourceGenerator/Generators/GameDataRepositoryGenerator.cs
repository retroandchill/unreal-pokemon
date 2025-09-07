using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using GameAccessTools.SourceGenerator.Properties;
using GameAccessTools.SourceGenerator.Utilities;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Generators;

[Generator]
internal class GameDataRepositoryGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var dataEntries = context
            .SyntaxProvider.CreateSyntaxProvider(
                (n, _) => n is ClassDeclarationSyntax,
                (ctx, _) =>
                {
                    var classNode = (ClassDeclarationSyntax)ctx.Node;
                    if (
                        ctx.SemanticModel.GetDeclaredSymbol(classNode)
                        is not INamedTypeSymbol syntax
                    )
                    {
                        return null;
                    }

                    return syntax.HasAttribute(typeof(GameDataRepositoryAttribute<>))
                        ? syntax
                        : null;
                }
            )
            .Where(m => m is not null);

        context.RegisterSourceOutput(dataEntries, GenerateGameAssetData!);
    }

    private static void GenerateGameAssetData(
        SourceProductionContext context,
        INamedTypeSymbol classSymbol
    )
    {
        var isValidType = true;

        var uclassAttributeInfo = classSymbol
            .GetAttributes()
            .SingleOrDefault(x =>
                x.AttributeClass?.ToDisplayString() == SourceContextNames.UClassAttribute
            );
        if (uclassAttributeInfo is null)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA0001",
                        "GameDataRepository must be annotated with UClass",
                        "{0} must be annotated with UClass",
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

        var baseType = classSymbol.BaseType;
        var isStatic = false;
        while (baseType is not null)
        {
            var displayName = baseType.ToDisplayString();
            if (displayName == SourceContextNames.UGameDataRepository)
            {
                context.ReportDiagnostic(
                    Diagnostic.Create(
                        new DiagnosticDescriptor(
                            "GDA0002",
                            "Cannot inherit directly from UGameDataRepository.",
                            "{0} may not inherit directly from UGameDataRepository.",
                            "GameDataAccessTools",
                            DiagnosticSeverity.Error,
                            true
                        ),
                        classSymbol.Locations.First(),
                        classSymbol.Name
                    )
                );
                isValidType = false;
                break;
            }

            if (displayName == SourceContextNames.UStaticGameDataRepository)
            {
                isStatic = true;
                break;
            }
            if (displayName is SourceContextNames.UAssetGameDataRepository)
            {
                break;
            }

            baseType = baseType.BaseType;
        }

        if (baseType is null)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA0003",
                        "GameDataEntry must inherit from UGameDataRepository",
                        "{0} must inherit from either UAssetGameDataRepository or UStaticGameDataRepository",
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

        var repositoryInfo = classSymbol.GetAttributes().GetGameDataRepositoryInfos().Single();

        var templateParams = new
        {
            Namespace = classSymbol.ContainingNamespace.ToDisplayString(),
            AssetClassName = classSymbol.Name,
            EntryName = repositoryInfo.EntryType.ToDisplayString(),
            IsStatic = isStatic,
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{classSymbol.Name[1..]}.g.cs",
            handlebars.Compile(SourceTemplates.GameDataRepositoryTemplate)(templateParams)
        );
    }
}
