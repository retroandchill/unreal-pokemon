namespace UnrealSharp.RPGCore;

public partial class URPGComponent
{
    public T GetRequiredSiblingComponent<T>()
        where T : URPGComponent
    {
        return GetRequiredSiblingComponent<T>(typeof(T));
    }

    public T GetRequiredSiblingComponent<T>(TSubclassOf<T> componentClass)
        where T : URPGComponent
    {
        return GetSiblingComponent(componentClass)
            ?? throw new InvalidOperationException(
                $"Entity {this} does not have component {componentClass}"
            );
    }

    public void BindInitFunction<TStruct>(Action<TStruct> initFunction)
        where TStruct : struct, MarshalledStruct<TStruct>
    {
        if (initFunction.Target is not URPGComponent component || !ReferenceEquals(component, this))
        {
            throw new InvalidOperationException("Function must be bound to this component");
        }

        var result = BindInitFunctionInternal(initFunction.Method.Name);
        if (result is null)
        {
            throw new InvalidOperationException("Failed to bind init function");
        }
    }
}
