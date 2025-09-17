using System.Collections.Immutable;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Types;
using RPG.SourceGenerator.Attributes;
using RPG.SourceGenerator.Model;
using RPG.SourceGenerator.Properties;
using TurnBased.SourceGenerator;

namespace RPG.SourceGenerator.Generators;

[Generator]
public class RPGEntityExtensionGenerator : IIncrementalGenerator
{
    private readonly Dictionary<ITypeSymbol, ComponentInfo> _components = new(
        SymbolEqualityComparer.Default
    );

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
                    .Any(a =>
                        a.AttributeClass?.ToDisplayString() == GeneratorStatics.UStructAttribute
                    )
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

        var templateParams = new RPGEntityInfo(classSymbol)
        {
            Components =
            [
                .. componentProperties.Select(
                    (p, i) =>
                        GetComponentInfo(
                            p,
                            i == componentProperties.Length - 1,
                            factoryMethod?.Parameters[1].Type,
                            context
                        )
                ),
            ],
            FactoryMethod = factoryMethod,
            SubclassSourceSymbol = subclassSourceSymbol,
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

        var asyncMethods = templateParams
            .Components.SelectMany(c => c.Methods, (c, m) => (Component: c, Method: m))
            .Where(m =>
                m.Method.ReturnType.MetadataName
                    is "Task"
                        or "Task`1"
                        or "ValueTask"
                        or "ValueTask`1"
            )
            .Select(m => new AsyncMethodInfo(
                templateParams.Namespace,
                $"{templateParams.ClassName}Extensions",
                m.Method.Name,
                m.Method.ReturnType,
                m.Component.ComponentType,
                m.Method.Parameters,
                m.Method.Attributes
            ))
            .ToImmutableArray();

        foreach (var asyncMethod in asyncMethods)
        {
            context.AddSource(
                $"{asyncMethod.ClassName}.{asyncMethod.MethodName}.g.cs",
                handlebars.Compile(SourceTemplates.AsyncMethodCallTemplate)(asyncMethod)
            );
        }
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
                    .Any(a =>
                        a.AttributeClass?.ToDisplayString() == GeneratorStatics.UFunctionAttribute
                    )
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
                    a.AttributeClass is not null
                    && a.AttributeClass.IsAssignableTo<ExcludeFromExtensionsAttribute>()
                )
        )
            return false;

        if (
            propertySymbol
                .GetAttributes()
                .Any(a =>
                    a.AttributeClass?.ToDisplayString() == GeneratorStatics.UPropertyAttribute
                )
        )
        {
            return true;
        }

        return GetAccessors(propertySymbol)
            .Any(m =>
                m.DeclaredAccessibility == Accessibility.Public
                && m.GetAttributes()
                    .Any(a =>
                        a.AttributeClass?.ToDisplayString() == GeneratorStatics.UFunctionAttribute
                    )
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
            .SingleOrDefault(a =>
                a.AttributeClass?.ToDisplayString() == GeneratorStatics.UPropertyAttribute
            );

        string? displayName = null;
        string? category = null;
        if (upropertyAttribute is not null)
        {
            var namedArguments = upropertyAttribute.NamedArguments.ToDictionary(
                a => a.Key,
                a => a.Value.Value
            );

            displayName = namedArguments.TryGetValue("DisplayName", out var displayNameValue)
                ? displayNameValue as string
                : null;
            category = namedArguments.TryGetValue("Category", out var categoryValue)
                ? categoryValue as string
                : null;
        }

        return new UPropertyInfo(
            propertySymbol.Type,
            propertySymbol.Name,
            GetAccessorInfo(propertySymbol.GetMethod),
            GetAccessorInfo(propertySymbol.SetMethod),
            displayName,
            category
        );
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
                    AssociatedSymbol: not IPropertySymbol
                }
            && methodSymbol
                .GetAttributes()
                .Any(a =>
                    a.AttributeClass?.ToDisplayString() == GeneratorStatics.UFunctionAttribute
                )
            && !methodSymbol
                .GetAttributes()
                .Any(a =>
                    a.AttributeClass is not null
                    && a.AttributeClass.IsAssignableTo<ExcludeFromExtensionsAttribute>()
                );
    }

    private static UFunctionInfo GetFunctionInfo(IMethodSymbol methodSymbol)
    {
        return new UFunctionInfo(
            methodSymbol.Name,
            methodSymbol.ReturnType,
            GetMethodAttributes(methodSymbol),
            [
                .. methodSymbol.Parameters.Select(
                    (p, i) =>
                        new UParamInfo(
                            p.Type,
                            p.Name,
                            GetDefaultValue(p),
                            i == methodSymbol.Parameters.Length - 1
                        )
                ),
            ]
        );
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
