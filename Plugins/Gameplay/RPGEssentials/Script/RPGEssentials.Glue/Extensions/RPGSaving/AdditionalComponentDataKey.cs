using UnrealSharp.RPGCore;

namespace UnrealSharp.RPGSaving;

public partial record struct FAdditionalComponentDataKey
{
    public FAdditionalComponentDataKey(TSoftClassPtr<URPGComponent> componentClass, int index = 0)
    {
        ComponentClass = componentClass;
        Index = index;
    }

    public void Deconstruct(out TSoftClassPtr<URPGComponent> componentClass, out int index)
    {
        componentClass = ComponentClass;
        index = Index;
    }
}
