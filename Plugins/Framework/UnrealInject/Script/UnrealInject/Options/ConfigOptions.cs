namespace UnrealInject.Options;

public interface IConfigOptions<out T>
{
    T Value { get; }
}

public sealed class ConfigOptions<T> : IConfigOptions<T>
{
    public T Value { get; }

    public ConfigOptions(
        T currentValue,
        IEnumerable<IOptionsConfiguration<T>> optionsConfigurations
    )
    {
        Value = currentValue;
        foreach (var optionsConfiguration in optionsConfigurations)
        {
            optionsConfiguration.Configure(Value);
        }
    }
}
