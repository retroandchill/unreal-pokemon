using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

/// <summary>
/// Represents the various ways an item can be used outside of battle in the Pokémon field.
/// </summary>
[UEnum]
public enum EFieldUse : byte
{
    /// <summary>
    /// Not usable in the field
    /// </summary>
    NoFieldUse = 0,

    /// <summary>
    /// Used on a Pokémon
    /// </summary>
    OnPokemon = 1,

    /// <summary>
    /// Used directly from the bag
    /// </summary>
    Direct = 2,

    /// <summary>
    /// Teaches a Pokémon a move (resuable on newer mechanics)
    /// </summary>
    TM = 3,

    /// <summary>
    /// Teaches a Pokémon a move (reusable)
    /// </summary>
    HM = 4,

    /// <summary>
    /// Teaches a Pokémon a move (single-use)
    /// </summary>
    TR = 5,
}

/// <summary>
/// Represents the various ways an item can be used during a battle in the Pokémon game.
/// </summary>
[UEnum]
public enum EBattleUse : byte
{
    /// <summary>
    /// Not usable in battle
    /// </summary>
    NoBattleUse = 0,

    /// <summary>
    /// Usable on a Pokémon in the party
    /// </summary>
    OnPokemon = 1,

    /// <summary>
    /// Usable on a Pokémon in the party and requiring a move to be selected
    /// </summary>
    OnMove = 2,

    /// <summary>
    /// Usable on the active Pokémon in battle
    /// </summary>
    OnBattler = 3,

    /// <summary>
    /// Used on an opponent in battle
    /// </summary>
    OnFoe = 4,

    /// <summary>
    /// Used directly with no target selection
    /// </summary>
    Direct = 5,
}

