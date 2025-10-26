using Pokemon.Data;
using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using Pokemon.Editor.Serialization.Pbs.Converters;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

/// <summary>
/// Represents information about an item in the Pokemon system.
/// </summary>
/// <remarks>
/// This record contains various properties related to an item, such as its ID,
/// localized names, descriptions, pricing, usage conditions, and other metadata.
/// </remarks>
public record ItemInfo
{
    /// <summary>
    /// Gets the unique identifier for an item.
    /// This property is decorated with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsKeyAttribute"/>
    /// to signify it as the key in the serialization process.
    /// </summary>
    [PbsKey]
    public required FName ID { get; init; }

    /// <summary>
    /// Gets the row index associated with this item.
    /// This property is marked with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsIndexAttribute"/>
    /// to indicate that it serves as the index reference in the serialization process.
    /// </summary>
    [PbsIndex]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the localized display name for the item.
    /// This property uses the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsLocalizedTextAttribute"/>
    /// to fetch the localized text for the item's name from the specified namespace and key format.
    /// If no localized value is provided, the default value is "Unnamed".
    /// </summary>
    [PbsLocalizedText("PokemonItems", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Gets the plural form of the item's name.
    /// This property is decorated with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsLocalizedTextAttribute"/>
    /// to specify a namespace and key format for localized text.
    /// </summary>
    [PbsLocalizedText("PokemonItems", "{0}_DisplayNamePlural")]
    public FText NamePlural { get; init; } = "Unnamed";

    /// <summary>
    /// Gets the localized text representing the display name of a portion of the item.
    /// This property is decorated with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsLocalizedTextAttribute"/>
    /// to facilitate localization. The key format used is "{0}_PortionDisplayName", where "{0}" represents the item's identifier.
    /// </summary>
    [PbsLocalizedText("PokemonItems", "{0}_PortionDisplayName")]
    public FText PortionName { get; init; } = FText.None;

    /// <summary>
    /// Gets the localized plural display name of the portion associated with this item.
    /// This property utilizes the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsLocalizedTextAttribute"/>
    /// with a namespace of "PokemonItems" and a key format of "{0}_PortionDisplayNamePlural"
    /// to retrieve the appropriate localized text.
    /// </summary>
    [PbsLocalizedText("PokemonItems", "{0}_PortionDisplayNamePlural")]
    public FText PortionNamePlural { get; init; } = FText.None;

    /// <summary>
    /// Indicates whether the quantity of this item should be displayed.
    /// Used to determine if item's count information is relevant in certain contexts.
    /// If not explicitly set, a default behavior may be determined based on other item properties.
    /// </summary>
    public bool? ShowQuantity { get; init; }

    /// <summary>
    /// Gets the localized description of an item.
    /// This property is decorated with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsLocalizedTextAttribute"/>
    /// to specify the namespace and key format for localization.
    /// </summary>
    [PbsLocalizedText("PokemonItems", "{0}_Description")]
    public FText Description { get; init; } = "???";

    /// <summary>
    /// Gets the gameplay tag representing the pocket classification of the item.
    /// This property utilizes the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsScalarAttribute{T}"/>
    /// with the specific converter <see cref="Pokemon.Editor.Serialization.Pbs.Converters.ItemPocketConverter"/>
    /// to handle the serialization and deserialization of the pocket category.
    /// </summary>
    [PbsScalar<ItemPocketConverter>]
    public FGameplayTag Pocket { get; init; }

    /// <summary>
    /// Gets the price of the item.
    /// This property is constrained with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsRangeAttribute{T}"/>
    /// to ensure the value meets specified requirements, with a minimum value of 0.
    /// </summary>
    [PbsRange<int>(0)]
    public int Price { get; init; }

    /// <summary>
    /// Gets the sell price of an item.
    /// This property defines the monetary value for which the item can be sold,
    /// and it must be either null or a non-negative integer as specified by the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsRangeAttribute{T}"/> constraint.
    /// If not explicitly set, a default mapping logic may calculate its value based on the item's price.
    /// </summary>
    [PbsRange<int>(0)]
    public int? SellPrice { get; init; }

    /// <summary>
    /// Gets the Battle Point (BP) price of the item.
    /// The value must meet the constraints defined by the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsRangeAttribute{T}"/>,
    /// which enforces a minimum of 1.
    /// </summary>
    [PbsRange<int>(1)]
    public int BPPrice { get; init; } = 1;

    /// <summary>
    /// Gets the field behavior or usage classification for an item.
    /// This property is of type <see cref="Pokemon.Data.Model.PBS.EFieldUse"/>,
    /// which defines the possible usages such as direct application, usage on Pokémon, or as a TM/HM/TR.
    /// </summary>
    public EFieldUse FieldUse { get; init; }

    /// <summary>
    /// Gets the usage type of an item during battles.
    /// This property utilizes the <see cref="EBattleUse"/> enumeration to determine
    /// the context in which the item can be used within a battle scenario.
    /// </summary>
    public EBattleUse BattleUse { get; init; }

    /// <summary>
    /// Represents the set of gameplay tags categorizing the item's usage in battles.
    /// This property is marked with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsGameplayTagAttribute"/>
    /// to denote its association with battle-specific functionality.
    /// </summary>
    [PbsGameplayTag(IdentifierConstants.BattleUseTag, Create = true)]
    public FGameplayTagContainer BattleUsageCategories { get; init; } = new() { GameplayTags = [], ParentTags = [] };

    /// <summary>
    /// Indicates whether the item is consumable after use.
    /// This property is optionally decorated with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsNameAttribute"/>
    /// to map it to a specific serialization attribute.
    /// </summary>
    [PbsName("Consumable")]
    public bool? IsConsumable { get; init; }

    /// <summary>
    /// Gets the identifier for the specific move associated with the item.
    /// This property is intended to link items with moves they might impact or relate to.
    /// </summary>
    public FName Move { get; init; }

    /// <summary>
    /// Represents a collection of gameplay tags associated with the item.
    /// This property is decorated with the <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsNameAttribute"/>
    /// and <see cref="Pokemon.Editor.Serialization.Pbs.Attributes.PbsGameplayTagAttribute"/> to define the tag namespace,
    /// and includes options for creating new tags and defining the separator for the tag hierarchy.
    /// </summary>
    [PbsName("Flags")]
    [PbsGameplayTag("Data.Item", Create = true, Separator = "_")]
    public FGameplayTagContainer Tags { get; init; }
}
