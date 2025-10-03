using System.Collections.Immutable;
using Microsoft.CodeAnalysis;
using Microsoft.Extensions.DependencyInjection;
using UnrealInject.SourceGenerator.Attributes;

namespace UnrealInject.SourceGenerator.Model;

internal readonly record struct ServiceInterfaceInfo(ITypeSymbol InterfaceType);

internal record ServiceInjection(
    ITypeSymbol ServiceType,
    ServiceLifetime Lifetime,
    ImmutableArray<ServiceInterfaceInfo> Interfaces
)
{
    public string Scope => Lifetime.ToString();
}
