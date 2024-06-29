// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Pokemon::Battle {

/**
 * Add a dynamic tag to the game
 * @param Tags The map of the native names to the tag
 * @param Format The format to place the tag into
 * @param TagName The actual tag name to apply
 */
inline void AddDynamicGameplayTag(TMap<FName, TSharedRef<FNativeGameplayTag>>& Tags, FStringView Format, FName TagName) {
    auto FullTag = FString::Format(Format.GetData(), { TagName.ToString() });
    auto Tag = MakeShared<FNativeGameplayTag>(UE_PLUGIN_NAME, UE_MODULE_NAME, *FullTag, TEXT(""), ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD);
    Tags.Emplace(TagName, MoveTemp(Tag));
}

}