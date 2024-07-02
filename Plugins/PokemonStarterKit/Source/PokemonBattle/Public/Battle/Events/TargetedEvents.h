// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "TargetedEvents.generated.h"

class IBattler;

/**
 * Enum used to look up the specific tag for a battle event
 */
UENUM(BlueprintType)
enum class ETargetedEventScope : uint8 {
    /**
     * The tag for abilities/effects that are activated by events that occur if possessed by any Pokémon in battle.
     */
    Global,

    /**
     * The tag for abilities/effects that are activated from the user of a move
     */
    User,

    /**
     * The tag for abilities/effects that are activated by an ally to the user of a move
     */
    UserAlly,

    /**
     * The tag for abilities/effects that are activated from the side of the user of a move.
     */
    UserSide,

    /**
     * The tag for abilities/effects that are activated by the target of a move
     */
    Target,
    
    /**
     * The tag for abilities/effects that are activated by an ally to the target of a move
     */
    TargetAlly,

    /**
     * The tag for abilities/effects that are activated from the side of the target of a move.
     */
    TargetSide,

    /**
     * The tag for abilities/effects that are activated for the entire battle field.
     */
    Battlefield
};

/**
 * Wrapper struct for an event that contains a set of native tags for various event types
 */
struct POKEMONBATTLE_API FTargetedEvent {

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

    /**
     * Get the specific tag for the given scope
     * @param Scope The scope of the tag to look up
     * @return The found tag
     */
    const FNativeGameplayTag& GetTagForScope(ETargetedEventScope Scope);
    
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
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .TargetSideTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".TargetSide", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD), \
    .BattlefieldTag = FNativeGameplayTag(UE_PLUGIN_NAME, UE_MODULE_NAME, Prefix ".Field", TEXT(""), \
        ENativeGameplayTagToken::PRIVATE_USE_MACRO_INSTEAD) \
    }

namespace Pokemon::Battle::Events {

/**
 * Send out a single event to the user of a move
 * @param User The user of the move in question
 * @param Payload The payload data for the event
 * @param EventTag The event tag to send out for this event
 */
POKEMONBATTLE_API void SendOutMoveEvent(const TScriptInterface<IBattler>& User, const UObject* Payload, const FNativeGameplayTag& EventTag);

/**
 * Send out a set of events to the battlefield.
 * @param User The user of the move in question
 * @param Target The target of the move in question
 * @param Payload The payload data for the event
 * @param EventTags The event tags to send out for this event
 */
POKEMONBATTLE_API void SendOutMoveEvents(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target,
                                         const UObject* Payload, const FTargetedEvent& EventTags);

}