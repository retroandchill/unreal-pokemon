// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Pokemon::Battle::StatusEffects {

    /**
     * The format used for the status effect tags
     */
    constexpr auto StatusEffectTagFormat = TEXT("Battle.StatusEffects.{0}");

    /**
     * The format used for the status effect immunity tags
     */
    constexpr auto StatusEffectImmunityTagFormat = TEXT("Battle.Battler.StatusEffects.Immunity.{0}");

    /**
     * Singleton class for looking up status effect tags by their ID
     */
    class POKEMONBATTLE_API FLookup {
        FLookup();
        ~FLookup();

      public:
        /**
         * Get the singleton instance of this class
         * @return The sole instance of the class
         */
        static FLookup &GetInstance();

        /**
         * Get the tag for the battler having the status condition
         * @param ID The ID of the status condition
         * @return The corresponding tag
         */
        FORCEINLINE const FNativeGameplayTag &GetTag(FName ID) {
            return *Tags.FindChecked(ID);
        }

        /**
         * Get the tag for the battler being immune to the status condition
         * @param ID The ID of the status condition
         * @return The corresponding tag
         */
        FORCEINLINE const FNativeGameplayTag &GetImmunityTag(FName ID) {
            return *Tags.FindChecked(ID);
        }

      private:
        /**
         * The map of status effect IDs to native gameplay tags.
         */
        TMap<FName, TSharedRef<FNativeGameplayTag>> Tags;

        /**
         * The map of status effect IDs to native gameplay tags for immunity to the conditions.
         */
        TMap<FName, TSharedRef<FNativeGameplayTag>> ImmunityTags;
    };

} // namespace Pokemon::Battle::StatusEffects
