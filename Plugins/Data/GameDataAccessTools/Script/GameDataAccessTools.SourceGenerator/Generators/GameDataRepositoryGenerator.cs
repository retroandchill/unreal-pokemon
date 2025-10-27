using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using GameAccessTools.SourceGenerator.Properties;
using GameAccessTools.SourceGenerator.Utilities;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using UnrealSharp.GlueGenerator;
using UnrealSharp.GlueGenerator.NativeTypes;
using UnrealSharp.GlueGenerator.NativeTypes.Properties;

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
                    if (ctx.SemanticModel.GetDeclaredSymbol(classNode) is not INamedTypeSymbol syntax)
                    {
                        return null;
                    }

                    return syntax.HasAttribute(typeof(GameDataRepositoryAttribute<>)) ? syntax : null;
                }
            )
            .Where(m => m is not null);

        context.RegisterSourceOutput(dataEntries, GenerateGameAssetData!);
    }

    private static void GenerateGameAssetData(SourceProductionContext context, INamedTypeSymbol classSymbol)
    {
        var isValidType = true;

        var uclassAttributeInfo = classSymbol
            .GetAttributes()
            .SingleOrDefault(x => x.AttributeClass?.ToDisplayString() == SourceContextNames.UClassAttribute);
        if (uclassAttributeInfo is not null)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA0001",
                        "GameDataRepository must not be annotated with UClass",
                        "{0} must not be annotated with UClass",
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

        var repositoryInfo = classSymbol.GetGameDataRepositoryInfo();

        var unrealClass = new UnrealClass(EClassFlags.None, classSymbol.BaseType!.Name,
            classSymbol.BaseType.ContainingNamespace.ToDisplayString(), classSymbol.Name, 
            classSymbol.ContainingNamespace.ToDisplayString(), classSymbol.DeclaredAccessibility,
            classSymbol.ContainingAssembly.Name);
        
        var innerProperty = new StructProperty(repositoryInfo.EntryType.ToDisplayString(), "Inner", Accessibility.NotApplicable, unrealClass);
        var dataEntriesProperty = new ArrayProperty(innerProperty, "DataEntries",
            $"TArray<{innerProperty.ManagedType}>", Accessibility.Private, "private", unrealClass)
        {
            HasSetter = false,
        };
        innerProperty.Outer = dataEntriesProperty;
        
        unrealClass.AddProperty(dataEntriesProperty);

        var moduleInitializerBuilder = new GeneratorStringBuilder();
        moduleInitializerBuilder.BeginModuleInitializer(unrealClass);
        
        var dataEntriesBuilder = new GeneratorStringBuilder();
        dataEntriesBuilder.Indent();
        dataEntriesProperty.ExportBackingVariables(dataEntriesBuilder, SourceGenUtilities.NativeTypePtr);
        dataEntriesProperty.ExportType(dataEntriesBuilder, context);

        var templateParams = new
        {
            Assembly = classSymbol.ContainingAssembly.Name,
            Namespace = classSymbol.ContainingNamespace.ToDisplayString(),
            AssetClassName = classSymbol.Name,
            EngineName = classSymbol.Name[1..],
            EntryName = repositoryInfo.EntryType.ToDisplayString(),
            IsStatic = isStatic,
            ModuleInitializer = moduleInitializerBuilder.ToString(),
            DataEntriesProperty = dataEntriesBuilder.ToString(),
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{templateParams.EngineName}.g.cs",
            handlebars.Compile(SourceTemplates.GameDataRepositoryTemplate)(templateParams)
        );
    }
}
