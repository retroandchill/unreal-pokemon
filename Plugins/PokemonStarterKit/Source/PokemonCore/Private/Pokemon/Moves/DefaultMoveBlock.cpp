﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Moves/DefaultMoveBlock.h"
#include "Algo/Unique.h"
#include "Blueprints/UtilityNodeSubsystem.h"
#include "DataManager.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/DefaultMove.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "PokemonDataSettings.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Utilities/Construct.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Map.h"
#include "Species/SpeciesData.h"
#include "Utilities/Node/Utility_LearnMove.h"

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

TArray<FMoveHandle> UDefaultMoveBlock::GetLevelUpMoves(int32 InitialLevel, int32 CurrentLevel) const {
    auto &Species = Owner->GetSpecies();

    auto MoveLevelInRange = [InitialLevel, CurrentLevel](const FLevelUpMove &Move) {
        return Move.Level > InitialLevel && Move.Level <= CurrentLevel;
    };
    auto DoesNotKnowMove = [this](const FLevelUpMove &Move) {
        return !Moves.ContainsByPredicate(
            [&Move](const TScriptInterface<IMove> &MoveData) { return MoveData->GetMoveData().ID == Move.Move; });
    };

    // clang-format off
    return Species.Moves |
           UE::Ranges::Filter(MoveLevelInRange) |
           UE::Ranges::Filter(DoesNotKnowMove) |
           UE::Ranges::Map(&FLevelUpMove::Move) |
           UE::Ranges::Map(UE::Ranges::Construct<FMoveHandle>) |    
           UE::Ranges::ToArray;
    // clang-format on
}

void UDefaultMoveBlock::LearnMove(FMoveHandle Move, FOnMoveLearnEnd::FDelegate &&AfterMoveLearned) {
    auto Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UUtilityNodeSubsystem>();
    Subsystem->ExecuteUtilityFunction<UUtility_LearnMove>(Owner, Move, std::move(AfterMoveLearned));
}

TScriptInterface<IMove> UDefaultMoveBlock::CreateNewMove(const FMoveDTO &MoveID) {
    return NewObject<UDefaultMove>(this)->Initialize(MoveID);
}

const TSet<FName> &UDefaultMoveBlock::GetMoveMemory() const {
    return MoveMemory;
}