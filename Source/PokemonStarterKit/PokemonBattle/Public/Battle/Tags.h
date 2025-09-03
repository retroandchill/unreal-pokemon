// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Pokemon::Battle {

    /**
     * The tag used for abilities whose activation should require the battle to hold until they complete. This is mainly
     * used for effects that play animations/display messages.
     */
    POKEMONBATTLE_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(HoldForComplete);

    /**
     * The tag used for events fired at the start of a battler's action
     */
    POKEMONBATTLE_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(StartAction);

    /**
     * The tag used for events fired at the end of a battler's action
     */
    POKEMONBATTLE_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(EndAction);

    /**
     * The tag used for events fired for a switch out event
     */
    POKEMONBATTLE_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(SwitchOut);

    /**
     * The tag used for events fired at the end of the turn
     */
    POKEMONBATTLE_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(StartTurn);

    /**
     * The tag used for events fired at the end of the turn
     */
    POKEMONBATTLE_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(EndTurn);

    /**
     * Add a dynamic tag to the game
     * @param Tags The map of the native names to the tag
     * @param Format The format to place the tag into
     * @param TagName The actual tag name to apply
     */
    inline void AddDynamicGameplayTag(TMap<FName, TSharedRef<FNativeGameplayTag>> &Tags, FStringView Format,
                                      FName TagName) {
        auto FullTag = FString::Format(Format.GetData(), {TagName.ToString()});
        auto Tag = MakeShared<FNativeGameplayTag>(UE_PLUGIN_NAME, UE_MODULE_NAME, *FullTag, TEXT(""),
                                                  ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
        Tags.Emplace(TagName, std::move(Tag));
    }

} // namespace Pokemon::Battle