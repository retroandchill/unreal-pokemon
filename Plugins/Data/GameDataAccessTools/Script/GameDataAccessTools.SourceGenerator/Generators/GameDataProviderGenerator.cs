using System.Collections.Immutable;
using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using GameAccessTools.SourceGenerator.Properties;
using GameAccessTools.SourceGenerator.Utilities;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Generators;

[Generator]
public class GameDataProviderGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var providers = context
            .SyntaxProvider.CreateSyntaxProvider(
                (n, _) => n is ClassDeclarationSyntax,
                (ctx, _) =>
                {
                    var classNode = (ClassDeclarationSyntax)ctx.Node;
                    if (
                        ModelExtensions.GetDeclaredSymbol(ctx.SemanticModel, classNode)
                        is not INamedTypeSymbol syntax
                    )
                    {
                        return null;
                    }

                    return syntax.HasAttribute<GameDataProviderAttribute>() ? syntax : null;
                }
            )
            .Where(s => s is not null);

        context.RegisterSourceOutput(providers, GenerateGameDataProvider!);
    }

    private static void GenerateGameDataProvider(
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
                        "GDA2001",
                        "GameDataProvider must be static",
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

        var dataProviderInfo = classSymbol.GetAttributes().GetGameDataProviderInfos().Single();

        var repositories = classSymbol
            .GetMembers()
            .OfType<IPropertySymbol>()
            .Where(p => p.IsStatic)
            .Where(p => p.DeclaredAccessibility == Accessibility.Public)
            .Where(p => p.Type is INamedTypeSymbol namedType && IsGameDataRepository(namedType))
            .Where(p => p.GetMethod is not null && p.SetMethod is null)
            .Where(p =>
                p.DeclaringSyntaxReferences.Select(r => r.GetSyntax())
                    .OfType<PropertyDeclarationSyntax>()
                    .Any(x => x.Modifiers.Any(m => m.IsKind(SyntaxKind.PartialKeyword)))
            )
            .Select(x => new RepositoryPropertyInfo
            {
                Type = x.Type.ToDisplayString(),
                Name = x.Name,
                SingularName = GetSingularName(x),
                RepositoryClassName = x.Type.ToDisplayString(),
                EntryType = GetEntryType(x.Type),
                HasAsset = HasAsset(x.Type),
            })
            .ToImmutableArray();

        var templateParams = new
        {
            Namespace = classSymbol.ContainingNamespace.ToDisplayString(),
            ClassName = classSymbol.Name,
            dataProviderInfo.DisplayName,
            ConfigCategory = dataProviderInfo.Category,
            HasDisplayName = !string.IsNullOrWhiteSpace(dataProviderInfo.DisplayName),
            Repositories = repositories,
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{classSymbol.Name}Settings.g.cs",
            handlebars.Compile(SourceTemplates.GameDataProviderSettingsTemplate)(templateParams)
        );
        context.AddSource(
            $"{classSymbol.Name}Manager.g.cs",
            handlebars.Compile(SourceTemplates.GameDataProviderManagerTemplate)(templateParams)
        );
        context.AddSource(
            $"{classSymbol.Name}.g.cs",
            handlebars.Compile(SourceTemplates.GameDataProviderTemplate)(templateParams)
        );
    }

    private static bool IsGameDataRepository(INamedTypeSymbol type)
    {
        if (
            !type.AllInterfaces.Any(i =>
                i.IsGenericType
                && i.ConstructedFrom.ToDisplayString() == SourceContextNames.IGameDataRepository
            ) && !type.HasAttribute(typeof(GameDataRepositoryAttribute<>))
        )
        {
            return false;
        }

        for (var baseType = type.BaseType; baseType is not null; baseType = baseType.BaseType)
        {
            if (baseType.ToDisplayString() == SourceContextNames.UGameDataRepository)
            {
                return true;
            }
        }

        return false;
    }

    private static ITypeSymbol GetEntryType(ITypeSymbol type)
    {
        return type.AllInterfaces.Where(i =>
                    i.IsGenericType
                    && i.ConstructedFrom.ToDisplayString() == SourceContextNames.IGameDataRepository
                )
                .Select(i => i.TypeArguments.Single())
                .SingleOrDefault()
            ?? type.GetAttributes().GetGameDataRepositoryInfos().Single().EntryType;
    }

    private static string GetSingularName(IPropertySymbol propertySymbol)
    {
        var explicitName = propertySymbol
            .GetAttributes()
            .GetSingularNameInfos()
            .Select(x => x.Name)
            .SingleOrDefault();
        if (!string.IsNullOrWhiteSpace(explicitName))
        {
            return explicitName;
        }

        return propertySymbol.Name.EndsWith("s") ? propertySymbol.Name[0..^1] : propertySymbol.Name;
    }

    private static bool HasAsset(ITypeSymbol type)
    {
        for (var baseType = type.BaseType; baseType is not null; baseType = baseType.BaseType)
        {
            if (baseType.ToDisplayString() == SourceContextNames.UAssetGameDataRepository)
            {
                return true;
            }
        }

        return false;
    }
}
