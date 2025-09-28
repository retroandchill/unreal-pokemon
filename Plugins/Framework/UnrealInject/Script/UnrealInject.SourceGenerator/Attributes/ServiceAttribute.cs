using Microsoft.Extensions.DependencyInjection;
#if UNREAL_INJECT_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace UnrealInject.SourceGenerator.Attributes;

[AttributeUsage(AttributeTargets.Class)]
#if UNREAL_INJECT_GENERATOR
[IncludeFile]
#endif
internal class ServiceAttribute(ServiceLifetime lifetime = ServiceLifetime.Singleton) : Attribute
{
    public ServiceLifetime Lifetime { get; } = lifetime;
}
