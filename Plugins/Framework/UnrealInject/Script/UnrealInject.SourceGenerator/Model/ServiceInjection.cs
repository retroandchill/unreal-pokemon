using Microsoft.CodeAnalysis;
using Microsoft.Extensions.DependencyInjection;
using UnrealInject.SourceGenerator.Attributes;

namespace UnrealInject.SourceGenerator.Model;

internal record ServiceInjection(ITypeSymbol ServiceType, ServiceLifetime Lifetime)
{
    public string Scope => Lifetime.ToString();

    public ITypeSymbol? ImplementationType { get; init; }

    public bool HasImplementation => ImplementationType is not null;
}
