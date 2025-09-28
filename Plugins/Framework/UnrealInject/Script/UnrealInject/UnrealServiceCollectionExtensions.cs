using System.Diagnostics.CodeAnalysis;
using Microsoft.Extensions.DependencyInjection;

namespace UnrealInject;

public static class UnrealServiceCollectionExtensions
{
    extension(UnrealServiceCollection services)
    {
        public Guid AddSingleton(
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                Type serviceType
        )
        {
            return services.Add(serviceType, ServiceLifetime.Singleton);
        }

        public Guid AddSingleton(
            Type serviceType,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                Type implementationType
        )
        {
            return services.Add(serviceType, implementationType, ServiceLifetime.Singleton);
        }

        public Guid AddSingleton<
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TService
        >()
        {
            return services.AddSingleton(typeof(TService));
        }

        public Guid AddSingleton<
            TService,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                TImplementation
        >()
            where TImplementation : TService
        {
            return services.AddSingleton(typeof(TService), typeof(TImplementation));
        }

        public Guid AddSingleton<TService>(Func<IServiceProvider, TService> implementationFactory)
            where TService : class
        {
            return services.Add(typeof(TService), implementationFactory, ServiceLifetime.Singleton);
        }

        public Guid AddSingleton<TService>(TService implementation)
            where TService : notnull
        {
            return services.Add(typeof(TService), implementation);
        }

        public Guid AddScoped(
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                Type serviceType
        )
        {
            return services.Add(serviceType, ServiceLifetime.Scoped);
        }

        public Guid AddScoped(
            Type serviceType,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                Type implementationType
        )
        {
            return services.Add(serviceType, implementationType, ServiceLifetime.Scoped);
        }

        public Guid AddScoped<
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TService
        >()
        {
            return services.AddScoped(typeof(TService));
        }

        public Guid AddScoped<
            TService,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                TImplementation
        >()
            where TImplementation : TService
        {
            return services.AddScoped(typeof(TService), typeof(TImplementation));
        }

        public Guid AddScoped<TService>(Func<IServiceProvider, TService> implementationFactory)
            where TService : class
        {
            return services.Add(typeof(TService), implementationFactory, ServiceLifetime.Scoped);
        }

        public Guid AddTransient(
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                Type serviceType
        )
        {
            return services.Add(serviceType, ServiceLifetime.Transient);
        }

        public Guid AddTransient(
            Type serviceType,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                Type implementationType
        )
        {
            return services.Add(serviceType, implementationType, ServiceLifetime.Transient);
        }

        public Guid AddTransient<
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)] TService
        >()
        {
            return services.AddTransient(typeof(TService));
        }

        public Guid AddTransient<
            TService,
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                TImplementation
        >()
            where TImplementation : TService
        {
            return services.AddTransient(typeof(TService), typeof(TImplementation));
        }

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
            [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.PublicConstructors)]
                Type implementationType,
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
