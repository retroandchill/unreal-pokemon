// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Utilities/Node/Utility_ProcessLevelUp.h"

#include "ProcessLevelUp.generated.h"

class IPokemon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAfterLevelUp);

/**
 *
 */
UCLASS(meta = (HideThen))
class POKEMONCORE_API UProcessLevelUp : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Pokémon|Stats")
    static UProcessLevelUp *ProcessLevelUp(const TScriptInterface<IPokemon> &Pokemon,
                                           const FLevelUpStatChanges &StatChanges, bool bShowMessages);

    void Activate() override;

  private:
    /**
     * Execute the callback after exp gain is completed
     */
    UFUNCTION()
    void ExecuteAfterLevelUp();

    /**
     * Called after level up messages are shown
     */
    UPROPERTY(BlueprintAssignable)
    FAfterLevelUp AfterLevelUp;

    /**
     * The Pokémon to gain exp
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;

    /**
     * The amount of Exp. to gain
     */
    UPROPERTY()
    FLevelUpStatChanges StatChanges;

    /**
     * Should level up messages be show? (Hiding messages does not allow Pokémon to learn moves on level up)
     */
    UPROPERTY()
    bool bShowMessages;
};
