using JetBrains.Annotations;
using LanguageExt;
using LanguageExt.UnsafeValueAccess;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

[UStruct]
public readonly partial record struct FWildHoldItems(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] IReadOnlyList<FItemHandle> Common,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] IReadOnlyList<FItemHandle> Uncommon,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] IReadOnlyList<FItemHandle> Rare
);

[UClass]
[UMetaData("HideCategories", "HoldItem")]
[UsedImplicitly]
public class UHoldItemComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UIdentityComponent IdentityComponent { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "HoldItem")]
    public Option<FItemHandle> Item
    {
        get;
        set
        {
            if (value.IsSome && !value.ValueUnsafe().IsValid)
                return;

            field = value;
        }
    }

    public bool HasItem
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Has Item", Category = "HoldItem")]
        get => Item.IsSome;
    }

    public FWildHoldItems WildHoldItems
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Wild Hold Items",
            Category = "HoldItem"
        )]
        get
        {
            var species = IdentityComponent.Species.Entry;
            return new FWildHoldItems(
                species.WildHoldItemCommon.ToArray(),
                species.WildHoldItemUncommon.ToArray(),
                species.WildHoldItemRare.ToArray()
            );
        }
    }

    protected override void PreInitialize()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
    }

    [UFunction(
        FunctionFlags.BlueprintPure,
        DisplayName = "Has Specific Item",
        Category = "HoldItem"
    )]
    public bool HasSpecificItem(FItemHandle item)
    {
        return Item.Match(x => x == item, () => false);
    }
}
