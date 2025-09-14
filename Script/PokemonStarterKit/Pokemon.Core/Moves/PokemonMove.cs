using GameDataAccessTools.Core.Views;
using LanguageExt;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Moves;

[UClass]
public class UPokemonMove : UObject
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Moves")]
    public FMoveHandle Move
    {
        get;
        set
        {
            field = value;
            PP = Math.Clamp(PP, 0, TotalPP);
        }
    }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Moves")]
    public int PP
    {
        get;
        set => field = Math.Clamp(value, 0, TotalPP);
    }

    [UProperty(PropertyFlags.BlueprintReadWrite, DisplayName = "PP Ups", Category = "Moves")]
    public int PPUps
    {
        get;
        set => field = Math.Max(value, 0);
    }

    public int TotalPP
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get
        {
            var maxPP = Move.Entry.TotalPP;
            return maxPP + maxPP * PPUps / 5;
        }
    }

    public static UPokemonMove Create(UObject outer, FMoveHandle move)
    {
        var newMove = NewObject<UPokemonMove>(outer);
        newMove.Move = move;
        newMove.PP = newMove.TotalPP;
        return newMove;
    }

    public FGameplayTag FunctionCode
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.FunctionCode;
    }

    public FDamageTypeData Power
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Damage;
    }

    public FTypeHandle Type
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Type;
    }

    public EDamageCategory Category
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Category;
    }

    public bool IsPhysical
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Physical", Category = "Moves")]
        get => Move.Entry.IsPhysical;
    }

    public bool IsSpecial
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Physical", Category = "Moves")]
        get => Move.Entry.IsSpecial;
    }

    public bool IsStatus
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Status", Category = "Moves")]
        get => Move.Entry.IsStatus;
    }

    public Option<int> Accuracy
    {
        get
        {
            var move = Move.Entry;
            return !move.AlwaysHits ? Accuracy : Option<int>.None;
        }
    }

    public Option<int> EffectChance
    {
        get
        {
            var move = Move.Entry;
            return !move.GuaranteedEffect ? EffectChance : Option<int>.None;
        }
    }

    public FTargetHandle Target
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Target;
    }

    public int Priority
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Priority;
    }

    public StructView<FGameplayTagContainer> Tags => Move.Entry.Tags;

    public FText Name
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Name;
    }

    public FText Description
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Move.Entry.Description;
    }

    public bool IsHiddenMove
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Hidden Move", Category = "Moves")]
        get => Move.Entry.IsHiddenMove;
    }

    // TODO: These need to be updated to actually get the true display information

    public FTypeHandle DisplayType
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Display")]
        get => Type;
    }

    public EDamageCategory DisplayCategory
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Display")]
        get => Category;
    }

    public FDamageTypeData DisplayDamage
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Display")]
        get => Power;
    }

    public Option<int> DisplayAccuracy
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Display")]
        get => Accuracy;
    }
}
