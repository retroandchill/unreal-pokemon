// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

struct FTargetedEvent {

    FNativeGameplayTag GlobalTag;

    FNativeGameplayTag UserTag;

    FNativeGameplayTag UserAllyTag;

    FNativeGameplayTag TargetTag;

    FNativeGameplayTag TargetAllyTag;
    
};

#define DECLARE_TARGETED_EVENT_TYPE(Name) extern FTargetedEvent Name;

#define DEFINE_TARGETED_EVENT_TYPE(Name, Prefix) FTargetedEvent Name { \
    .GlobalTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".Global", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .UserTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".User", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .UserAllyTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".UserAlly", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .TargetTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".Target", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .TargetAllyTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".TargetAlly", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD) \
    }