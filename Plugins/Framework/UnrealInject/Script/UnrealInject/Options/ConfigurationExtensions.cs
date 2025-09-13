using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.DependencyInjection.Extensions;

namespace UnrealInject.Options;

public static class ConfigurationExtensions
{
    public static IServiceCollection AddConfigOptions(this IServiceCollection services)
    {
        services.TryAddSingleton(typeof(IConfigOptions<>), typeof(ConfigOptions<>));
        return services;
    }
}
