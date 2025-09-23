using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using RPG.SourceGenerator.Attributes;
using RPG.SourceGenerator.Model;
using RPG.SourceGenerator.Properties;
using TurnBased.SourceGenerator;

namespace RPG.SourceGenerator.Generators;

[Generator]
public class ComponentSaverGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var classesProvider = context
            .SyntaxProvider.CreateSyntaxProvider(
                predicate: (s, _) => s is ClassDeclarationSyntax,
                transform: (ctx, _) =>
                {
                    var classNode = (ClassDeclarationSyntax)ctx.Node;
                    if (
                        ctx.SemanticModel.GetDeclaredSymbol(classNode)
                        is not INamedTypeSymbol classSymbol
                    )
                    {
                        return null;
                    }

                    return classSymbol.HasAttribute(typeof(ComponentSaverAttribute<,>))
                        ? classSymbol
                        : null;
                }
            )
            .Where(m => m is not null);

        context.RegisterSourceOutput(classesProvider, Execute!);
    }

    private static void Execute(SourceProductionContext context, INamedTypeSymbol classSymbol)
    {
        if (classSymbol.BaseType?.ToDisplayString() != GeneratorStatics.ComponentSaver)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "RPG1001",
                        "Must inherit directly from UComponentSaver",
                        "{0} must inherit directly from UComponentSaver.",
                        "RPGEssentials",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    classSymbol.Locations.First(),
                    classSymbol.Name
                )
            );
            return;
        }

        var componentSaverInfo = classSymbol.GetComponentSaverInfo();

        IMethodSymbol? saveMethod = null;
        IMethodSymbol? loadMethod = null;
        foreach (var method in classSymbol.GetMembers().OfType<IMethodSymbol>())
        {
            if (
                method.ReturnType.Equals(
                    componentSaverInfo.ModelStruct,
                    SymbolEqualityComparer.Default
                )
                && method.Parameters.Length == 1
                && method
                    .Parameters[0]
                    .Type.Equals(componentSaverInfo.ComponentClass, SymbolEqualityComparer.Default)
            )
            {
                saveMethod ??= method;
                continue;
            }

            if (
                method is { ReturnsVoid: true, Parameters.Length: 2 }
                && (
                    (
                        method
                            .Parameters[0]
                            .Type.Equals(
                                componentSaverInfo.ComponentClass,
                                SymbolEqualityComparer.Default
                            )
                        && method
                            .Parameters[1]
                            .Type.Equals(
                                componentSaverInfo.ModelStruct,
                                SymbolEqualityComparer.Default
                            )
                    )
                    || (
                        method
                            .Parameters[0]
                            .Type.Equals(
                                componentSaverInfo.ModelStruct,
                                SymbolEqualityComparer.Default
                            )
                        && method
                            .Parameters[1]
                            .Type.Equals(
                                componentSaverInfo.ComponentClass,
                                SymbolEqualityComparer.Default
                            )
                    )
                )
            )
            {
                loadMethod ??= method;
            }
        }

        if (saveMethod is null)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "RPG1002",
                        "Must provide a save method",
                        "{0} must provide a save method",
                        "RPGEssentials",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    classSymbol.Locations.First(),
                    classSymbol.Name
                )
            );
        }

        if (loadMethod is null)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "RPG1003",
                        "Must provide a load method",
                        "{0} must provide a load method",
                        "RPGEssentials",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    classSymbol.Locations.First(),
                    classSymbol.Name
                )
            );
        }

        if (saveMethod is null || loadMethod is null)
        {
            return;
        }

        var templateParams = new
        {
            Namespace = classSymbol.ContainingNamespace.ToDisplayString(),
            ClassName = classSymbol.Name,
            ComponentType = componentSaverInfo.ComponentClass.ToDisplayString(
                SymbolDisplayFormat.FullyQualifiedFormat
            ),
            ModelType = componentSaverInfo.ModelStruct.ToDisplayString(
                SymbolDisplayFormat.FullyQualifiedFormat
            ),
            LoggerClass = componentSaverInfo.LoggerClass?.ToDisplayString(
                SymbolDisplayFormat.FullyQualifiedFormat
            ),
            WithLogging = componentSaverInfo.LoggerClass is not null,
            SaveMethod = saveMethod.Name,
            LoadMethod = loadMethod.Name,
            ModelFirst = loadMethod
                .Parameters[0]
                .Type.Equals(componentSaverInfo.ModelStruct, SymbolEqualityComparer.Default),
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{templateParams.ClassName[1..]}.g.cs",
            handlebars.Compile(SourceTemplates.ComponentSaverTemplate)(templateParams)
        );
    }
}
