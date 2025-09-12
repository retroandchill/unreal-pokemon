namespace UnrealSharp.RPGCore;

public partial class URPGComponent
{
    public T GetRequiredSiblingComponent<T>() where T : URPGComponent
    {
        return GetRequiredSiblingComponent<T>(typeof(T));
    }
    
    public T GetRequiredSiblingComponent<T>(TSubclassOf<T> componentClass) where T : URPGComponent
    {
        return GetSiblingComponent(componentClass) ?? throw new InvalidOperationException($"Entity {this} does not have component {componentClass}");
    }
}