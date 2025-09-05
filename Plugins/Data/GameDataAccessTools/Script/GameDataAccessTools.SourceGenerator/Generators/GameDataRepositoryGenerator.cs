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
using RhoMicro.CodeAnalysis.Library.Extensions;

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
                        ModelExtensions.GetDeclaredSymbol(ctx.SemanticModel, classNode)
                        is not INamedTypeSymbol syntax
                    )
                    {
                        return null;
                    }

                    return syntax.HasAttribute<GameDataEntryAttribute>() ? syntax : null;
                }
            )
            .Where(m => m is not null)
            .Collect();

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

                    return syntax.HasAttribute<GameDataRepositoryProviderAttribute>()
                        ? syntax
                        : null;
                }
            )
            .Where(m => m is not null)
            .Collect();

        var combinedInfo = dataEntries.Combine(providers);

        context.RegisterSourceOutput(
            combinedInfo,
            (ctx, info) =>
            {
                Execute(ctx, info.Left!, info.Right!);
            }
        );
    }

    private static void Execute(
        SourceProductionContext context,
        ImmutableArray<INamedTypeSymbol> entryTypes,
        ImmutableArray<INamedTypeSymbol> providerTypes
    )
    {
        var foundDataEntries = entryTypes
            .Select(x => GenerateGameAssetData(context, x))
            .Where(x => x.HasValue)
            .Select(x => x!.Value)
            .ToDictionary(x => x.AssetClassName);

        foreach (var (_, info) in foundDataEntries)
        {
            GeneratePropertyAccessorType(info.EntryType, context);
        }

        foreach (var providerType in providerTypes)
        {
            GenerateProviderType(context, providerType, foundDataEntries);
        }
    }

    private static GameDataRepositoryInfo? GenerateGameAssetData(
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
                        "GameDataEntry must be annotated with UClass",
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
        else if (
            uclassAttributeInfo.ConstructorArguments[0].Value is ulong propertyTag
            && (propertyTag & SourceContextNames.EditInlineNew) == 0
        )
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA0002",
                        "GameDataEntry must have EditInlineNew flag set on UClass",
                        "{0} must have EditInlineNew flag set on UClass",
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
        while (baseType is not null)
        {
            if (baseType.ToDisplayString() == SourceContextNames.AActor)
            {
                context.ReportDiagnostic(
                    Diagnostic.Create(
                        new DiagnosticDescriptor(
                            "GDA0005",
                            "GameDataEntry may not inherit from AActor",
                            "{0} may not inherit from AActor",
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

            if (baseType.ToDisplayString() == SourceContextNames.UObject)
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
                        "GameDataEntry must inherit from UObject",
                        "{0} must inherit from UObject",
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

        if (
            !classSymbol.AllInterfaces.Any(i =>
                i.ToDisplayString() == SourceContextNames.IGameDataEntry
            )
        )
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA0004",
                        "GameDataEntry must implement from IGameDataEntry",
                        "{0} must implement from IGameDataEntry",
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
            return null;
        }

        var gameDataEntryInfo = classSymbol.GetAttributes().GetGameDataEntryInfos().Single();
        string generatedClassName;
        if (gameDataEntryInfo.GeneratedClassName is not null)
        {
            generatedClassName = gameDataEntryInfo.GeneratedClassName;
        }
        else
        {
            generatedClassName = classSymbol.Name.EndsWith("Data")
                ? $"U{classSymbol.Name[1..]}Repository"
                : $"U{classSymbol.Name[1..]}DataRepository";
        }

        var templateParams = new GameDataRepositoryInfo(classSymbol, generatedClassName);

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{generatedClassName[1..]}.g.cs",
            handlebars.Compile(SourceTemplates.GameDataRepositoryTemplate)(templateParams)
        );
        return templateParams;
    }

    private static void GenerateProviderType(
        SourceProductionContext context,
        INamedTypeSymbol providerType,
        Dictionary<string, GameDataRepositoryInfo> foundDataEntries
    )
    {
        var providerInfo = providerType
            .GetAttributes()
            .GetGameDataRepositoryProviderInfos()
            .Single();

        var isValidType = true;

        if (
            !providerType
                .DeclaringSyntaxReferences.Select(r => r.GetSyntax())
                .OfType<ClassDeclarationSyntax>()
                .SelectMany(x => x.Modifiers)
                .Any(m => m.IsKind(SyntaxKind.PartialKeyword))
        )
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA0004",
                        "Provider type must be declared as partial",
                        "{0} must be declared as partial",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    providerType.Locations.First(),
                    providerType.Name
                )
            );
            isValidType = false;
        }

        if (!isValidType)
        {
            return;
        }

        var repositories = providerType
            .GetMembers()
            .OfType<IPropertySymbol>()
            .Where(p => p.IsStatic)
            .Where(p => p.DeclaredAccessibility == Accessibility.Public)
            .Where(p =>
                p.Type is INamedTypeSymbol namedType
                && IsGameDataRepository(namedType, foundDataEntries)
            )
            .Where(p => p.GetMethod is not null && p.SetMethod is null)
            .Where(p =>
                p.DeclaringSyntaxReferences.Select(r => r.GetSyntax())
                    .OfType<PropertyDeclarationSyntax>()
                    .Any(x => x.Modifiers.Any(m => m.IsKind(SyntaxKind.PartialKeyword)))
            )
            .Select(x => new RepositoryPropertyInfo
            {
                Type = x.Type.BaseType is not null
                    ? x.Type.ToDisplayString()
                    : GetFormattedName(foundDataEntries, x.Type.Name),
                Name = x.Name,
                SingularName = GetSingularName(x),
                RepositoryClassName = x.Type.Name[1..],
                EntryType = GetEntryType(x.Type, foundDataEntries),
                Category = x.GetAttributes().GetSettingsCategoryInfos().SingleOrDefault().Name,
            })
            .ToImmutableArray();

        var templateParams = new
        {
            Namespace = providerType.ContainingNamespace.ToDisplayString(),
            ClassName = providerType.Name,
            DisplayName = providerInfo.SettingsDisplayName,
            HasDisplayName = providerInfo.SettingsDisplayName is not null,
            Repositories = repositories,
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{providerType.Name}.g.cs",
            handlebars.Compile(SourceTemplates.GameDataRepositoryProviderTemplate)(templateParams)
        );
    }

    private static bool IsGameDataRepository(
        INamedTypeSymbol type,
        Dictionary<string, GameDataRepositoryInfo> foundDataEntries
    )
    {
        if (
            type.BaseType?.ToDisplayString() == SourceContextNames.UGameDataRepository
            && type.Interfaces.Any(i =>
                i.IsGenericType
                && i.ConstructedFrom.ToDisplayString() == SourceContextNames.IGameDataRepository
            )
        )
        {
            return true;
        }

        return foundDataEntries.ContainsKey(type.Name);
    }

    private static ITypeSymbol GetEntryType(
        ITypeSymbol type,
        Dictionary<string, GameDataRepositoryInfo> foundDataEntries
    )
    {
        if (type.BaseType?.ToDisplayString() == SourceContextNames.UGameDataRepository)
        {
            return type
                .Interfaces.Where(i =>
                    i.IsGenericType
                    && i.ConstructedFrom.ToDisplayString() == SourceContextNames.IGameDataRepository
                )
                .Select(i => i.TypeArguments.Single())
                .Single();
        }

        return foundDataEntries[type.Name].EntryType;
    }

    private static string GetFormattedName(
        Dictionary<string, GameDataRepositoryInfo> foundDataEntries,
        string name
    )
    {
        return foundDataEntries.TryGetValue(name, out var entry)
            ? $"{entry.Namespace}.{entry.AssetClassName}"
            : name;
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

    private static void GeneratePropertyAccessorType(
        INamedTypeSymbol targetType,
        SourceProductionContext context
    )
    {
        List<ITypeSymbol> allTypes = [targetType];
        var baseType = targetType.BaseType;
        while (baseType is not null)
        {
            allTypes.Add(baseType);
            if (baseType.ToDisplayString() == SourceContextNames.UObject)
            {
                break;
            }

            baseType = baseType.BaseType;
        }

        var properties = ((IEnumerable<ITypeSymbol>)allTypes)
            .Reverse()
            .SelectMany(x => x.GetMembers())
            .OfType<IPropertySymbol>()
            .Where(x => !x.IsStatic)
            .Where(x =>
                x.GetAttributes()
                    .Any(y =>
                        y.AttributeClass?.ToDisplayString() == SourceContextNames.UPropertyAttribute
                    )
            )
            .Select(x => x.GetPropertyInfo())
            .ToImmutableArray();

        var templateParams = new
        {
            Namespace = targetType.ContainingNamespace.ToDisplayString(),
            ClassName = targetType.Name,
            EngineName = targetType.Name[1..],
            Properties = properties,
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{templateParams.EngineName}Initializer.g.cs",
            handlebars.Compile(SourceTemplates.GameDataEntrInitializerTemplate)(templateParams)
        );
    }

    private static string GetClassType(INamedTypeSymbol classSymbol)
    {
        if (classSymbol.TypeKind == TypeKind.Struct)
        {
            return classSymbol.IsRecord ? "record struct" : "struct";
        }

        return classSymbol.IsRecord ? "record" : "class";
    }
}
