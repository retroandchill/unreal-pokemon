// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * Wrapper struct for an event that contains a set of native tags for various event types
 */
struct FTargetedEvent {

    /**
     * The tag for abilities/effects that are activated by events that occur if possessed by any Pokémon in battle.
     */
    FNativeGameplayTag GlobalTag;

    /**
     * The tag for abilities/effects that are activated from the user of a move
     */
    FNativeGameplayTag UserTag;

    /**
     * The tag for abilities/effects that are activated by an ally to the user of a move
     */
    FNativeGameplayTag UserAllyTag;

    /**
     * The tag for abilities/effects that are activated from the side of the user of a move.
     */
    FNativeGameplayTag UserSideTag;

    /**
     * The tag for abilities/effects that are activated by the target of a move
     */
    FNativeGameplayTag TargetTag;

    /**
     * The tag for abilities/effects that are activated by an ally to the target of a move
     */
    FNativeGameplayTag TargetAllyTag;

    /**
     * The tag for abilities/effects that are activated from the side of the target of a move.
     */
    FNativeGameplayTag TargetSideTag;

    /**
     * The tag for abilities/effects that are activated for the entire battle field.
     */
    FNativeGameplayTag BattlefieldTag;
    
};

/**
 * Declare a set of targeted events with the following variable name.
 * @param Name The name of the variable
 */
#define DECLARE_TARGETED_EVENT_TYPE(Name) extern FTargetedEvent Name;

/**
 * Define the set of targeted events to be used.
 * @param Name The name of the variable that is being defined
 * @param Prefix The prefix to append the scope names to create the tag
 */
#define DEFINE_TARGETED_EVENT_TYPE(Name, Prefix) FTargetedEvent Name { \
    .GlobalTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".Global", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .UserTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".User", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .UserAllyTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".UserAlly", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .UserSideTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".UserSide", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .TargetTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".Target", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .TargetAllyTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".TargetAlly", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD) \
    .TargetSideTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".TargetSide", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD) \
    .BattlefieldTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".Field", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD) \
    }