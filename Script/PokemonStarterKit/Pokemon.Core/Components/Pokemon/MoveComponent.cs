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

/// <summary>
/// The UMoveComponent class is responsible for managing the set of moves associated with a Pokémon instance.
/// It provides functionalities to initialize, query, and modify the move list, as well as manage compatible moves,
/// learning new moves, forgetting existing moves, and handling the first set of moves.
/// </summary>
[UClass]
[UMetaData("HideCategories", "Moves")]
public partial class UMoveComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private partial UIdentityComponent IdentityComponent { get; set; }

    [UProperty(PropertyFlags.Transient)]
    private partial UStatComponent StatComponent { get; set; }

    [UProperty]
    private partial TArray<UPokemonMove> Moves { get; }

    /// <summary>
    /// Provides a read-only list of moves associated with the current component.
    /// </summary>
    /// <remarks>
    /// This property retrieves all moves currently available within the move component.
    /// It is a blueprint-pure property, used in various contexts where the move data is required
    /// without directly modifying the underlying collection.
    /// </remarks>
    public IReadOnlyList<UPokemonMove> MoveList
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Moves;
    }

    [UProperty]
    private partial TArray<FMoveHandle> InitialMoves { get; }

    /// <summary>
    /// Provides a read-only list of the initial moves associated with the component.
    /// </summary>
    /// <remarks>
    /// This property retrieves the original set of moves assigned to the component upon initialization.
    /// It is a blueprint-pure property, primarily used to reference the predefined move collection
    /// without altering the underlying data.
    /// </remarks>
    public IReadOnlyList<FMoveHandle> InitialMoveList
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => InitialMoves;
    }

    /// <summary>
    /// Retrieves the total count of moves currently associated with the move component.
    /// </summary>
    /// <remarks>
    /// This property provides the number of moves stored within the component.
    /// It offers a quick way to check the size of the move set without accessing the full list of moves.
    /// The count is read-only and purely informational.
    /// </remarks>
    public int MoveCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Moves")]
        get => Moves.Count;
    }

    /// <inheritdoc />
    protected override void PreInitialize_Implementation()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
        StatComponent = GetRequiredSiblingComponent<UStatComponent>();
    }

    /// <summary>
    /// Initializes the move component using the provided Pokémon initialization parameters.
    /// </summary>
    /// <param name="initParams">The parameters used to initialize the Pokémon, including its species and level.</param>
    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        ResetMoves();
    }

    /// <summary>
    /// Determines if the specified move exists in the current list of moves.
    /// </summary>
    /// <param name="move">The move to check for existence in the list of moves.</param>
    /// <returns>True if the move exists in the list; otherwise, false.</returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public bool HasMove(FMoveHandle move)
    {
        return move.IsValid && Moves.Any(x => x.Move == move);
    }

    internal void SetMoves(IEnumerable<UPokemonMove> moves)
    {
        Moves.Clear();
        foreach (var move in moves)
        {
            Moves.Add(move);
        }
    }

    /// <summary>
    /// Resets the Pokémon's moves based on its species' learnset and current level.
    /// Clears all existing moves and sets a new set of moves learned up to the Pokémon's level,
    /// ensuring the list contains the most recent and unique moves limited by the maximum allowed moves.
    /// </summary>
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

    /// <summary>
    /// Adds a new move to the Pokémon's move list.
    /// If the move already exists in the list, it is moved to the end.
    /// If adding a new move exceeds the maximum number of allowed moves, the oldest move is removed.
    /// </summary>
    /// <param name="move">The move to add to the Pokémon's move list.</param>
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

    /// <summary>
    /// Forgets a move from the current list of Pokémon moves if it matches the specified move handle.
    /// </summary>
    /// <param name="move">The handle of the move to be forgotten. If the handle is invalid, no action is performed.</param>
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

    /// <summary>
    /// Removes the move at the specified index from the Pokémon's list of moves, if the index is valid.
    /// </summary>
    /// <param name="index">The zero-based index of the move to remove from the list.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void ForgetMoveAtIndex(int index)
    {
        if (index < 0 || index >= Moves.Count)
            return;

        Moves.RemoveAt(index);
    }

    /// <summary>
    /// Removes all moves currently associated with the Pokémon instance managed by this component.
    /// After execution, the move list will be empty. This operation is irreversible unless explicitly reset or
    /// re-initialized with a predefined set of moves.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void ForgetAllMoves()
    {
        Moves.Clear();
    }

    /// <summary>
    /// Captures the current set of moves associated with the Pokémon and stores them as its initial moves.
    /// This method ensures the initial move list reflects the Pokémon's current move state at the time of calling,
    /// clearing any previously recorded initial moves before populating the list.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void RecordFirstMoves()
    {
        InitialMoves.Clear();
        foreach (var move in Moves)
        {
            InitialMoves.Add(move.Move);
        }
    }

    /// <summary>
    /// Adds a move to the list of initial moves if it is valid and not already included.
    /// </summary>
    /// <param name="move">The move to be added to the list of initial moves.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void AddFirstMove(FMoveHandle move)
    {
        if (!move.IsValid || InitialMoves.Contains(move))
            return;

        InitialMoves.Add(move);
    }

    /// <summary>
    /// Clears all moves marked as the initial moves for the current component.
    /// This method modifies the internal state by removing all entries
    /// from the collection of recorded initial moves, effectively resetting
    /// it to an empty state.
    /// </summary>
    /// <remarks>
    /// Typically used to reset any moves that were initially recorded as
    /// part of the first moves in the component's move list.
    /// This operation is irreversible and affects only the internal
    /// management of initial moves, not the overall move list.
    /// Declared as callable from Blueprints for enhanced flexibility.
    /// </remarks>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Moves")]
    public void ClearFirstMoves()
    {
        InitialMoves.Clear();
    }

    /// <summary>
    /// Determines whether the specified move is compatible with the identity's species.
    /// </summary>
    /// <param name="move">The move handle to check for compatibility.</param>
    /// <returns>
    /// <c>true</c> if the move is compatible with the species based on its moves, tutor moves, or egg moves;
    /// otherwise, <c>false</c>.
    /// </returns>
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

    /// <summary>
    /// Determines whether the Pokémon can relearn a move based on its current level
    /// and species' available moves.
    /// </summary>
    /// <return>True if the Pokémon has at least one move that can be relearned
    /// by matching its species' move list or initial moves; otherwise, false.</return>
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
