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
using UnrealSharp.GlueGenerator;
using UnrealSharp.GlueGenerator.NativeTypes;
using UnrealSharp.GlueGenerator.NativeTypes.Properties;

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
                    if (ModelExtensions.GetDeclaredSymbol(ctx.SemanticModel, classNode) is not INamedTypeSymbol syntax)
                    {
                        return null;
                    }

                    return syntax.HasAttribute<GameDataProviderAttribute>() ? syntax : null;
                }
            )
            .Where(s => s is not null);

        context.RegisterSourceOutput(providers, GenerateGameDataProvider!);
    }

    private static void GenerateGameDataProvider(SourceProductionContext context, INamedTypeSymbol classSymbol)
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

        var dataProviderInfo = classSymbol.GetGameDataProviderInfo();

        var repositories = classSymbol
            .GetMembers()
            .OfType<IPropertySymbol>()
            .Where(p => p.IsStatic)
            .Where(p => p.DeclaredAccessibility == Accessibility.Public)
            .Where(p => p.Type is INamedTypeSymbol namedType && namedType.IsGameDataRepository())
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
                RepositoryType = x.Type,
                EntryType = x.Type.GetEntryType(),
                HasAsset = HasAsset(x.Type),
            })
            .ToImmutableArray();

        var managerType = new UnrealClass(
            EClassFlags.None,
            "UGameDataManagerBase",
            "UnrealSharp.GameDataAccessTools",
            $"U{classSymbol.Name}Manager",
            classSymbol.ContainingNamespace.ToDisplayString(),
            Accessibility.Public,
            classSymbol.ContainingAssembly.Name
        );
        managerType.Overrides.List.Add("Initialize");

        var settingsType = new UnrealClass(
            EClassFlags.DefaultConfig,
            "UCSDeveloperSettings",
            "UnrealSharp.UnrealSharpCore",
            $"U{classSymbol.Name}Settings",
            classSymbol.ContainingNamespace.ToDisplayString(),
            Accessibility.Public,
            classSymbol.ContainingAssembly.Name
        );
        settingsType.AddMetaData("ConfigCategory", dataProviderInfo.Category);

        foreach (var repository in repositories)
        {
            ObjectProperty assetProperty;
            if (repository.HasAsset)
            {
                var objectProperty = new ObjectProperty(
                    repository.RepositoryClassName,
                    $"{repository.Name}_Inner",
                    Accessibility.Public,
                    settingsType
                );
                var property = new SoftObjectProperty(
                    objectProperty,
                    repository.Name,
                    Accessibility.Public,
                    "public",
                    settingsType
                )
                {
                    HasSetter = false,
                    PropertyFlags =
                        EPropertyFlags.EditDefaultsOnly | EPropertyFlags.BlueprintReadOnly | EPropertyFlags.Config,
                };
                property.AddMetaData("Category", "RepositoryAssets");
                objectProperty.Outer = property;
                settingsType.AddProperty(property);

                assetProperty = new ObjectProperty(
                    repository.RepositoryClassName,
                    $"{repository.Name}Asset",
                    Accessibility.Private,
                    managerType
                );
            }
            else
            {
                assetProperty = new ObjectProperty(
                    repository.RepositoryClassName,
                    repository.Name,
                    Accessibility.Public,
                    managerType
                )
                {
                    SetterAccessibility = Accessibility.Private,
                };
            }

            managerType.AddProperty(assetProperty);
        }

        var hasDisplayName = !string.IsNullOrWhiteSpace(dataProviderInfo.DisplayName);
        if (hasDisplayName)
        {
            settingsType.Overrides.List.Add("GetDisplayName");
        }

        var settingsModuleInitializerBuilder = new GeneratorStringBuilder();
        settingsModuleInitializerBuilder.BeginModuleInitializer(settingsType);

        var managerModuleInitializerBuilder = new GeneratorStringBuilder();
        managerModuleInitializerBuilder.BeginModuleInitializer(managerType);

        var templateParams = new
        {
            Assembly = classSymbol.ContainingAssembly.Name,
            Namespace = classSymbol.ContainingNamespace.ToDisplayString(),
            ClassName = classSymbol.Name,
            dataProviderInfo.DisplayName,
            ConfigCategory = dataProviderInfo.Category,
            HasDisplayName = hasDisplayName,
            Repositories = repositories,
            SettingsModuleInitializer = settingsModuleInitializerBuilder.ToString(),
            ManagerModuleInitializer = managerModuleInitializerBuilder.ToString(),
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

    private static string GetSingularName(IPropertySymbol propertySymbol)
    {
        if (propertySymbol.TryGetSingularNameInfo(out var nameInfo) && !string.IsNullOrWhiteSpace(nameInfo.Name))
        {
            return nameInfo.Name;
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
