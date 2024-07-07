// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Moves/DefaultMoveBlock.h"
#include "Algo/Unique.h"
#include "DataManager.h"
#include "RangeHelpers.h"
#include "Moves/MoveData.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Moves/DefaultMove.h"
#include "Pokemon/PokemonDTO.h"
#include "Settings/BaseSettings.h"
#include "Species/SpeciesData.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

TScriptInterface<IMoveBlock> UDefaultMoveBlock::Initialize(const TScriptInterface<IPokemon>& Pokemon, const FPokemonDTO &DTO) {
    auto Species = FDataManager::GetInstance().GetDataTable<FSpeciesData>().GetData(DTO.Species);
    check(Species != nullptr)
    auto KnowableMoves =
        Species->Moves.FilterByPredicate([&DTO](const FLevelUpMove &Move) { return Move.Level <= DTO.Level; });

    Algo::Transform(KnowableMoves, MoveMemory, [](const FLevelUpMove &Move) { return Move.Move; });

    // We want to get the last possible level a move can be learned at for our purposes
    Algo::Reverse(KnowableMoves);
    Algo::UniqueBy(KnowableMoves, [](const FLevelUpMove &Move) { return Move.Move; });
    Algo::Reverse(KnowableMoves);

    const auto &Settings = Pokemon::FBaseSettings::Get();
    int32 MaxMoves = Settings.GetMaxMoves();
    if (DTO.Moves.Num() > 0) {
        int32 MoveMax = FMath::Min(MaxMoves, DTO.Moves.Num());
        for (int32 i = 0; i < MoveMax; i++) {
            auto &Move = DTO.Moves[i];
            Moves.Emplace(NewObject<UDefaultMove>(this)->Initialize(Move));

            // A manual move override list could cause moves not known by default to be displayed,
            // so we need to add them manually here.
            MoveMemory.Add(Move);
        }
    } else {
        int32 MoveMax = FMath::Min(MaxMoves, KnowableMoves.Num());
        for (int32 i = KnowableMoves.Num() - MoveMax; i < KnowableMoves.Num(); i++) {
            Moves.Emplace(NewObject<UDefaultMove>(this)->Initialize(KnowableMoves[i].Move));
        }
    }

    return this;
}

const TArray<TScriptInterface<IMove>> &UDefaultMoveBlock::GetMoves() const {
    return Moves;
}

TArray<FName> UDefaultMoveBlock::GetLevelUpMoves(int32 InitialLevel, int32 CurrentLevel) const {
    auto &Species = Owner->GetSpecies();

    auto MoveLevelInRange = [InitialLevel, CurrentLevel](const FLevelUpMove &Move) {
        return Move.Level > InitialLevel && Move.Level <= CurrentLevel;
    };
    auto DoesNotKnowMove = [this](const FLevelUpMove& Move) {
        return !Moves.ContainsByPredicate([&Move](const TScriptInterface<IMove>& MoveData) {
            return MoveData->GetMoveData().ID == Move.Move;
        });
    };
    
    return RangeHelpers::CreateRange(Species.Moves)
        | ranges::views::filter(MoveLevelInRange)
        | ranges::views::filter(DoesNotKnowMove)
        | ranges::views::transform([](const FLevelUpMove& Move) { return Move.Move; })
        | RangeHelpers::TToArray<FName>();
}
