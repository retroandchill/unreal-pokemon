using Pokemon.Core.Entities;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UPokemonDisplayBase : UCommonUserWidget
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Content")]
    public UPokemon? Pokemon
    {
        get;
        set
        {
            if (ReferenceEquals(field, value)) return;
            
            field = value;
            Refresh();
        } 
    }
    
    [UFunction(FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent, Category = "Display")]
    public virtual void Refresh()
    {
        // No native implementation
    }
}