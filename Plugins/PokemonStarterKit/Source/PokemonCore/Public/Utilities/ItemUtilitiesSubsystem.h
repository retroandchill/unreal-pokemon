// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ItemUtilitiesSubsystem.generated.h"

/**
 * Subsystem that is used to get the item utilities object.
 */
UCLASS()
class POKEMONCORE_API UItemUtilitiesSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    void Deinitialize() override;

    /**
     * Get the item utilities object
     * @return The object in question
     */
    UObject *GetItemUtilities() const;

  private:
    /**
     * The item utilities class object
     */
    UPROPERTY()
    TObjectPtr<UObject> ItemUtilities;
};
