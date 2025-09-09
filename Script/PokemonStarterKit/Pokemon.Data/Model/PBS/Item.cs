using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
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
    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Identification"
    )]
    public required FName ID { get; init; }

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere,
        Category = "Identification"
    )]
    public int RowIndex { get; init; }

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    [DisplayName]
    public FText Name { get; init; } = "Unnamed";

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Name (Plural)",
        Category = "Display"
    )]
    public FText NamePlural { get; init; } = "Unnamed";

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    public FText PotionName
    {
        get => !field.Empty ? field : Name;
        init;
    } = FText.None;

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Portion Name (Plural)",
        Category = "Display"
    )]
    public FText PortionNamePlural
    {
        get => !field.Empty ? field : NamePlural;
        init;
    } = FText.None;

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    public FText Description { get; init; } = "???";

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Metadata"
    )]
    public FGameplayTagContainer Tags { get; init; }
}

[UClass]
[GameDataRepository<FItem>]
[UsedImplicitly]
public partial class UItemRepository : UAssetGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Items))]
public readonly partial record struct FItemHandle;
