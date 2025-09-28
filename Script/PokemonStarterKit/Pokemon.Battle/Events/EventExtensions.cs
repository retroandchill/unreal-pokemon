using Microsoft.Extensions.DependencyInjection;

namespace Pokemon.Battle.Events;

public static class EventExtensions
{
    public static IServiceCollection ConfigureAbilities(
        this IServiceCollection services,
        Action<AbilityEvents> configure
    )
    {
        return services.Configure(configure);
    }
}
