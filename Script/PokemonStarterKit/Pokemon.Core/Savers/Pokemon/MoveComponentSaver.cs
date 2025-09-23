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

[UClass]
[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
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
