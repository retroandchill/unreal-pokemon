using System.Collections.Immutable;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Types;
using RhoMicro.CodeAnalysis.Library.Extensions;
using RPG.SourceGenerator.Attributes;
using RPG.SourceGenerator.Model;
using RPG.SourceGenerator.Properties;
using TurnBased.SourceGenerator;
using UnrealSharp.GlueGenerator;
using UnrealSharp.GlueGenerator.NativeTypes;
using UnrealSharp.GlueGenerator.NativeTypes.Properties;

namespace RPG.SourceGenerator.Generators;

[Generator]
public class RPGEntityExtensionGenerator : IIncrementalGenerator
{
    private readonly Dictionary<ITypeSymbol, ComponentInfo> _components = new(SymbolEqualityComparer.Default);

    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        _components.Clear();
        var classesProvider = context
            .SyntaxProvider.CreateSyntaxProvider(
                predicate: (s, _) => s is ClassDeclarationSyntax,
                transform: (ctx, _) =>
                {
                    var classNode = (ClassDeclarationSyntax)ctx.Node;
                    if (
                        ModelExtensions.GetDeclaredSymbol(ctx.SemanticModel, classNode)
                        is not INamedTypeSymbol classSymbol
                    )
                    {
                        return null;
                    }

                    return classSymbol.IsTurnBasedUnit() ? classSymbol : null;
                }
            )
            .Where(m => m is not null);

