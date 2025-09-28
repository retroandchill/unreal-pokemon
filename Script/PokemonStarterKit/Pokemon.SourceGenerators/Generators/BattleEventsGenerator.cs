using System.Collections.Immutable;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Pokemon.SourceGenerators.Attributes;
using Pokemon.SourceGenerators.Model;
using Pokemon.SourceGenerators.Properties;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Types;

namespace Pokemon.SourceGenerators.Generators;

[Generator]
public class BattleEventsGenerator : IIncrementalGenerator
{
    private readonly Dictionary<string, IMethodSymbol> _abilityEventTypes = new();

    private static readonly DiagnosticDescriptor InvalidEventType = new(
        "POKE001",
        "Invalid event name",
        "The event name '{0}' is invalid",
        "PokemonStarterKit",
        DiagnosticSeverity.Error,
        true
    );

    private static readonly DiagnosticDescriptor DelegateSignatureMismatch = new(
        "POKE002",
        "Mismatch between the event and method signatures",
        "The event name '{0}' expects a signature of {1}, but method {2} has a signature of {3}",
        "PokemonStarterKit",
        DiagnosticSeverity.Error,
        true
    );

    private static readonly DiagnosticDescriptor HandlerMustBeStatic = new(
        "POKE003",
        "An event handler must be static",
        "The event handler method {0} must be static",
        "PokemonStarterKit",
        DiagnosticSeverity.Error,
        true
    );

    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var metadataProvider = context.CompilationProvider.Select(
            (compilation, _) =>
                (
                    compilation.Assembly,
                    BattleEvents: compilation.GetTypeByMetadataName(
                        "Pokemon.Battle.Events.AbilityEvents"
                    )
                )
        );

        var abilityMethods = context
            .SyntaxProvider.CreateSyntaxProvider(
                (node, _) => node is MethodDeclarationSyntax,
                (ctx, _) =>
                {
                    var methodSyntax = (MethodDeclarationSyntax)ctx.Node;
                    if (
                        ctx.SemanticModel.GetDeclaredSymbol(methodSyntax)
                        is not IMethodSymbol methodSymbol
                    )
                    {
                        return null;
                    }

                    return methodSymbol.HasAttribute<BattleAbilityEventAttribute>()
                        ? methodSymbol
                        : null;
                }
            )
            .Where(m => m is not null)
            .Collect();

        var abilityContext = metadataProvider.Combine(abilityMethods);

        context.RegisterSourceOutput(
            abilityContext,
            (ctx, data) => Execute(ctx, data.Left.Assembly, data.Left.BattleEvents, data.Right!)
        );
    }

    private void Execute(
        SourceProductionContext context,
        IAssemblySymbol assemblySymbol,
        INamedTypeSymbol? abilityEventsSymbol,
        ImmutableArray<IMethodSymbol> methods
    )
    {
        if (abilityEventsSymbol is null)
        {
            return;
        }

        _abilityEventTypes.Clear();
        foreach (
            var abilityEvent in abilityEventsSymbol
                .GetMembers()
                .OfType<IPropertySymbol>()
                .Where(p =>
                    p.Type is INamedTypeSymbol { IsGenericType: true, TypeParameters.Length: 2 }
                )
        )
        {
            _abilityEventTypes[abilityEvent.Name] = (
                (INamedTypeSymbol)((INamedTypeSymbol)abilityEvent.Type).TypeArguments[1]
            ).DelegateInvokeMethod!;
        }

        var templateParams = new
        {
            Namespace = assemblySymbol.MetadataName,
            Registrations = methods
                .Select(m => GetMethodInformation(m, context))
                .ToImmutableArray(),
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            "AbilityRegistrations.g.cs",
            handlebars.Compile(SourceTemplates.AbilityRegistrationsTemplate)(templateParams)
        );
    }

    private EventMethodInfo GetMethodInformation(
        IMethodSymbol methodSymbol,
        SourceProductionContext context
    )
    {
        var (eventName, abilityID) = methodSymbol.GetBattleAbilityEventInfo();

        if (_abilityEventTypes.TryGetValue(eventName, out var abilityEvent))
        {
            ValidateEventSignature(eventName, methodSymbol, abilityEvent, context);
        }
        else
        {
            context.ReportDiagnostic(
                Diagnostic.Create(InvalidEventType, methodSymbol.Locations.First(), eventName)
            );
        }

        return new EventMethodInfo(eventName, abilityID, methodSymbol);
    }

    private static void ValidateEventSignature(
        string eventName,
        IMethodSymbol targetMethod,
        IMethodSymbol delegateMethod,
        SourceProductionContext context
    )
    {
        if (!targetMethod.IsStatic)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    HandlerMustBeStatic,
                    targetMethod.Locations.First(),
                    targetMethod.Name
                )
            );
        }

        if (
            targetMethod.ReturnType.IsAssignableTo(delegateMethod.ReturnType)
            && targetMethod.Parameters.Length == delegateMethod.Parameters.Length
            && Enumerable
                .Range(0, targetMethod.Parameters.Length)
                .All(i =>
                    delegateMethod
                        .Parameters[i]
                        .Type.IsAssignableTo(targetMethod.Parameters[i].Type)
                )
        )
        {
            return;
        }

        context.ReportDiagnostic(
            Diagnostic.Create(
                DelegateSignatureMismatch,
                targetMethod.Locations.First(),
                eventName,
                GetSignature(delegateMethod),
                targetMethod.Name,
                GetSignature(targetMethod)
            )
        );
    }

    private static string GetSignature(IMethodSymbol methodSymbol)
    {
        var returnType = methodSymbol.ReturnsVoid ? "void" : methodSymbol.ReturnType.Name;
        var parameters = string.Join(
            ", ",
            methodSymbol.Parameters.Select(p => $"{p.Type.Name} {p.Name}")
        );
        return $"{returnType} {methodSymbol.Name}({parameters})";
    }
}
