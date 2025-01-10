// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/PokemonDTO.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "Trainers/Trainer.h"
#include "Utilities/PartyManagementHelpers.h"
#include "Utilities/TrainerHelpers.h"

class UTrainerHelpers;

namespace Pokemon::Tests {
    /**
     *
     */
    class POKEMONTESTS_API FScopedParty {
      public:
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, FPokemonDTO>
        explicit FScopedParty(const UObject *WorldContextObject, R &&Pokemon)
            : Trainer(UTrainerHelpers::GetPlayerCharacter(WorldContextObject).GetObject()),
              Pokemon(CreatePokemon(WorldContextObject, std::forward<R>(Pokemon))) {
        }

        ~FScopedParty();

        UE_NONCOPYABLE(FScopedParty)

        const TScriptInterface<IPokemon> &GetPokemon(int32 Index) const;

      private:
        template <std::ranges::input_range R>
            requires std::convertible_to<std::ranges::range_reference_t<R>, FPokemonDTO>
        static TArray<TScriptInterface<IPokemon>> CreatePokemon(const UObject *WorldContextObject, R &&Pokemon) {
            return std::forward<R>(Pokemon) |
                   Retro::Ranges::Views::Transform(
                       Retro::BindFront<&UPartyManagementHelpers::AddPokemonToParty>(WorldContextObject)) |
                   Retro::Ranges::To<TArray>();
        }

        TWeakInterfacePtr<ITrainer> Trainer;

        TArray<TScriptInterface<IPokemon>> Pokemon;
    };
} // namespace Pokemon::Tests