        context.RegisterSourceOutput(classesProvider, Execute!);
    }

    private void Execute(SourceProductionContext context, INamedTypeSymbol classSymbol)
    {
        var componentProperties = classSymbol
            .GetMembers()
            .OfType<IPropertySymbol>()
            .Where(p => p.Type.IsTurnBasedUnitComponent())
            .ToArray();

        var factoryMethods = new List<IMethodSymbol>();
        foreach (
            var method in classSymbol
                .GetMembers()
                .OfType<IMethodSymbol>()
                .Where(m => m.HasAttribute<FactoryAttribute>())
        )
        {
            var isValidMethod = true;
            if (!method.IsStatic)
            {
                context.ReportDiagnostic(
                    Diagnostic.Create(
                        new DiagnosticDescriptor(
                            "RPG0002",
                            "Factory must be static",
                            "{0} must be static",
                            "RPGEssentials",
                            DiagnosticSeverity.Error,
                            true
                        ),
                        method.Locations.First(),
                        method.Name
                    )
                );
                isValidMethod = false;
            }

            if (!SymbolEqualityComparer.Default.Equals(method.ReturnType, classSymbol))
            {
                context.ReportDiagnostic(
                    Diagnostic.Create(
                        new DiagnosticDescriptor(
                            "RPG0006",
                            "Factory must return the same type",
                            "{0} must return {1}",
                            "RPGEssentials",
                            DiagnosticSeverity.Error,
                            true
                        ),
                        method.Locations.First(),
                        method,
                        classSymbol
                    )
                );
            }

            if (method.Parameters.Length != 2)
            {
                context.ReportDiagnostic(
                    Diagnostic.Create(
                        new DiagnosticDescriptor(
                            "RPG0003",
                            "Factory must take two parameters",
                            "{0} must take exactly two parameters",
                            "RPGEssentials",
                            DiagnosticSeverity.Error,
                            true
                        ),
                        method.Locations.First(),
                        method.Name
                    )
                );
                continue;
            }

            if (!method.Parameters[0].Type.IsUObject())
            {
                context.ReportDiagnostic(
                    Diagnostic.Create(
                        new DiagnosticDescriptor(
                            "RPG0004",
                            "Factory's first parameter must be a UObject",
                            "{0} must be a UObject or UObject derived class",
                            "RPGEssentials",
                            DiagnosticSeverity.Error,
                            true
                        ),
                        method.Parameters[0].Locations.First(),
                        method.Parameters[0]
                    )
                );
                isValidMethod = false;
            }

            if (
                !method
                    .Parameters[1]
                    .Type.GetAttributes()
                    .Any(a => a.AttributeClass?.ToDisplayString() == GeneratorStatics.UStructAttribute)
            )
            {
                context.ReportDiagnostic(
                    Diagnostic.Create(
                        new DiagnosticDescriptor(
                            "RPG0005",
                            "Factory's second parameter must be a UStruct",
                            "{0} must be a UStruct",
                            "RPGEssentials",
                            DiagnosticSeverity.Error,
                            true
                        ),
                        method.Parameters[1].Locations.First(),
                        method.Parameters[1]
                    )
                );
                isValidMethod = false;
            }

            if (!isValidMethod)
                continue;

            factoryMethods.Add(method);
        }

        if (factoryMethods.Count > 1)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "RPG0007",
                        "Factory may only have one factory method",
                        "{0} may only have one factory method",
                        "RPGEssentials",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    factoryMethods[0].Locations.First(),
                    factoryMethods.Skip(1).Select(m => m.Locations.First()),
                    classSymbol
                )
            );
        }

        var factoryMethod = factoryMethods.FirstOrDefault();

        var factoryInfo = factoryMethod?.GetAttributes().GetFactoryInfos().Single();

        var subclassSource = factoryInfo?.SubclassSource;

        var subclassSourceSymbol = classSymbol
            .GetMembers()
            .FirstOrDefault(m =>
                m.IsStatic
                && m
                    is IPropertySymbol { GetMethod: not null }
                        or IMethodSymbol { Parameters.Length: 0 }
                        or IFieldSymbol
            );

        if (subclassSource is not null && subclassSourceSymbol is null)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "RPG0008",
                        "Factory with subclass source must exist",
                        "{0} must be a valid static method, property, or field on {1}",
                        "RPGEssentials",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    factoryMethod!.Locations.First(),
                    subclassSource,
                    classSymbol
                )
            );
        }

        var components = componentProperties
            .Select(
                (p, i) =>
                    GetComponentInfo(p, i == componentProperties.Length - 1, factoryMethod?.Parameters[1].Type, context)
            )
            .ToImmutableArray();

        var namespaceName = classSymbol.ContainingNamespace.ToDisplayString();
        var assemblyName = classSymbol.ContainingAssembly.Name;

        var blueprintLibraryClass = new UnrealClass(
            EClassFlags.None,
            "UBlueprintFunctionLibrary",
            "UnrealSharp.Engine",
            $"{classSymbol.Name}BlueprintLibrary",
            classSymbol.ContainingNamespace.ToDisplayString(),
            Accessibility.Public,
            classSymbol.ContainingAssembly.Name
        );
        var functions = new EquatableList<UnrealFunctionBase>([]);

        foreach (var component in components)
        {
            foreach (var property in component.Properties.Where(p => p.IsExposed))
            {
                var propertySyntax = property
                    .Property.DeclaringSyntaxReferences.Select(r => r.GetSyntax())
                    .FirstOrDefault();

                if (property.HasGetter)
                {
                    var propertyGetter = new UnrealFunction(
                        EFunctionFlags.Static,
                        $"Get{property.Name}",
                        namespaceName,
                        Accessibility.Public,
                        assemblyName,
                        blueprintLibraryClass
                    );
                    if (property.GetterIsUFunction)
                    {
                        ApplyUFunctionAttribute(property.Property.GetMethod!, propertyGetter);
                    }
                    else
                    {
                        propertyGetter.FunctionFlags |= EFunctionFlags.BlueprintCallable;
                        if (property.HasDisplayName)
                        {
                            propertyGetter.AddMetaData("DisplayName", $"Get {property.DisplayName}");
                        }

                        if (property.HasCategory)
                        {
                            propertyGetter.AddMetaData("Category", property.Category);
                        }
                    }

                    propertyGetter.AddMetaData("DefaultToSelf", "unit");

                    propertyGetter.Properties.List.Add(
                        new ObjectProperty(classSymbol.ToDisplayString(), "unit", Accessibility.Public, propertyGetter)
                        {
                            PropertyFlags =
                                EPropertyFlags.Parm
                                | EPropertyFlags.BlueprintVisible
                                | EPropertyFlags.BlueprintReadOnly,
                        }
                    );
                    if (property.IsOptionOrNullableType)
                    {
                        propertyGetter.AddMetaData("ExpandEnumAsExecs", "ReturnValue");
                        propertyGetter.ReturnType = new EnumProperty(
                            "EValueFindResult",
                            "ReturnValue",
                            Accessibility.NotApplicable,
                            propertyGetter
                        )
                        {
                            PropertyFlags =
                                EPropertyFlags.Parm
                                | EPropertyFlags.ReturnParm
                                | EPropertyFlags.OutParm
                                | EPropertyFlags.BlueprintVisible
                                | EPropertyFlags.BlueprintReadOnly,
                        };

                        var outProperty = PropertyFactory.CreateProperty(
                            property.UnderlyingType,
                            propertySyntax!,
                            property.Property,
                            propertyGetter
                        );
                        outProperty.PropertyFlags =
                            EPropertyFlags.Parm
                            | EPropertyFlags.OutParm
                            | EPropertyFlags.BlueprintVisible
                            | EPropertyFlags.BlueprintReadOnly;
                        outProperty.RefKind = RefKind.Out;
                        propertyGetter.FunctionFlags |= EFunctionFlags.HasOutParms;
                        propertyGetter.Properties.List.Add(outProperty);
                    }
                    else
                    {
                        propertyGetter.FunctionFlags |= EFunctionFlags.BlueprintPure;

                        propertyGetter.ReturnType = PropertyFactory.CreateProperty(
                            property.Type,
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
                    }

                    blueprintLibraryClass.AddFunction(propertyGetter);
                    functions.List.Add(propertyGetter);
                }

                if (!property.HasSetter)
                    continue;

                var propertySetter = new UnrealFunction(
                    EFunctionFlags.Static,
                    $"Set{property.Name}",
                    namespaceName,
                    Accessibility.Public,
                    assemblyName,
                    blueprintLibraryClass
                );
                if (property.SetterIsUFunction)
                {
                    ApplyUFunctionAttribute(property.Property.SetMethod!, propertySetter);
                }
                else
                {
                    propertySetter.FunctionFlags |= EFunctionFlags.BlueprintCallable;
                    if (property.HasDisplayName)
                    {
                        propertySetter.AddMetaData("DisplayName", $"Set {property.DisplayName}");
                    }

                    if (property.HasCategory)
                    {
                        propertySetter.AddMetaData("Category", property.Category);
                    }
                }

                propertySetter.AddMetaData("DefaultToSelf", "unit");
                propertySetter.ReturnType = new VoidProperty(propertySetter);

                propertySetter.Properties.List.Add(
                    new ObjectProperty(classSymbol.ToDisplayString(), "unit", Accessibility.Public, propertySetter)
                    {
                        PropertyFlags =
                            EPropertyFlags.Parm | EPropertyFlags.BlueprintVisible | EPropertyFlags.BlueprintReadOnly,
                    }
                );

                var propertyValueType = PropertyFactory.CreateProperty(
                    property.UnderlyingType,
                    propertySyntax!,
                    property.Property,
                    propertySetter
                );
                propertyValueType.PropertyFlags =
                    EPropertyFlags.Parm | EPropertyFlags.BlueprintVisible | EPropertyFlags.BlueprintReadOnly;
                propertySetter.Properties.List.Add(propertyValueType);

                blueprintLibraryClass.AddFunction(propertySetter);
                functions.List.Add(propertySetter);

                if (!property.IsOptionOrNullableType)
                    continue;

                var propertyReset = new UnrealFunction(
                    EFunctionFlags.BlueprintCallable | EFunctionFlags.Static,
                    $"Reset{property.Name}",
                    namespaceName,
                    Accessibility.Public,
                    assemblyName,
                    blueprintLibraryClass
                );
                if (property.HasDisplayName)
                {
                    propertyReset.AddMetaData("DisplayName", $"Set {property.DisplayName}");
                }

                if (property.HasCategory)
                {
                    propertyReset.AddMetaData("Category", property.Category);
                }
                propertyReset.AddMetaData("DefaultToSelf", "unit");
                propertyReset.ReturnType = new VoidProperty(propertyReset);

                propertyReset.Properties.List.Add(
                    new ObjectProperty(classSymbol.ToDisplayString(), "unit", Accessibility.Public, propertyReset)
                    {
                        PropertyFlags =
                            EPropertyFlags.Parm | EPropertyFlags.BlueprintVisible | EPropertyFlags.BlueprintReadOnly,
                    }
                );

                blueprintLibraryClass.AddFunction(propertyReset);
                functions.List.Add(propertyReset);
            }

            foreach (var method in component.Methods.Where(m => m.IsExposed))
            {
                UnrealFunctionBase methodWrapper;
                if (method.Method.ReturnType.MetadataName is "Task" or "Task`1" or "ValueTask" or "ValueTask`1")
                {
                    methodWrapper = new UnrealAsyncFunction(
                        EFunctionFlags.Static,
                        method.Name,
                        namespaceName,
                        Accessibility.Public,
                        assemblyName,
                        blueprintLibraryClass
                    );
                }
                else
                {
                    methodWrapper = new UnrealFunction(
                        EFunctionFlags.Static,
                        method.Name,
                        namespaceName,
                        Accessibility.Public,
                        assemblyName,
                        blueprintLibraryClass
                    );
                }

                ApplyUFunctionAttribute(method.Method, methodWrapper);
                methodWrapper.AddMetaData("DefaultToSelf", "unit");

                methodWrapper.Properties.List.Add(
                    new ObjectProperty(classSymbol.ToDisplayString(), "unit", Accessibility.Public, methodWrapper)
                    {
                        PropertyFlags =
                            EPropertyFlags.Parm | EPropertyFlags.BlueprintVisible | EPropertyFlags.BlueprintReadOnly,
                    }
                );

                if (method.ReturnsOptionOrNullable)
                {
                    methodWrapper.ReturnType = new EnumProperty(
                        "EValueFindResult",
                        "ReturnValue",
                        Accessibility.NotApplicable,
                        methodWrapper
                    )
                    {
                        PropertyFlags =
                            EPropertyFlags.Parm
                            | EPropertyFlags.ReturnParm
                            | EPropertyFlags.OutParm
                            | EPropertyFlags.BlueprintVisible
                            | EPropertyFlags.BlueprintReadOnly,
                    };

                    var outParameter = PropertyFactory.CreateProperty(
                        method.UnderlyingType,
                        null!,
                        method.ReturnType,
                        methodWrapper
                    );
                    outParameter.PropertyFlags =
                        EPropertyFlags.Parm
                        | EPropertyFlags.OutParm
                        | EPropertyFlags.BlueprintVisible
                        | EPropertyFlags.BlueprintReadOnly;
                    outParameter.RefKind = RefKind.Out;

                    methodWrapper.FunctionFlags |= EFunctionFlags.HasOutParms;

                    methodWrapper.Properties.List.Add(outParameter);
                }
                else if (method.Method.ReturnsVoid)
                {
                    methodWrapper.ReturnType = new VoidProperty(methodWrapper);
                }
                else
                {
                    methodWrapper.ReturnType = PropertyFactory.CreateProperty(
                        method.ReturnType,
                        null!,
                        method.Method,
                        methodWrapper
                    );
                    methodWrapper.ReturnType.SourceName = "ReturnValue";
                    methodWrapper.ReturnType.PropertyFlags =
                        EPropertyFlags.Parm
                        | EPropertyFlags.ReturnParm
                        | EPropertyFlags.OutParm
                        | EPropertyFlags.BlueprintVisible
                        | EPropertyFlags.BlueprintReadOnly;
                }

                var hasOutParams = false;
                var paramHasDefaults = false;
                foreach (var parameter in method.Parameters)
                {
                    var param = PropertyFactory.CreateProperty(
                        parameter.Type,
                        null!,
                        parameter.Parameter,
                        methodWrapper
                    );
                    param.PropertyFlags =
                        EPropertyFlags.Parm | EPropertyFlags.BlueprintVisible | EPropertyFlags.BlueprintReadOnly;

                    switch (parameter.Parameter.RefKind)
                    {
                        case RefKind.Out:
                            param.PropertyFlags |= EPropertyFlags.OutParm;
                            hasOutParams = true;
                            break;
                        case RefKind.Ref:
                            param.PropertyFlags |= EPropertyFlags.OutParm | EPropertyFlags.ReferenceParm;
                            hasOutParams = true;
                            break;
                        case RefKind.None:
                        case RefKind.In:
                        case RefKind.RefReadOnlyParameter:
                            break;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }

                    if (parameter.Parameter is { HasExplicitDefaultValue: true, ExplicitDefaultValue: not null })
                    {
                        string defaultValue;
                        if (parameter.Parameter.Type.TypeKind == TypeKind.Enum)
                        {
                            defaultValue = SourceGenUtilities.GetEnumNameFromValue(
                                parameter.Parameter.Type,
                                (byte)parameter.Parameter.ExplicitDefaultValue
                            );
                        }
                        else
                        {
                            defaultValue = parameter.Parameter.ExplicitDefaultValue.ToString();
                        }

                        methodWrapper.AddMetaData($"CPP_Default_{parameter.Parameter.Name}", defaultValue);
                        paramHasDefaults = true;
                    }

                    methodWrapper.Properties.List.Add(param);
                }

                if (hasOutParams)
                {
                    methodWrapper.FunctionFlags |= EFunctionFlags.HasOutParms;
                }

                if (paramHasDefaults)
                {
                    methodWrapper.FunctionFlags |= EFunctionFlags.HasDefaults;
                }

                blueprintLibraryClass.AddFunction(methodWrapper);
                functions.List.Add(methodWrapper);
            }
        }

        var functionGlueBuilder = new GeneratorStringBuilder();
        functionGlueBuilder.Indent();
        blueprintLibraryClass.TryExportList(functionGlueBuilder, context, functions);

        var moduleInitializerBuilder = new GeneratorStringBuilder();
        moduleInitializerBuilder.BeginModuleInitializer(blueprintLibraryClass);

        var templateParams = new RPGEntityInfo(classSymbol)
        {
            Components = components,
            FactoryMethod = factoryMethod,
            SubclassSourceSymbol = subclassSourceSymbol,
            FunctionGlue = functionGlueBuilder.ToString(),
            ModuleInitializer = moduleInitializerBuilder.ToString(),
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{templateParams.EngineName}Extensions.g.cs",
            handlebars.Compile(SourceTemplates.RPGEntityExtensionsTemplate)(templateParams)
        );

        context.AddSource(
            $"{templateParams.EngineName}BlueprintLibrary.g.cs",
            handlebars.Compile(SourceTemplates.RPGEntityBlueprintLibraryTemplate)(templateParams)
        );

        if (
            !classSymbol
                .DeclaringSyntaxReferences.Select(r => r.GetSyntax())
                .OfType<ClassDeclarationSyntax>()
                .SelectMany(x => x.Modifiers)
                .Any(m => m.IsKind(SyntaxKind.PartialKeyword))
        )
        {
            return;
        }

        context.AddSource(
            $"{templateParams.EngineName}.g.cs",
            handlebars.Compile(SourceTemplates.RPGEntityTemplate)(templateParams)
        );
    }

    private ComponentInfo GetComponentInfo(
        IPropertySymbol propertySymbol,
        bool isLast,
        ITypeSymbol? initializerStruct,
        SourceProductionContext context
    )
    {
        if (_components.TryGetValue(propertySymbol.Type, out var componentInfo))
        {
            return componentInfo with { ComponentName = propertySymbol.Name, IsLast = isLast };
        }

        var initializerName = propertySymbol
            .GetAttributes()
            .GetInitializerInfos()
            .Select(x => x.InitializerName)
            .FirstOrDefault();

        var initializerMethod = propertySymbol
            .Type.GetMembers()
            .OfType<IMethodSymbol>()
            .Where(m => m.Name == initializerName)
            .Where(m =>
                !m.IsStatic
                && m.GetAttributes()
                    .Any(a => a.AttributeClass?.ToDisplayString() == GeneratorStatics.UFunctionAttribute)
            )
            .FirstOrDefault(m =>
                m.Parameters.Length == 1
                && SymbolEqualityComparer.Default.Equals(m.Parameters[0].Type, initializerStruct)
            );

        if (initializerName is not null && initializerMethod is null)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "RPG0001",
                        "Initializer must be a valid UFunction that takes a single struct",
                        "{0} is not the name of a valid UFunction that takes a single struct",
                        "RPGEssentials",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    propertySymbol.Locations.First(),
                    initializerName
                )
            );
        }

        var info = new ComponentInfo(
            propertySymbol.Type,
            propertySymbol.Name,
            initializerName,
            [
                .. propertySymbol
                    .Type.GetMembers()
                    .OfType<IPropertySymbol>()
                    .Where(IsAccessibleProperty)
                    .Select(GetPropertyInfo),
            ],
            [
                .. propertySymbol
                    .Type.GetMembers()
                    .OfType<IMethodSymbol>()
                    .Where(IsAccessibleMethod)
                    .Select(GetFunctionInfo),
            ],
            isLast
        );

        _components.Add(propertySymbol.Type, info);
        return info;
    }

    private static bool IsAccessibleProperty(IPropertySymbol propertySymbol)
    {
        if (
            propertySymbol.IsStatic
            || propertySymbol.DeclaredAccessibility != Accessibility.Public
            || propertySymbol
                .GetAttributes()
                .Any(a =>
                    a.AttributeClass is not null && a.AttributeClass.IsAssignableTo<ExcludeFromExtensionsAttribute>()
                )
        )
            return false;

        return true;
    }

    private static bool IsExposedProperty(IPropertySymbol propertySymbol)
    {
        if (
            propertySymbol
                .GetAttributes()
                .Any(a => a.AttributeClass?.ToDisplayString() == GeneratorStatics.UPropertyAttribute)
        )
        {
            return true;
        }

        return GetAccessors(propertySymbol)
            .Any(m =>
                m.DeclaredAccessibility == Accessibility.Public
                && m.GetAttributes()
                    .Any(a => a.AttributeClass?.ToDisplayString() == GeneratorStatics.UFunctionAttribute)
            );
    }

    private static IEnumerable<IMethodSymbol> GetAccessors(IPropertySymbol propertySymbol)
    {
        if (propertySymbol.GetMethod is not null)
            yield return propertySymbol.GetMethod;
        if (propertySymbol.SetMethod is not null)
            yield return propertySymbol.SetMethod;
    }

    private static UPropertyInfo GetPropertyInfo(IPropertySymbol propertySymbol)
    {
        var upropertyAttribute = propertySymbol
            .GetAttributes()
            .SingleOrDefault(a => a.AttributeClass?.ToDisplayString() == GeneratorStatics.UPropertyAttribute);

        string? displayName = null;
        string? category = null;
        if (upropertyAttribute is not null)
        {
            var namedArguments = upropertyAttribute.NamedArguments.ToDictionary(a => a.Key, a => a.Value.Value);

            displayName = namedArguments.TryGetValue("DisplayName", out var displayNameValue)
                ? displayNameValue as string
                : null;
            category = namedArguments.TryGetValue("Category", out var categoryValue) ? categoryValue as string : null;
        }

        return new UPropertyInfo(
            propertySymbol,
            GetAccessorInfo(propertySymbol.GetMethod),
            GetAccessorInfo(propertySymbol.SetMethod),
            displayName,
            category
        )
        {
            IsExposed = IsExposedProperty(propertySymbol),
        };
    }

    private static AccessorInfo? GetAccessorInfo(IMethodSymbol? methodSymbol)
    {
        if (methodSymbol is not { DeclaredAccessibility: Accessibility.Public })
            return null;

        var isUFunction = methodSymbol
            .GetAttributes()
            .Any(a => a.AttributeClass?.ToDisplayString() == GeneratorStatics.UFunctionAttribute);
        return new AccessorInfo(isUFunction, GetMethodAttributes(methodSymbol));
    }

    private static ImmutableArray<AttributeInfo> GetMethodAttributes(IMethodSymbol methodSymbol)
    {
        return
        [
            .. methodSymbol
                .GetAttributes()
                .Select(a => a.ApplicationSyntaxReference?.GetSyntax())
                .OfType<AttributeSyntax>()
                .Select(a => new AttributeInfo(a)),
        ];
    }

    private static bool IsAccessibleMethod(IMethodSymbol methodSymbol)
    {
        return methodSymbol
                is {
                    DeclaredAccessibility: Accessibility.Public,
                    AssociatedSymbol: not IPropertySymbol,
                    MethodKind: MethodKind.Ordinary,
                }
            && !methodSymbol
                .GetAttributes()
                .Any(a =>
                    a.AttributeClass is not null && a.AttributeClass.IsAssignableTo<ExcludeFromExtensionsAttribute>()
                );
    }

    private static bool IsExposedMethod(IMethodSymbol methodSymbol)
    {
        return methodSymbol
            .GetAttributes()
            .Any(a => a.AttributeClass?.ToDisplayString() == GeneratorStatics.UFunctionAttribute);
    }

    private static UFunctionInfo GetFunctionInfo(IMethodSymbol methodSymbol)
    {
        var methodName = methodSymbol.IsGenericMethod
            ? $"{methodSymbol.Name}<{string.Join(", ", methodSymbol.TypeParameters)}>"
            : methodSymbol.Name;
        return new UFunctionInfo(
            methodName,
            methodSymbol,
            GetMethodAttributes(methodSymbol),
            [
                .. methodSymbol.Parameters.Select(
                    (p, i) =>
                        new UParamInfo(
                            p,
                            GetRefQualifier(p),
                            GetDefaultValue(p),
                            i == methodSymbol.Parameters.Length - 1
                        )
                ),
            ]
        )
        {
            IsExposed = IsExposedMethod(methodSymbol),
            GenericConstraint = methodSymbol.IsGenericMethod
                ? string.Join(
                    "",
                    methodSymbol.TypeParameters.Select(x => $" where {x.Name}: {GetGenericConstraint(x)}")
                )
                : string.Empty,
        };
    }

    private static void ApplyUFunctionAttribute(IMethodSymbol methodSymbol, UnrealFunctionBase unrealFunction)
    {
        const string ufunctionAttribute = "UFunctionAttribute";
        var attributes = methodSymbol.GetAttributesByName(ufunctionAttribute);
        InspectorManager.InspectSpecifiers(ufunctionAttribute, unrealFunction, attributes);
        foreach (var attribute in attributes)
        {
            if (attribute.AttributeClass is null)
                continue;

            if (attribute.AttributeClass.ToDisplayString() == GeneratorStatics.UMetaDataAttribute)
            {
                unrealFunction.AddMetaData(
                    (attribute.ConstructorArguments[0].Value as string)!,
                    (attribute.ConstructorArguments[1].Value as string)!
                );
            }
            else if (
                attribute.AttributeClass.ContainingNamespace.ToDisplayString() == "UnrealSharp.Attributes.Metadata"
            )
            {
                var argument = attribute.ConstructorArguments.FirstOrDefault().Value as string ?? string.Empty;
                unrealFunction.AddMetaData(attribute.AttributeClass.Name, argument);
            }
        }
    }

    private static string GetGenericConstraint(ITypeParameterSymbol typeParameterSymbol)
    {
        return string.Join(", ", GetGenericConstraints(typeParameterSymbol));
    }

    private static IEnumerable<string> GetGenericConstraints(ITypeParameterSymbol typeParameterSymbol)
    {
        if (typeParameterSymbol.HasReferenceTypeConstraint)
            yield return "class";
        if (typeParameterSymbol.HasValueTypeConstraint)
            yield return "struct";
        if (typeParameterSymbol.HasConstructorConstraint)
            yield return "new()";
        if (typeParameterSymbol.HasNotNullConstraint)
            yield return "unmanaged";
        foreach (var constraint in typeParameterSymbol.ConstraintTypes)
        {
            yield return constraint.ToDisplayString();
        }
    }

    private static string GetRefQualifier(IParameterSymbol typeSymbol)
    {
        return typeSymbol.RefKind switch
        {
            RefKind.None => "",
            RefKind.Ref => "ref ",
            RefKind.Out => "out ",
            RefKind.In => "in ",
            RefKind.RefReadOnlyParameter => "ref readonly ",
            _ => throw new ArgumentOutOfRangeException(),
        };
    }

    private static string? GetDefaultValue(IParameterSymbol parameterSymbol)
    {
        return parameterSymbol
            .DeclaringSyntaxReferences.Select(s => s.GetSyntax())
            .OfType<ParameterSyntax>()
            .Select(pax => pax.Default?.Value.ToString())
            .FirstOrDefault();
    }
}
