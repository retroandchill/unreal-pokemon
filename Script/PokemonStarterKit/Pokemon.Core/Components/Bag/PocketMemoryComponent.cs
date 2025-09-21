using Pokemon.Core.Entities;
using Pokemon.Data;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Bag;

[UClass]
[UMetaData("HideCategories", "Inventory")]
public class UPocketMemoryComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UPocketsComponent PocketsComponent { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Inventory")]
    public FGameplayTag LastViewedPocket { get; set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public TMap<FGameplayTag, int> LastViewedItems { get; }

    protected override void PreInitialize()
    {
        PocketsComponent = GetRequiredSiblingComponent<UPocketsComponent>();
    }

    [UFunction]
    [ExcludeFromExtensions]
    public virtual void Initialize(FBagInitParams initParams)
    {
        ResetLastSelections();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public void ResetLastSelections()
    {
        foreach (var (tag, i) in PocketsComponent.AllPockets.Select((x, i) => (x.Tag, i)))
        {
            if (i == 0)
            {
                LastViewedPocket = tag;
            }

            LastViewedItems.Add(tag, 0);
        }
    }

    [UFunction(FunctionFlags.BlueprintPure, Category = "Inventory")]
    public int GetLastViewedIndex(
        [Categories(IdentifierConstants.PocketTag)] FGameplayTag pocketTag
    )
    {
        return LastViewedItems.TryGetValue(pocketTag, out var index)
            ? index
            : throw new ArgumentException($"Invalid pocket tag: {pocketTag}");
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Inventory")]
    public void SetLastViewedIndex(
        [Categories(IdentifierConstants.PocketTag)] FGameplayTag pocketTag,
        int index
    )
    {
        if (!LastViewedItems.ContainsKey(pocketTag))
        {
            throw new ArgumentException($"Invalid pocket tag: {pocketTag}");
        }

        LastViewedItems[pocketTag] = index;
    }
}
