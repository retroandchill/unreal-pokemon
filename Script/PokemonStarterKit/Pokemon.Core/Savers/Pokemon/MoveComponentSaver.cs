using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Model.Moves;
using Pokemon.Core.Model.Pokemon;
using Pokemon.Core.Moves;
using Pokemon.Data.Model.PBS;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Pokemon;

/// <summary>
/// Represents a component saver specifically designed for the UMoveComponent.
/// This class is responsible for mapping and persisting the state of a UMoveComponent into the corresponding
/// FMoveComponentInfo structure during save and load operations.
/// </summary>
/// <remarks>
/// Utilizes the Riok.Mapperly library for object mapping and adheres to the required mapping strategy.
/// Implements the ComponentSaver attribute with a logger class for handling save-related operations.
/// </remarks>
/// <seealso cref="UMoveComponent"/>
/// <seealso cref="FMoveComponentInfo"/>
/// <seealso cref="UComponentSaver"/>
[UClass]
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
[ComponentSaver<UMoveComponent, FMoveComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UMoveComponentSaver : UComponentSaver
{
    [MapProperty(nameof(UMoveComponent.MoveList), nameof(FMoveComponentInfo.Moves))]
    [MapProperty(nameof(UMoveComponent.InitialMoveList), nameof(FMoveComponentInfo.InitialMoves))]
    private partial FMoveComponentInfo SaveComponent(UMoveComponent component);

    private void LoadComponent(FMoveComponentInfo info, UMoveComponent component)
    {
        component.SetMoves(info.Moves.Select(m => CreateMove(component, m)));

        component.ClearFirstMoves();
        foreach (var move in info.InitialMoves)
        {
            component.AddFirstMove(move);
        }
    }

    private UPokemonMove CreateMove(UObject outer, FPokemonMoveInfo moveInfo)
    {
        var move = NewObject<UPokemonMove>(outer);
        SetMoveProperties(moveInfo, move);
        return move;
    }

    private partial void SetMoveProperties(FPokemonMoveInfo moveInfo, UPokemonMove move);
}
