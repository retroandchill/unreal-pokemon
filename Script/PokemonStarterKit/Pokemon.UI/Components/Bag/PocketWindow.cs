using Pokemon.Core;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
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

    [UProperty]
    [BindWidgetOptional]
    private UCommonTextBlock? PocketName { get; }

    private void UpdatePocketInfo()
    {
        PocketName?.Text =
            GetDefault<UPokemonCoreSettings>()
                .Pockets.Where(p => p.Tag == CurrentPocket)
                .Select(p => p.Name)
                .SingleOrDefault()
            ?? FText.None;
    }
}
