// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Moves/DefaultMoveBlock.h"
#include "Algo/Unique.h"
#include "DataManager.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/DefaultMove.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "PokemonDataSettings.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Utils/Construct.h"
#include "Species/SpeciesData.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

TScriptInterface<IMoveBlock> UDefaultMoveBlock::Initialize(const TScriptInterface<IPokemon> &Pokemon,
                                                           const FPokemonDTO &DTO) {
    Owner = Pokemon;
    auto Species = FDataManager::GetInstance().GetDataTable<FSpeciesData>().GetData(DTO.Species);
    check(Species != nullptr)
    auto KnowableMoves =
        Species->Moves.FilterByPredicate([&DTO](const FLevelUpMove &Move) { return Move.Level <= DTO.Level; });

    MoveMemory.Append(DTO.MoveMemory);
    Algo::Transform(KnowableMoves, MoveMemory, [](const FLevelUpMove &Move) { return Move.Move; });

    // We want to get the last possible level a move can be learned at for our purposes
    Algo::Reverse(KnowableMoves);
    Algo::UniqueBy(KnowableMoves, [](const FLevelUpMove &Move) { return Move.Move; });
    Algo::Reverse(KnowableMoves);

    const auto &Settings = *GetDefault<UPokemonDataSettings>();
    int32 MaxMoves = Settings.MaxMoves;
    if (DTO.Moves.Num() > 0) {
        int32 MoveMax = FMath::Min(MaxMoves, DTO.Moves.Num());
        for (int32 i = 0; i < MoveMax; i++) {
            auto &Move = DTO.Moves[i];
            Moves.Emplace(NewObject<UDefaultMove>(this)->Initialize(Move));

            // A manual move override list could cause moves not known by default to be displayed,
            // so we need to add them manually here.
            MoveMemory.Add(Move.Move);
        }
    } else {
        int32 MoveMax = FMath::Min(MaxMoves, KnowableMoves.Num());
        for (int32 i = KnowableMoves.Num() - MoveMax; i < KnowableMoves.Num(); i++) {
            Moves.Emplace(CreateNewMove({.Move = KnowableMoves[i].Move}));
        }
    }

    return this;
}

const TArray<TScriptInterface<IMove>> &UDefaultMoveBlock::GetMoves() const {
    return Moves;
}

bool UDefaultMoveBlock::HasOpenMoveSlot() const {
    return Moves.Num() < GetDefault<UPokemonDataSettings>()->MaxMoves;
}

void UDefaultMoveBlock::PlaceMoveInOpenSlot(FMoveHandle Move) {
    check(HasOpenMoveSlot())
    Moves.Emplace(CreateNewMove({.Move = Move}));
}

void UDefaultMoveBlock::OverwriteMoveSlot(FMoveHandle Move, int32 SlotIndex) {
    check(Moves.IsValidIndex(SlotIndex))
    Moves[SlotIndex] = CreateNewMove({.Move = Move});
}

Retro::TGenerator<FMoveHandle> UDefaultMoveBlock::GetLevelUpMoves(int32 InitialLevel, int32 CurrentLevel) const {
    auto &Species = Owner->GetSpecies();

    auto MoveLevelInRange = [InitialLevel, CurrentLevel](const FLevelUpMove &Move) {
        return Move.Level > InitialLevel && Move.Level <= CurrentLevel;
    };
    auto DoesNotKnowMove = [this](const FLevelUpMove &Move) {
        return !Moves.ContainsByPredicate(
            [&Move](const TScriptInterface<IMove> &MoveData) { return MoveData->GetMoveData().ID == Move.Move; });
    };

    // clang-format off
    co_yield Retro::Ranges::TElementsOf(Species.Moves |
           Retro::Ranges::Views::Filter(MoveLevelInRange) |
           Retro::Ranges::Views::Filter(DoesNotKnowMove) |
           Retro::Ranges::Views::Transform(&FLevelUpMove::Move) |
           Retro::Ranges::Views::Transform(Retro::Construct<FMoveHandle>));
    // clang-format on
}

UE5Coro::TCoroutine<bool> UDefaultMoveBlock::LearnMove(FMoveHandle Move, FForceLatentCoroutine Coro) {
    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(this);
    co_return co_await Dispatcher.LearnMove(this, Move);
}

TScriptInterface<IMove> UDefaultMoveBlock::CreateNewMove(const FMoveDTO &MoveID) {
    return NewObject<UDefaultMove>(this)->Initialize(MoveID);
}

const TSet<FName> &UDefaultMoveBlock::GetMoveMemory() const {
    return MoveMemory;
}