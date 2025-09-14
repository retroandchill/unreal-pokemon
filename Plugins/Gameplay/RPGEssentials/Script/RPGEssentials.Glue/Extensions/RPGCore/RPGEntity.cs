using System.Diagnostics.CodeAnalysis;

namespace UnrealSharp.RPGCore;

public interface IRPGEntity<T>
    where T : URPGComponent
{
    T Component { get; }
}

public partial class URPGEntity
{
    public T? GetComponent<T>()
        where T : URPGComponent
    {
        if (this is IRPGEntity<T> rpgEntity)
        {
            return rpgEntity.Component;
        }

        return GetComponentInternal<T>();
    }

    public T? GetComponent<T>(TSubclassOf<T> componentClass)
        where T : URPGComponent
    {
        return GetComponentInternal(componentClass);
    }

    public bool TryGetComponent<T>([NotNullWhen(true)] out T? component)
        where T : URPGComponent
    {
        component = GetComponent<T>();
        return component is not null;
    }

    public bool TryGetComponent<T>(
        TSubclassOf<T> componentClass,
        [NotNullWhen(true)] out T? component
    )
        where T : URPGComponent
    {
        component = GetComponent(componentClass);
        return component is not null;
    }

    public T GetRequiredComponent<T>()
        where T : URPGComponent
    {
        return GetRequiredComponent<T>(typeof(T));
    }

    public T GetRequiredComponent<T>(TSubclassOf<T> componentClass)
        where T : URPGComponent
    {
        return GetComponent(componentClass)
            ?? throw new InvalidOperationException(
                $"Entity {this} does not have component {componentClass}"
            );
    }

    protected void InitializeComponents()
    {
        DefaultInitializeComponents();
    }
}
