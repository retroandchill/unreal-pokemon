using System.Diagnostics.CodeAnalysis;
using Microsoft.Extensions.DependencyInjection;

namespace UnrealInject;

/// <summary>
/// Provides extension methods for configuring Unreal-related services in a ServiceCollection.
/// </summary>
public static class UnrealServiceCollectionExtensions
{
    /// <summary>
    /// Extension methods for configuring Unreal-related services in a ServiceCollection.
    /// </summary>
    extension(UnrealServiceCollection services)
    {
        /// <summary>
        /// Registers a singleton service of the specified type in the service collection.
        /// </summary>
        /// <param name="serviceType">The type of the service to add as a singleton.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddSingleton(
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] Type serviceType
        )
        {
            return services.Add(serviceType, ServiceLifetime.Singleton);
        }

        /// <summary>
        /// Registers a singleton service of the specified type in the service collection.
        /// </summary>
        /// <param name="serviceType">The type of the service to add as a singleton.</param>
        /// <param name="implementationType">The type that implements the service.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddSingleton(
            Type serviceType,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] Type implementationType
        )
        {
            return services.Add(serviceType, implementationType, ServiceLifetime.Singleton);
        }

        /// <summary>
        /// Registers a singleton service of the specified type in the service collection.
        /// </summary>
        /// <typeparam name="TService">The type of the service to add as a singleton.</typeparam>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddSingleton<
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TService
        >()
        {
            return services.AddSingleton(typeof(TService));
        }

        /// <summary>
        /// Registers a singleton service of the specified type in the service collection.
        /// </summary>
        /// <typeparam name="TService">The type of the service to add as a singleton.</typeparam>
        /// <typeparam name="TImplementation">The type of the service to add as a singleton.</typeparam>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddSingleton<
            TService,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TImplementation
        >()
            where TImplementation : TService
        {
            return services.AddSingleton(typeof(TService), typeof(TImplementation));
        }

        /// <summary>
        /// Registers a singleton service of the specified type in the service collection.
        /// </summary>
        /// <param name="implementationFactory">The factory method used to produce the singleton.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddSingleton<TService>(Func<IServiceProvider, TService> implementationFactory)
            where TService : class
        {
            return services.Add(typeof(TService), implementationFactory, ServiceLifetime.Singleton);
        }

        /// <summary>
        /// Registers a singleton service of the specified type in the service collection.
        /// </summary>
        /// <param name="implementation">The implementation instance.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddSingleton<TService>(TService implementation)
            where TService : notnull
        {
            return services.Add(typeof(TService), implementation);
        }

        /// <summary>
        /// Registers a scoped service of the specified type in the service collection.
        /// </summary>
        /// <param name="serviceType">The type of the service to add as scoped.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddScoped(
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] Type serviceType
        )
        {
            return services.Add(serviceType, ServiceLifetime.Scoped);
        }

        /// <summary>
        /// Registers a scoped service of the specified type in the service collection.
        /// </summary>
        /// <param name="serviceType">The type of the service to add as scoped.</param>
        /// <param name="implementationType">The type of the implementation to use for the service.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddScoped(
            Type serviceType,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] Type implementationType
        )
        {
            return services.Add(serviceType, implementationType, ServiceLifetime.Scoped);
        }

        /// <summary>
        /// Registers a scoped service of the specified type in the service collection.
        /// </summary>
        /// <typeparam name="TService">The type of the service to add as scoped.</typeparam>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddScoped<
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TService
        >()
        {
            return services.AddScoped(typeof(TService));
        }

        /// <summary>
        /// Registers a scoped service of the specified type in the service collection.
        /// </summary>
        /// <typeparam name="TService">The type of the service to add as scoped.</typeparam>
        /// <typeparam name="TImplementation">The type of the service to add as scoped.</typeparam>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddScoped<
            TService,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TImplementation
        >()
            where TImplementation : TService
        {
            return services.AddScoped(typeof(TService), typeof(TImplementation));
        }

        /// <summary>
        /// Registers a scoped service of the specified type in the service collection.
        /// </summary>
        /// <param name="implementationFactory">The factory method used to create the service.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddScoped<TService>(Func<IServiceProvider, TService> implementationFactory)
            where TService : class
        {
            return services.Add(typeof(TService), implementationFactory, ServiceLifetime.Scoped);
        }

        /// <summary>
        /// Registers a transient service of the specified type in the service collection.
        /// </summary>
        /// <param name="serviceType">The type of the service to add as transient.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddTransient(
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] Type serviceType
        )
        {
            return services.Add(serviceType, ServiceLifetime.Transient);
        }

        /// <summary>
        /// Registers a transient service of the specified type in the service collection.
        /// </summary>
        /// <param name="serviceType">The type of the service to add as transient.</param>
        /// <param name="implementationType">The type of the implementation to use for the service.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddTransient(
            Type serviceType,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] Type implementationType
        )
        {
            return services.Add(serviceType, implementationType, ServiceLifetime.Transient);
        }

        /// <summary>
        /// Registers a transient service of the specified type in the service collection.
        /// Transient services are created each time they are requested.
        /// </summary>
        /// <typeparam name="TService">The type of the service to add as transient.</typeparam>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddTransient<
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TService
        >()
        {
            return services.AddTransient(typeof(TService));
        }

        /// <summary>
        /// Registers a transient service of the specified type in the service collection.
        /// </summary>
        /// <typeparam name="TService">The service collection to which the transient service is added.</typeparam>
        /// <typeparam name="TImplementation">The type of the service to add as a transient.</typeparam>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddTransient<
            TService,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TImplementation
        >()
            where TImplementation : TService
        {
            return services.AddTransient(typeof(TService), typeof(TImplementation));
        }

        /// <summary>
        /// Registers a transient service of the specified type in the service collection.
        /// </summary>
        /// <param name="implementationFactory">The factory method used to create the service.</param>
        /// <returns>A unique identifier (GUID) associated with the service registration.</returns>
        public Guid AddTransient<TService>(Func<IServiceProvider, TService> implementationFactory)
            where TService : class
        {
            return services.Add(typeof(TService), implementationFactory, ServiceLifetime.Scoped);
        }

        private Guid Add(Type serviceType, ServiceLifetime lifetime)
        {
            var descriptor = new ServiceDescriptor(serviceType, lifetime);
            return services.Add(descriptor);
        }

        private Guid Add(
            Type serviceType,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] Type implementationType,
            ServiceLifetime lifetime
        )
        {
            var descriptor = new ServiceDescriptor(serviceType, implementationType, lifetime);
            return services.Add(descriptor);
        }

        private Guid Add(Type serviceType, object implementationInstance)
        {
            var descriptor = new ServiceDescriptor(serviceType, implementationInstance);
            return services.Add(descriptor);
        }

        private Guid Add(
            Type serviceType,
            Func<IServiceProvider, object> implementationFactory,
            ServiceLifetime lifetime
        )
        {
            var descriptor = new ServiceDescriptor(serviceType, implementationFactory, lifetime);
            return services.Add(descriptor);
        }
    }
}
