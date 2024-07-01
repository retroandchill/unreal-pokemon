// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/BattleAbilitySystemComponent.h"


// Sets default values for this component's properties
UBattleAbilitySystemComponent::UBattleAbilitySystemComponent() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UBattleAbilitySystemComponent::BeginPlay() {
    Super::BeginPlay();

    // ...
    
}


// Called every frame
void UBattleAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

