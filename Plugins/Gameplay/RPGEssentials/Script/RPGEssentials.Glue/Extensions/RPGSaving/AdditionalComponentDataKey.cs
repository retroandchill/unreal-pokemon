using UnrealSharp.RPGCore;

namespace UnrealSharp.RPGSaving;

public partial record struct FAdditionalComponentDataKey
{
    /// <summary>
    /// Represents a key used to uniquely identify additional component data
    /// within a specific RPG save system context, combining the component class
    /// with an optional index for multi-instance support.
    /// </summary>
    public FAdditionalComponentDataKey(TSoftClassPtr<URPGComponent> componentClass, int index = 0)
    {
        ComponentClass = componentClass;
        Index = index;
    }

    /// <summary>
    /// Deconstructs the FAdditionalComponentDataKey into its component class and index.
    /// Unpacks the values into the specified out parameters.
    /// </summary>
    /// <param name="componentClass">
    /// The component class associated with the FAdditionalComponentDataKey.
    /// </param>
    /// <param name="index">
    /// The index associated with the FAdditionalComponentDataKey.
    /// </param>
    public void Deconstruct(out TSoftClassPtr<URPGComponent> componentClass, out int index)
    {
        componentClass = ComponentClass;
        index = Index;
    }
}
