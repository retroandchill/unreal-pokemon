using System.Collections.Immutable;
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
public class DataHandleSourceGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var handleProvider = context
            .SyntaxProvider.CreateSyntaxProvider(
                (n, _) => n is RecordDeclarationSyntax,
                (ctx, _) =>
                {
                    var classNode = (RecordDeclarationSyntax)ctx.Node;
                    if (
                        ctx.SemanticModel.GetDeclaredSymbol(classNode)
                        is not INamedTypeSymbol { IsValueType: true } structSymbol
                    )
                    {
                        return null;
                    }

                    return structSymbol.HasAttribute<DataHandleBaseAttribute>() ? structSymbol : null;
                }
            )
            .Where(s => s is not null);

        context.RegisterSourceOutput(handleProvider, Execute!);
    }

    private static void Execute(SourceProductionContext context, INamedTypeSymbol structSymbol)
    {
        var isValidType = true;
        if (
            !structSymbol
                .GetAttributes()
                .Any(a => a.AttributeClass?.ToDisplayString() == SourceContextNames.UStructAttribute)
        )
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3001",
                        "Data handle should be marked UStruct",
                        "{0} must be annotated with UStruct",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structSymbol.Locations.First(),
                    structSymbol.Name
                )
            );
            isValidType = false;
        }

        if (!structSymbol.TryGetDataHandleBaseInfo(out var dataHandleInfo))
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3002",
                        "DataHandleInfo may only be used once",
                        "{0} may only use one DataHandleBaseInfo attribute",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structSymbol.Locations.First(),
                    structSymbol.Name
                )
            );
            isValidType = false;
        }

        if (
            !structSymbol
                .GetMembers()
                .OfType<IPropertySymbol>()
                .Any(p =>
                    !p.IsStatic
                    && p.DeclaredAccessibility == Accessibility.Public
                    && p.Type.ToDisplayString() == "UnrealSharp.Core.FName"
                    && p.Name == "ID"
                )
        )
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3003",
                        "Data handle type must have ID property",
                        "{0} needs a public, non-static property 'ID' of type FName",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structSymbol.Locations.First(),
                    structSymbol.Name
                )
            );
            isValidType = false;
        }

        if (
            !structSymbol
                .GetMembers()
                .OfType<IMethodSymbol>()
                .Any(c =>
                    c
                        is {
                            MethodKind: MethodKind.Constructor,
                            DeclaredAccessibility: Accessibility.Public,
                            Parameters.Length: 1
                        }
                    && c.Parameters[0].Type.ToDisplayString() == "UnrealSharp.Core.FName"
                )
        )
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3004",
                        "Data handle type must have a public constructor with FName parameter",
                        "{0} needs a public constructor with a single parameter of type FName",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structSymbol.Locations.First(),
                    structSymbol.Name
                )
            );
            isValidType = false;
        }

        if (!isValidType)
        {
            return;
        }

        DataHandleTemplateBase templateParams;
        try
        {
            templateParams = dataHandleInfo switch
            {
                DataHandleInfo providerRepository => new ProviderTemplateHandle(
                    structSymbol,
                    GetEntryType(providerRepository),
                    providerRepository.Type,
                    providerRepository.RepositoryName,
                    GetConvertibleTypes(dataHandleInfo)
                ),
                DataHandleInfoOneParam specificType => new ExplicitDataHandleTemplate(
                    structSymbol,
                    specificType.EntryType,
                    GetConvertibleTypes(dataHandleInfo)
                ),
                _ => throw new InvalidOperationException(
                    $"Unknown DataHandleInfo type: {dataHandleInfo!.GetType().Name}"
                ),
            };
        }
        catch (InvalidOperationException e)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3003",
                        "Unable to get entry type.",
                        "{0}",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structSymbol.Locations.First(),
                    e.Message
                )
            );
            return;
        }

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{structSymbol.Name}.g.cs",
            handlebars.Compile(SourceTemplates.RepositoryDataHandleTemplate)(templateParams)
        );

        var entryType = templateParams.EntryType;
        var hasViewMethods = entryType.HasAttribute<CreateStructViewAttribute>();

        var namespaceName = structSymbol.ContainingNamespace.ToDisplayString();
        var assemblyName = structSymbol.ContainingAssembly.Name;
        var engineName = structSymbol.Name[1..];

        var blueprintLibraryClass = new UnrealClass(
            EClassFlags.None,
            "UBlueprintFunctionLibrary",
            "UnrealSharp.Engine",
            $"U{engineName}BlueprintLibrary",
            namespaceName,
            Accessibility.Public,
            assemblyName
        );
        var functions = new EquatableList<UnrealFunction>([]);

        var isValidHandleFunction = new UnrealFunction(
            EFunctionFlags.BlueprintCallable | EFunctionFlags.BlueprintPure | EFunctionFlags.Static,
            "IsValidHandle",
            namespaceName,
            Accessibility.Public,
            assemblyName,
            blueprintLibraryClass
        );
        isValidHandleFunction.AddMetaData("DisplayName", "Is Valid");
        isValidHandleFunction.AddMetaData("Category", engineName);
        isValidHandleFunction.ReturnType = new BoolProperty("ReturnValue", Accessibility.Public, isValidHandleFunction)
        {
            PropertyFlags =
                EPropertyFlags.Parm
                | EPropertyFlags.ReturnParm
                | EPropertyFlags.OutParm
                | EPropertyFlags.BlueprintVisible
                | EPropertyFlags.BlueprintReadOnly,
        };
        isValidHandleFunction.Properties.List.Add(
            new StructProperty(
                structSymbol.ToDisplayString(),
                "handle",
                Accessibility.NotApplicable,
                isValidHandleFunction
            )
            {
                PropertyFlags =
                    EPropertyFlags.Parm | EPropertyFlags.BlueprintVisible | EPropertyFlags.BlueprintReadOnly,
            }
        );
        blueprintLibraryClass.AddFunction(isValidHandleFunction);
        functions.List.Add(isValidHandleFunction);

        var convertToNameFunction = new UnrealFunction(
            EFunctionFlags.BlueprintCallable | EFunctionFlags.BlueprintPure | EFunctionFlags.Static,
            $"Convert{engineName}ToName",
            namespaceName,
            Accessibility.Public,
            assemblyName,
            blueprintLibraryClass
        );
        convertToNameFunction.AddMetaData("DisplayName", "Convert to Name");
        convertToNameFunction.AddMetaData("Category", engineName);
        convertToNameFunction.AddMetaData("CompactNodeTitle", "->");
        convertToNameFunction.AddMetaData("BlueprintAutoCast", "");
        convertToNameFunction.ReturnType = new NameProperty("ReturnValue", Accessibility.Public, convertToNameFunction)
        {
            PropertyFlags =
                EPropertyFlags.Parm
                | EPropertyFlags.ReturnParm
                | EPropertyFlags.OutParm
                | EPropertyFlags.BlueprintVisible
                | EPropertyFlags.BlueprintReadOnly,
        };
        convertToNameFunction.Properties.List.Add(
            new StructProperty(
                structSymbol.ToDisplayString(),
                "handle",
                Accessibility.NotApplicable,
                convertToNameFunction
            )
            {
                PropertyFlags =
                    EPropertyFlags.Parm | EPropertyFlags.BlueprintVisible | EPropertyFlags.BlueprintReadOnly,
            }
        );
        blueprintLibraryClass.AddFunction(convertToNameFunction);
        functions.List.Add(convertToNameFunction);

        var convertFromNameFunction = new UnrealFunction(
            EFunctionFlags.BlueprintCallable | EFunctionFlags.BlueprintPure | EFunctionFlags.Static,
            $"ConvertNameTo{engineName}",
            namespaceName,
            Accessibility.Public,
            assemblyName,
            blueprintLibraryClass
        );
        convertFromNameFunction.AddMetaData("DisplayName", $"Convert to {engineName}");
        convertFromNameFunction.AddMetaData("Category", engineName);
        convertFromNameFunction.AddMetaData("CompactNodeTitle", "->");
        convertFromNameFunction.AddMetaData("BlueprintAutoCast", "");
        convertFromNameFunction.ReturnType = new StructProperty(
            structSymbol.ToDisplayString(),
            "ReturnValue",
            Accessibility.NotApplicable,
            convertFromNameFunction
        )
        {
            PropertyFlags =
                EPropertyFlags.Parm
                | EPropertyFlags.ReturnParm
                | EPropertyFlags.OutParm
                | EPropertyFlags.BlueprintVisible
                | EPropertyFlags.BlueprintReadOnly,
        };
        convertFromNameFunction.Properties.List.Add(
            new NameProperty("name", Accessibility.NotApplicable, convertFromNameFunction)
            {
                PropertyFlags =
                    EPropertyFlags.Parm | EPropertyFlags.BlueprintVisible | EPropertyFlags.BlueprintReadOnly,
            }
        );
        blueprintLibraryClass.AddFunction(convertFromNameFunction);
        functions.List.Add(convertFromNameFunction);

        var properties = hasViewMethods
            ? entryType
                .GetMembers()
                .OfType<IPropertySymbol>()
                .Where(s => !s.IsStatic && s.GetMethod is not null)
                .Select(x => new DataHandlePropertyMethod(x))
                .ToImmutableArray()
            : [];

        foreach (var property in properties)
        {
            var propertyGetter = new UnrealFunction(
                EFunctionFlags.BlueprintCallable | EFunctionFlags.BlueprintPure | EFunctionFlags.Static,
                $"{property.Prefix}{property.Name}",
                namespaceName,
                Accessibility.Public,
                assemblyName,
                blueprintLibraryClass
            );
            propertyGetter.AddMetaData("Category", engineName);

            var propertySyntax = property
                .Property.DeclaringSyntaxReferences.Select(x => x.GetSyntax())
                .FirstOrDefault();
            propertyGetter.ReturnType = PropertyFactory.CreateProperty(
                property.Property.Type,
                propertySyntax!,
                property.Property,
                propertyGetter
            );
            propertyGetter.ReturnType.SourceName = "ReturnValue";
            propertyGetter.ReturnType.PropertyFlags =
                EPropertyFlags.Parm
                | EPropertyFlags.ReturnParm
                | EPropertyFlags.OutParm
                | EPropertyFlags.BlueprintVisible
                | EPropertyFlags.BlueprintReadOnly;

            propertyGetter.Properties.List.Add(
                new StructProperty(
                    structSymbol.ToDisplayString(),
                    "handle",
                    Accessibility.NotApplicable,
                    propertyGetter
                )
                {
                    PropertyFlags = EPropertyFlags.Parm,
                }
            );
            blueprintLibraryClass.AddFunction(propertyGetter);
            functions.List.Add(propertyGetter);
        }

        var functionGlueBuilder = new GeneratorStringBuilder();
        functionGlueBuilder.Indent();
        blueprintLibraryClass.TryExportList(functionGlueBuilder, context, functions);

        var moduleInitializerBuilder = new GeneratorStringBuilder();
        moduleInitializerBuilder.BeginModuleInitializer(blueprintLibraryClass);

        var blueprintLibraryParams = new
        {
            Namespace = structSymbol.ContainingNamespace.ToDisplayString(),
            StructName = structSymbol.Name,
            Assembly = assemblyName,
            EngineName = engineName,
            Properties = properties,
            FunctionGlue = functionGlueBuilder.ToString(),
            ModuleInitializer = moduleInitializerBuilder.ToString(),
        };

        context.AddSource(
            $"{structSymbol.Name}BlueprintLibrary.g.cs",
            handlebars.Compile(SourceTemplates.RepositoryDataHandleBlueprintLibraryTemplate)(blueprintLibraryParams)
        );
    }

    private static ITypeSymbol GetEntryType(DataHandleInfo dataHandleInfo)
    {
        var providerType = dataHandleInfo.Type;

        if (!providerType.HasAttribute<GameDataProviderAttribute>())
        {
            throw new InvalidOperationException(
                $"Provider class must be marked with {nameof(GameDataProviderAttribute)}"
            );
        }

        var property = providerType
            .GetMembers()
            .OfType<IPropertySymbol>()
            .SingleOrDefault(s => s.Name == dataHandleInfo.RepositoryName);

        if (property is not { Type: INamedTypeSymbol typeSymbol })
        {
            throw new InvalidOperationException(
                $"Provider {dataHandleInfo.Type.Name} class does not have property named {dataHandleInfo.RepositoryName}"
            );
        }

        if (!typeSymbol.IsGameDataRepository())
        {
            throw new InvalidOperationException(
                $"Property {dataHandleInfo.RepositoryName} on {dataHandleInfo.Type.Name} is not a GameDataRepository"
            );
        }

        return typeSymbol.GetEntryType();
    }

    private static ImmutableArray<ConvertibleType> GetConvertibleTypes(DataHandleBaseInfo info)
    {
        return
        [
            .. info
                .ComparableTypes.Where(t => t.HasAttribute<DataHandleBaseAttribute>())
                .Select(t => new ConvertibleType(t)),
        ];
    }
}
