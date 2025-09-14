using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using Pokemon.Core.Entities;
using Pokemon.Core.Moves;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;
using ZLinq;

namespace Pokemon.Core.Components.Pokemon;

[UClass]
[UMetaData("HideCategories", "Moves")]
[UsedImplicitly]
public class UMoveComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UIdentityComponent IdentityComponent { get; set; }

    [UProperty(PropertyFlags.Transient)]
    private UStatComponent StatComponent { get; set; }

    [UProperty]
    [UsedImplicitly]
    private TArray<UPokemonMove> Moves { get; }

    public IReadOnlyList<UPokemonMove> MoveList
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Moves;
    }

    [UProperty]
    [UsedImplicitly]
    private TArray<FMoveHandle> InitialMoves { get; }

    public IReadOnlyList<FMoveHandle> InitialMoveList
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => InitialMoves;
    }

    public int MoveCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Moves.Count;
    }

    protected override void PreInitialize()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
        StatComponent = GetRequiredSiblingComponent<UStatComponent>();
    }

    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        ResetMoves();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public bool HasMove(FMoveHandle move)
    {
        return move.IsValid && Moves.Any(x => x.Move == move);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void ResetMoves()
    {
        var species = IdentityComponent.Species.Entry;
        var level = StatComponent.Level;

        Moves.Clear();
        foreach (
            var (move, _) in species
                .Moves.AsValueEnumerable(x => (x.Move, Level: x.Match(l => l, () => 0)))
                .Where(x => x.Level <= level)
                .OrderByDescending(x => x.Level)
                .DistinctBy(x => x.Move)
                .Take(PokemonStatics.MaxMoves)
                .Reverse()
        )
        {
            Moves.Add(UPokemonMove.Create(this, move));
        }
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void LearnMove(FMoveHandle move)
    {
        if (!move.IsValid)
            return;

        // If we already know the move just shift it down to the end of the list
        for (var i = 0; i < Moves.Count; i++)
        {
            var moveAtIndex = Moves[i];
            if (moveAtIndex.Move != move)
                continue;
            Moves.RemoveAt(i);
            Moves.Add(moveAtIndex);
            return;
        }

        // Otherwise, add it to the end of the list
        Moves.Add(UPokemonMove.Create(this, move));

        // If we have too many moves, remove the first one
        if (Moves.Count > PokemonStatics.MaxMoves)
            Moves.RemoveAt(0);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void ForgetMove(FMoveHandle move)
    {
        if (!move.IsValid)
            return;

        for (var i = 0; i < Moves.Count; i++)
        {
            var moveAtIndex = Moves[i];
            if (moveAtIndex.Move != move)
                continue;
            Moves.RemoveAt(i);
            return;
        }
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void ForgetMoveAtIndex(int index)
    {
        if (index < 0 || index >= Moves.Count)
            return;

        Moves.RemoveAt(index);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void ForgetAllMoves()
    {
        Moves.Clear();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void RecordFirstMoves()
    {
        InitialMoves.Clear();
        foreach (var move in Moves)
        {
            InitialMoves.Add(move.Move);
        }
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void AddFirstMove(FMoveHandle move)
    {
        if (!move.IsValid || InitialMoves.Contains(move))
            return;

        InitialMoves.Add(move);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void ClearFirstMoves()
    {
        InitialMoves.Clear();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public bool IsCompatibleWithMove(FMoveHandle move)
    {
        if (!move.IsValid)
            return false;

        var species = IdentityComponent.Species.Entry;
        return species
            .Moves.AsValueEnumerable(x => x.Move)
            .Concat(species.TutorMoves.AsValueEnumerable())
            .Concat(species.EggMoves.AsValueEnumerable())
            .Any(x => x == move);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public bool CanRelearnMove()
    {
        var species = IdentityComponent.Species.Entry;
        var level = StatComponent.Level;
        return species
            .Moves.AsValueEnumerable()
            .Where(x => x.Match(l => l <= level, () => true))
            .Select(x => x.Move)
            .Concat(InitialMoves)
            .Any(m => !HasMove(m));
    }
}