/// <summary>
/// Represents a Pokémon item with various properties defining its identification, display,
/// price, usage, and other categorizations.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FItem() : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Identification")]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere, Category = "Identification")]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the name of the item, initialized to "Unnamed" by default.
    /// Ensures non-whitespace value is returned; otherwise, falls back to default name.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Represents the pluralized display name of the item. Used for indicating multiple quantities or instances of the item.
    /// </summary>
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Name (Plural)",
        Category = "Display"
    )]
    public FText NamePlural { get; init; } = "Unnamed";

    /// <summary>
    /// Gets or sets the portion-specific name of the item.
    /// If not explicitly set or contains only whitespace, defaults to the item's main display <see cref="Name"/>.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    public FText PortionName
    {
        get => !field.IsNullOrWhitespace() ? field : Name;
        init;
    } = FText.None;

    /// <summary>
    /// Gets or initializes the plural name for a portion of the item.
    /// If not explicitly set, defaults to the plural name of the item.
    /// </summary>
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Portion Name (Plural)",
        Category = "Display"
    )]
    public FText PortionNamePlural
    {
        get => !field.IsNullOrWhitespace() ? field : NamePlural;
        init;
    } = FText.None;

    /// <summary>
    /// Indicates whether the quantity of the item should be displayed.
    /// The value is determined based on additional criteria, such as
    /// the item's importance (e.g., whether it is a key item, HM, or TM).
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    public bool ShowQuantity
    {
        get => field && !IsImportant;
        init;
    }

    /// <summary>
    /// Represents the description of the item, providing details or information about it.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    public FText Description { get; init; } = "???";

    /// <summary>
    /// Represents the pocket category associated with the item, identified by a gameplay tag.
    /// This property defines the classification or organizational grouping of the item within the bag inventory.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "BagInfo")]
    [field: Categories(IdentifierConstants.PocketTag)]
    [AsValue]
    public FGameplayTag Pocket { get; init; }

    /// <summary>
    /// Indicates whether the item is sellable or not.
    /// This property is primarily used to determine if the item can be sold in the game's economy.
    /// If true, the item can be sold, and its price is configured via the Price or BPPrice properties.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Price")]
    public bool CanSell { get; init; } = true;

    /// <summary>
    /// Gets the price of the item. This property represents the monetary value
    /// of the item, which can be assigned and retrieved as an integer. The price
    /// must be a positive value, with a minimum of 1, and it is only applicable
    /// if the item can be sold, as indicated by the `CanSell` property.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Price")]
    [field: EditCondition(nameof(CanSell))]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int Price { get; init; } = 0;

    /// <summary>
    /// Gets the price at which the item can be sold.
    /// This property defines the monetary value of the item when sold, typically expressed as an integer.
    /// The value must be greater than or equal to 1.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Price")]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int SellPrice { get; init; } = 0;

    /// <summary>
    /// Represents the price of the item in Battle Points (BP). This value is editable in the Unreal Editor and is constrained to have a minimum value of 1.
    /// It is only applicable when the item is marked as sellable.
    /// </summary>
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "BP Price",
        Category = "Price"
    )]
    [field: EditCondition(nameof(CanSell))]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int BPPrice { get; init; } = 1;

    /// <summary>
    /// Represents the usage category of a field for an item.
    /// </summary>
    /// <remarks>
    /// The <see cref="FieldUse"/> property defines the context in which an item can be used,
    /// categorized based on different usage types such as on Pokémon, as a TM/HM, or direct use.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Usage")]
    public EFieldUse FieldUse { get; init; }

    /// <summary>
    /// Represents the specified usage scenario of an item during a battle.
    /// Determines the context in which an item can be applied within battle conditions.
    /// The categorization is based on predefined enumeration values defining possible in-battle use cases.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Usage")]
    public EBattleUse BattleUse { get; init; }

    /// <summary>
    /// Represents the category or categories under which this item can be used during battles.
    /// These categories are defined as gameplay tags and are typically used to determine how and when the item can function within combat scenarios.
    /// This property is editable under conditions where the item's battle use is not explicitly restricted or set to "No Battle Use."
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Usage")]
    [field: Categories(IdentifierConstants.BattleUseTag)]
    [field: EditCondition($"{nameof(BattleUse)} != {nameof(EBattleUse)}::{nameof(EBattleUse.NoBattleUse)}")]
    [field: EditConditionHides]
    public FGameplayTagContainer BattleUsageCategories { get; init; }

    /// <summary>
    /// Indicates whether the item can be consumed during use. If set to true, the item is considered consumable.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Usage")]
    public bool IsConsumable { get; init; }

    /// <summary>
    /// Gets or initializes the associated move handle, representing a move
    /// associated with this field item when the field use is categorized as
    /// TM, TR, or HM.
    /// </summary>
    /// <remarks>
    /// The value is restricted based on the <see cref="EFieldUse"/> property:
    /// it is only relevant for items where the <see cref="FieldUse"/> is set
    /// to <see cref="EFieldUse.TM"/>, <see cref="EFieldUse.TR"/>, or
    /// <see cref="EFieldUse.HM"/>. The property also supports an option to
    /// allow unassigned values as defined by metadata.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Usage")]
    [field: EditCondition(
        $"{nameof(FieldUse)} == FieldUse::{nameof(EFieldUse.TM)} || "
            + $"{nameof(FieldUse)} == FieldUse::{nameof(EFieldUse.TR)} || "
            + $"{nameof(FieldUse)} == FieldUse::{nameof(EFieldUse.HM)}"
    )]
    [field: EditConditionHides]
    [field: UMetaData(Metadata.AllowNone)]
    [AsValue]
    public FMoveHandle Move { get; init; }

    /// <summary>
    /// Represents a collection of gameplay tags associated with the item.
    /// These tags provide metadata that can be used to categorize or filter the item.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Metadata")]
    public FGameplayTagContainer Tags { get; init; }

    /// <summary>
    /// Indicates whether the item is classified as a Technical Machine (TM).
    /// </summary>
    public bool IsTM => FieldUse == EFieldUse.TM;

    /// <summary>
    /// Indicates whether the item is classified as a Technical Record (TR) based on its field use.
    /// </summary>
    public bool IsTR => FieldUse == EFieldUse.TR;

    /// <summary>
    /// Determines if the item is categorized as an HM (Hidden Machine).
    /// </summary>
    public bool IsHM => FieldUse == EFieldUse.HM;

    /// <summary>
    /// Indicates whether the item is a machine, such as a TM, TR, or HM.
    /// </summary>
    public bool IsMachine => IsTM || IsTR || IsHM;

    /// <summary>
    /// Indicates whether the item is categorized as mail.
    /// The property determines this status based on specific tags in the item's
    /// `Tags` field, such as `Data_Item_Mail`, or if it is designated as `IsIconMail`.
    /// </summary>
    public bool IsMail => Tags.HasTag(GameplayTags.Data_Item_Mail) || IsIconMail;

    /// <summary>
    /// Indicates whether the item is categorized as an "Icon Mail."
    /// Evaluates to true if the item's tags contain the `Data_Item_IconMail` gameplay tag.
    /// </summary>
    public bool IsIconMail => Tags.HasTag(GameplayTags.Data_Item_IconMail);

    /// <summary>
    /// Determines whether the item is categorized as a Poké Ball.
    /// This property checks if the item's tags contain the "Data.Item.PokeBall" gameplay tag.
    /// </summary>
    public bool IsPokeBall => Tags.HasTag(GameplayTags.Data_Item_PokeBall);

    /// <summary>
    /// Indicates whether the item is classified as a berry.
    /// This determination is based on the presence of the `Data_Item_Berry` tag in the item's tag container.
    /// </summary>
    public bool IsBerry => Tags.HasTag(GameplayTags.Data_Item_Berry);

    /// <summary>
    /// Indicates whether the item is classified as a key item.
    /// A key item is a special type of item often integral to gameplay progression or story.
    /// This property is determined by checking for the presence of the specific key item tag
    /// in the item's associated tags.
    /// </summary>
    public bool IsKeyItem => Tags.HasTag(GameplayTags.Data_Item_KeyItem);

    /// <summary>
    /// Indicates whether the item is classified as an evolution stone.
    /// This property is determined by checking if the item's tags include the `Data_Item_EvolutionStone` tag.
    /// </summary>
    public bool IsEvolutionStone => Tags.HasTag(GameplayTags.Data_Item_EvolutionStone);

    /// <summary>
    /// Indicates whether the item is categorized as a fossil.
    /// This property evaluates to true if the item's tags contain the `Data.Item.Fossil` tag.
    /// </summary>
    public bool IsFossil => Tags.HasTag(GameplayTags.Data_Item_Fossil);

    /// <summary>
    /// Indicates whether the item is classified as an Apricorn.
    /// This determination is based on whether the associated `Tags` contain the `Data_Item_Apricorn` gameplay tag.
    /// </summary>
    public bool IsApricorn => Tags.HasTag(GameplayTags.Data_Item_Apricorn);

    /// <summary>
    /// Indicates whether the item is classified as a gem.
    /// This property checks if the item's tags include the tag "Data.Item.TypeGem".
    /// </summary>
    public bool IsGem => Tags.HasTag(GameplayTags.Data_Item_TypeGem);

    /// <summary>
    /// Gets a value indicating whether the item is categorized as mulch.
    /// This is determined by checking if the item's tags contain the `Data_Item_Mulch` gameplay tag.
    /// </summary>
    public bool IsMulch => Tags.HasTag(GameplayTags.Data_Item_Mulch);

    /// <summary>
    /// Indicates whether the item is classified as a Mega Stone.
    /// This property evaluates to true if the item's tags include the `Data_Item_MegaStone` tag.
    /// </summary>
    public bool IsMegaStone => Tags.HasTag(GameplayTags.Data_Item_MegaStone);

    /// <summary>
    /// Indicates whether the item is classified as a "scent" by checking if it has the associated gameplay tag `Data.Item.Scent`.
    /// </summary>
    public bool IsScent => Tags.HasTag(GameplayTags.Data_Item_Scent);

    /// <summary>
    /// Indicates whether the item is considered important.
    /// An item is deemed important if it is classified as a Key Item, HM, or TM.
    /// </summary>
    public bool IsImportant => IsKeyItem || IsHM || IsTM;

    /// <summary>
    /// Indicates whether the item can be held by a Pokémon.
    /// Returns true if the item is not important (e.g., not a key item, HM, or TM); otherwise, false.
    /// </summary>
    public bool CanHold => !IsImportant;

    /// <summary>
    /// Indicates whether the item is consumed after use.
    /// This property returns true if the item is consumable and not considered important.
    /// Important items, such as key items, HMs, or TMs, are not consumed after use.
    /// </summary>
    public bool ConsumedAfterUse => !IsImportant && IsConsumable;
}

/// <summary>
/// Serves as a repository for managing and accessing item data within the game.
/// This class acts as an intermediary for retrieving, updating, and storing data
/// related to items defined in the game world.
/// </summary>
[GameDataRepository<FItem>]
public partial class UItemRepository : UAssetGameDataRepository;

/// <summary>
/// Represents a handle for an item in the Pokémon game data. Includes functionality for referencing items
/// stored within the game's item repository and is utilized in various game systems to identify specific items.
/// </summary>
[DataHandle(typeof(GameData), nameof(GameData.Items))]
public readonly partial record struct FItemHandle;
