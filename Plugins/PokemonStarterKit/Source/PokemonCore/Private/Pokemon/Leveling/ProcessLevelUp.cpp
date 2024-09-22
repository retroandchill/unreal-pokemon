// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Leveling/ProcessLevelUp.h"
#include "Blueprints/UtilityNodeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/Node/Utility_ProcessLevelUp.h"

UProcessLevelUp *UProcessLevelUp::ProcessLevelUp(const TScriptInterface<IPokemon> &Pokemon,
                                                 const FLevelUpStatChanges &StatChanges, bool bShowMessages) {
    auto Node = NewObject<UProcessLevelUp>();
    Node->Pokemon = Pokemon;
    Node->StatChanges = StatChanges;
    Node->bShowMessages = bShowMessages;
    return Node;
}

void UProcessLevelUp::Activate() {
    auto Subsystem = UGameplayStatics::GetGameInstance(Pokemon.GetObject())->GetSubsystem<UUtilityNodeSubsystem>();
    Subsystem->ExecuteUtilityFunction<UUtility_ProcessLevelUp>(
        Pokemon, StatChanges, bShowMessages,
        FSimpleDelegate::CreateUObject(this, &UProcessLevelUp::ExecuteAfterLevelUp));
}

void UProcessLevelUp::ExecuteAfterLevelUp() {
    AfterLevelUp.Broadcast();
    SetReadyToDestroy();
}