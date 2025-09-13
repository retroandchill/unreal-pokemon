namespace UnrealSharp.RPGCore;

public partial class URPGEntity
{
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
