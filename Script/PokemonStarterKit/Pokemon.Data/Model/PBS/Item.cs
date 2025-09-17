using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

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

[UStruct]
[CreateStructView]
public readonly partial struct FItem() : IGameDataEntry
{
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Identification"
    )]
    public required FName ID { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere,
        Category = "Identification"
    )]
    public int RowIndex { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public FText Name { get; init; } = "Unnamed";

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Name (Plural)",
        Category = "Display"
    )]
    public FText NamePlural { get; init; } = "Unnamed";

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    public FText PortionName
    {
        get => !field.IsNullOrWhitespace() ? field : Name;
        init;
    } = FText.None;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Portion Name (Plural)",
        Category = "Display"
    )]
    public FText PortionNamePlural
    {
        get => !field.IsNullOrWhitespace() ? field : NamePlural;
        init;
    } = FText.None;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    public bool ShowQuantity
    {
        get => field && IsImportant;
        init;
    }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    public FText Description { get; init; } = "???";

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "BagInfo"
    )]
    [field: Categories(IdentifierConstants.PocketTag)]
    [AsValue]
    public FGameplayTag Pocket { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Price"
    )]
    public bool CanSell { get; init; } = true;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Price"
    )]
    [field: EditCondition(nameof(CanSell))]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int Price { get; init; } = 0;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Price"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int SellPrice { get; init; } = 0;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "BP Price",
        Category = "Price"
    )]
    [field: EditCondition(nameof(CanSell))]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int BPPrice { get; init; } = 1;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Usage"
    )]
    public EFieldUse FieldUse { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Usage"
    )]
    public EBattleUse BattleUse { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Usage"
    )]
    [field: Categories(IdentifierConstants.BattleUseTag)]
    [field: EditCondition(
        $"{nameof(BattleUse)} != {nameof(EBattleUse)}::{nameof(EBattleUse.NoBattleUse)}"
    )]
    [field: EditConditionHides]
    public FGameplayTagContainer BattleUsageCategories { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Usage"
    )]
    public bool IsConsumable { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Usage"
    )]
    [field: EditCondition(
        $"{nameof(FieldUse)} == FieldUse::{nameof(EFieldUse.TM)} || "
            + $"{nameof(FieldUse)} == FieldUse::{nameof(EFieldUse.TR)} || "
            + $"{nameof(FieldUse)} == FieldUse::{nameof(EFieldUse.HM)}"
    )]
    [field: EditConditionHides]
    [field: UMetaData(Metadata.AllowNone)]
    [AsValue]
    public FMoveHandle Move { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Metadata"
    )]
    public FGameplayTagContainer Tags { get; init; }

    [UsedImplicitly]
    public bool IsTM => FieldUse == EFieldUse.TM;

    [UsedImplicitly]
    public bool IsTR => FieldUse == EFieldUse.TR;

    [UsedImplicitly]
    public bool IsHM => FieldUse == EFieldUse.HM;

    [UsedImplicitly]
    public bool IsMachine => IsTM || IsTR || IsHM;

    [UsedImplicitly]
    public bool IsMail => Tags.HasTag(GameplayTags.Data_Item_Mail) || IsIconMail;

    [UsedImplicitly]
    public bool IsIconMail => Tags.HasTag(GameplayTags.Data_Item_IconMail);

    [UsedImplicitly]
    public bool IsPokeBall => Tags.HasTag(GameplayTags.Data_Item_PokeBall);

    [UsedImplicitly]
    public bool IsBerry => Tags.HasTag(GameplayTags.Data_Item_Berry);

    [UsedImplicitly]
    public bool IsKeyItem => Tags.HasTag(GameplayTags.Data_Item_KeyItem);

    [UsedImplicitly]
    public bool IsEvolutionStone => Tags.HasTag(GameplayTags.Data_Item_EvolutionStone);

    [UsedImplicitly]
    public bool IsFossil => Tags.HasTag(GameplayTags.Data_Item_Fossil);

    [UsedImplicitly]
    public bool IsApricorn => Tags.HasTag(GameplayTags.Data_Item_Apricorn);

    [UsedImplicitly]
    public bool IsGem => Tags.HasTag(GameplayTags.Data_Item_TypeGem);

    [UsedImplicitly]
    public bool IsMulch => Tags.HasTag(GameplayTags.Data_Item_Mulch);

    [UsedImplicitly]
    public bool IsMegaStone => Tags.HasTag(GameplayTags.Data_Item_MegaStone);

    [UsedImplicitly]
    public bool IsScent => Tags.HasTag(GameplayTags.Data_Item_Scent);

    [UsedImplicitly]
    public bool IsImportant => IsKeyItem || IsHM || IsTM;

    [UsedImplicitly]
    public bool CanHold => !IsImportant;

    [UsedImplicitly]
    public bool ConsumedAfterUse => !IsImportant && IsConsumable;
}

[UClass]
[GameDataRepository<FItem>]
[UsedImplicitly]
public partial class UItemRepository : UAssetGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Items))]
public readonly partial record struct FItemHandle;
