// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ObtainedBlock.h"
#include "UObject/Object.h"

#include "DefaultObtainedBlock.generated.h"

/**
 * The default implementation for the obtained information
 */
UCLASS()
class POKEMONCORE_API UDefaultObtainedBlock : public UObject, public IObtainedBlock {
    GENERATED_BODY()

  public:
    TScriptInterface<IObtainedBlock> Initialize(const FPokemonDTO &DTO) override;

    UFUNCTION(BlueprintPure, Category = "Trainer Memo")
    EObtainMethod GetObtainMethod() const override;

    UFUNCTION(BlueprintPure, Category = "Trainer Memo")
    int32 GetLevelMet() const override;

    TOptional<const FDateTime &> GetTimeReceived() const override;
    TOptional<FText> GetObtainText() override;
    TOptional<const FDateTime &> GetTimeHatched() const override;

    UFUNCTION(BlueprintCallable, Category = "Trainer Memo")
    void SetTimeHatched(const FDateTime &DateTime) override;

    TOptional<FText> GetHatchedMap() override;

    UFUNCTION(BlueprintCallable, Category = "Trainer Memo")
    void SetHatchedMap(const FText &MapName) override;

  private:
    UPROPERTY(SaveGame)
    EObtainMethod ObtainMethod;

    UPROPERTY(SaveGame)
    int32 LevelMet;

    UPROPERTY(SaveGame)
    FDateTime TimeReceived;

    UPROPERTY(SaveGame)
    FText ObtainText;

    UPROPERTY(SaveGame)
    TOptional<FDateTime> TimeHatched;

    UPROPERTY(SaveGame)
    TOptional<FText> HatchedMap;
};
