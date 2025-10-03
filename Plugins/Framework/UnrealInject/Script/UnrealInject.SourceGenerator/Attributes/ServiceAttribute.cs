using Microsoft.Extensions.DependencyInjection;
#if UNREAL_INJECT_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace UnrealInject.SourceGenerator.Attributes;

/// <summary>
/// Specifies that a class is a service and defines the service's lifetime within a dependency injection container.
/// </summary>
/// <remarks>
/// This attribute is used to annotate classes that should be registered within a service collection.
/// It supports specifying the <see cref="ServiceLifetime"/> to control how the service instances are created and reused.
/// </remarks>
/// <param name="lifetime">The lifetime of the service within the dependency injection container.</param>
/// <example>
/// Use this attribute to mark a class as a service and configure its lifecycle in dependency injection.
/// </example>
[AttributeUsage(AttributeTargets.Class)]
#if UNREAL_INJECT_GENERATOR
[IncludeFile]
#endif
internal class ServiceAttribute(ServiceLifetime lifetime = ServiceLifetime.Singleton) : Attribute
{
    /// <summary>
    /// Gets the lifetime of the service within the dependency injection container.
    /// </summary>
    /// <value>
    /// The <see cref="ServiceLifetime"/> value representing the service's lifecycle. Valid options are:
    /// Singleton - A single instance of the service is created and shared throughout the application.
    /// Scoped - A new instance of the service is created for each scope.
    /// Transient - A new instance of the service is created each time it is requested.
    /// </value>
    /// <remarks>
    /// This property is used to define how instances of the service are managed in the dependency injection lifecycle.
    /// </remarks>
    public ServiceLifetime Lifetime { get; } = lifetime;
}
