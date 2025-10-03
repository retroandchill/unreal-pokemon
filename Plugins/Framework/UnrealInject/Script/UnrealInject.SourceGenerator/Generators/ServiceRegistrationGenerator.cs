using System.Collections.Immutable;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using UnrealInject.SourceGenerator.Attributes;
using UnrealInject.SourceGenerator.Model;
using UnrealInject.SourceGenerator.Properties;

namespace UnrealInject.SourceGenerator.Generators;

[Generator]
public class ServiceRegistrationGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var servicesProvider = context
            .SyntaxProvider.CreateSyntaxProvider(
                (node, _) => node is ClassDeclarationSyntax classDeclarationSyntax,
                (ctx, _) =>
                {
                    var classDeclarationSyntax = (ClassDeclarationSyntax)ctx.Node;
                    if (ctx.SemanticModel.GetDeclaredSymbol(classDeclarationSyntax) is not INamedTypeSymbol classSymbol)
                    {
                        return null;
                    }

                    return classSymbol.HasAttribute<ServiceAttribute>() ? classSymbol : null;
                }
            )
            .Where(node => node is not null)
            .Collect();

        var assemblyProvider = context.CompilationProvider.Select((compilation, _) => compilation.Assembly);

        var combinedProvider = servicesProvider.Combine(assemblyProvider);

        context.RegisterSourceOutput(
            combinedProvider,
            (ctx, data) =>
            {
                Execute(ctx, data.Right, data.Left!);
            }
        );
    }

    private static void Execute(
        SourceProductionContext context,
        IAssemblySymbol assemblySymbol,
        ImmutableArray<INamedTypeSymbol> serviceTypes
    )
    {
        if (serviceTypes.IsDefaultOrEmpty)
            return;

        var templateParameters = new
        {
            Namespace = assemblySymbol.MetadataName,
            Services = serviceTypes.Select(GetServiceInjection).ToImmutableArray(),
        };

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            "ServiceRegistrations.g.cs",
            handlebars.Compile(SourceTemplates.ServiceRegistrationsTemplate)(templateParameters)
        );
    }

    private static ServiceInjection GetServiceInjection(INamedTypeSymbol serviceType)
    {
        var serviceInfo = serviceType.GetServiceInfo();
        return new ServiceInjection(
            serviceType,
            serviceInfo.ServiceLifetime,
            [
                .. serviceType
                    .AllInterfaces.Where(i =>
                        i.SpecialType != SpecialType.System_IDisposable && i.MetadataName != "IAsyncDisposable`1"
                    )
                    .Select(i => new ServiceInterfaceInfo(i)),
            ]
        );
    }
}
