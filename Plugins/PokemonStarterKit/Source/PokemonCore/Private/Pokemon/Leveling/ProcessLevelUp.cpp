// "Unreal Pokémon" created by Retro & Chill.


#include "Pokemon/Leveling/ProcessLevelUp.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/UtilitiesSubsystem.h"

UProcessLevelUp * UProcessLevelUp::ProcessLevelUp(const TScriptInterface<IPokemon> &Pokemon,
                                                  const FLevelUpStatChanges &StatChanges, bool bShowMessages) {
    auto Node = NewObject<UProcessLevelUp>();
    Node->Pokemon = Pokemon;
    Node->StatChanges = StatChanges;
    Node->bShowMessages = bShowMessages;
    return Node;
}

void UProcessLevelUp::Activate() {
    FLevelUpEnd LevelUpEnd;
    LevelUpEnd.BindDynamic(this, &UProcessLevelUp::ExecuteAfterLevelUp);

    auto Subsystem = UGameplayStatics::GetGameInstance(Pokemon.GetObject())->GetSubsystem<UUtilitiesSubsystem>();
    IPokemonUtilities::Execute_ProcessLevelUp(Subsystem->GetPokemonUtilities(), Pokemon.GetObject(), Pokemon,
        StatChanges, bShowMessages, LevelUpEnd);
}

void UProcessLevelUp::ExecuteAfterLevelUp() {
    AfterLevelUp.Broadcast();
    SetReadyToDestroy();
}