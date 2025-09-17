using Pokemon.Data;
using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using Pokemon.Editor.Serialization.Pbs.Converters;
using UnrealSharp;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

public record ItemInfo
{
    [PbsKey]
    public required FName ID { get; init; }

    [PbsIndex]
    public int RowIndex { get; init; }

    [PbsLocalizedText("PokemonItems", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    [PbsLocalizedText("PokemonItems", "{0}_DisplayNamePlural")]
    public FText NamePlural { get; init; } = "Unnamed";

    [PbsLocalizedText("PokemonItems", "{0}_PortionDisplayName")]
    public FText PortionName { get; init; } = FText.None;

    [PbsLocalizedText("PokemonItems", "{0}_PortionDisplayNamePlural")]
    public FText PortionNamePlural { get; init; } = FText.None;

    public bool? ShowQuantity { get; init; }

    [PbsLocalizedText("PokemonItems", "{0}_Description")]
    public FText Description { get; init; } = "???";

    [PbsScalar<ItemPocketConverter>]
    public FGameplayTag Pocket { get; init; }

    [PbsRange<int>(0)]
    public int Price { get; init; }

    [PbsRange<int>(0)]
    public int? SellPrice { get; init; }

    [PbsRange<int>(1)]
    public int BPPrice { get; init; } = 1;

    public EFieldUse FieldUse { get; init; }

    public EBattleUse BattleUse { get; init; }

    [PbsGameplayTag(IdentifierConstants.BattleUseTag, Create = true)]
    public FGameplayTagContainer BattleUsageCategories { get; init; } =
        new() { GameplayTags = [], ParentTags = [] };

    [PbsName("Consumable")]
    public bool? IsConsumable { get; init; }

    public FName Move { get; init; }

    [PbsName("Flags")]
    [PbsGameplayTag("Data.Item", Create = true, Separator = "_")]
    public FGameplayTagContainer Tags { get; init; }
}
