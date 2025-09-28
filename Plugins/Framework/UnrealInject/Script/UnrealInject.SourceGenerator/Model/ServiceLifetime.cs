namespace Microsoft.Extensions.DependencyInjection;

internal enum ServiceLifetime
{
    Singleton = 0,
    Scoped = 1,
    Transient = 2,
}
