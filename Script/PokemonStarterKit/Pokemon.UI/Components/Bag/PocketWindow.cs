using Pokemon.Core;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.GameplayTags;

namespace Pokemon.UI.Components.Bag;

[UClass(ClassFlags.Abstract)]
public class UPocketWindow : UCommonUserWidget
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Inventory")]
    public FGameplayTag CurrentPocket
    {
        get;
        set
        {
            field = value;
            UpdatePocketInfo();
        }
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Display")]
    protected virtual void SetPocketName(FText pocketName)
    {
        // No native implementation
    }

    private void UpdatePocketInfo()
    {
        SetPocketName(GetDefault<UPokemonCoreSettings>().Pockets
            .Where(p => p.Tag == CurrentPocket)
            .Select(p => p.Name)
            .Single());
    }
}