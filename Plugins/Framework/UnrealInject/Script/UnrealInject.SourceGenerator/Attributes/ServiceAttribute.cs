#if UNREAL_INJECT_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using Microsoft.Extensions.DependencyInjection;
#endif

namespace UnrealInject.SourceGenerator.Attributes;

#if UNREAL_INJECT_GENERATOR
public enum ServiceLifetime
{
    Singleton = 0,
    Scoped = 1,
    Transient = 2,
}
#endif

[AttributeUsage(AttributeTargets.Class)]
#if UNREAL_INJECT_GENERATOR
[IncludeFile]
#endif
internal class ServiceAttribute(ServiceLifetime lifetime = ServiceLifetime.Singleton) : Attribute
{
    public ServiceLifetime Lifetime { get; } = lifetime;
}
