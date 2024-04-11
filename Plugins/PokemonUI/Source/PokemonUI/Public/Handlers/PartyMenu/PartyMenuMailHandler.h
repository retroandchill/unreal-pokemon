// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PartyMenuHandler.h"
#include "PartyMenuMailHandler.generated.h"

/**
 * Handler for the mail option for a Pokémon
 */
UCLASS()
class POKEMONUI_API UPartyMenuMailHandler : public UPartyMenuHandler {
    GENERATED_BODY()

  public:
    bool ShouldShow(const IPartyScreen &Screen, const ITrainer &Trainer, int32 PartyIndex) const override;
    void Handle(IPartyScreen &Screen, const ITrainer &Trainer, int32 PartyIndex) override;
};
