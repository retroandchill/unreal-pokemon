// "Unreal Pokémon" created by Retro & Chill.

#include "GameplayTags/GameplayTagUtilities.h"
#include "GameplayTagsManager.h"

FGameplayTag UGameplayTagUtilities::RequestGameplayTagChild(const FGameplayTag &Base, FName Suffix) {
    auto &Manager = UGameplayTagsManager::Get();
    auto GameplayTag = Base.ToString();
    GameplayTag.Append(TEXT(".")).Append(Suffix.ToString());
    return Manager.RequestGameplayTag(FName(*GameplayTag), false);
}