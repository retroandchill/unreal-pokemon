using InteractiveUI.Core;
using Pokemon.Core.Services.Async;
using Pokemon.Data.Model.PBS;
using Pokemon.UI.Components.Bag;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;

namespace Pokemon.UI.Screens;

[UClass(ClassFlags.Abstract)]
public class UBagScreen : UCommonActivatableWidget
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UItemSelectionWindow ItemSelectionWindow { get; }
    
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UItemInfoWindow ItemInfoWindow { get; }
    
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPocketTabWidget PocketTabWidget { get; }
    
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Widgets")]
    [BindWidget]
    protected UPocketWindow PocketWindow { get; }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Screens")]
    public static async Task<UBagScreen> PushBagScreenToStackAsync(APlayerController playerController, CancellationToken cancellationToken = default)
    {
        var screenClass = GetDefault<UPokemonUISettings>().BagScreenClass;
        return await playerController.PushContentToLayerAsync(GameplayTags.UI_Layer_GameMenu, screenClass, cancellationToken);
    }

    public override void Construct()
    {
        throw new NotImplementedException();
    }

    public void ApplyItemFilter(Func<FItemHandle, bool> filter)
    {
        throw new NotImplementedException();
    }

    public void ResetItemFilter()
    {
        throw new NotImplementedException();
    }

    public Task<FItemSlotInfo?> PromptItemSelection()
    {
        throw new NotImplementedException();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    public void ToggleItemSelection(bool canSelect)
    {
        throw new NotImplementedException();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    public void Refresh()
    {
        throw new NotImplementedException();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Items|Selection")]
    protected void SelectItem(FItemHandle itemHandle, int quantity)
    {
        throw new NotImplementedException();
    }

    [UFunction(FunctionFlags.BlueprintEvent, Category = "Items|Selection")]
    protected void ShowItemCommands()
    {
        throw new NotImplementedException();
    }
    
    // TODO: Using item events
    
}