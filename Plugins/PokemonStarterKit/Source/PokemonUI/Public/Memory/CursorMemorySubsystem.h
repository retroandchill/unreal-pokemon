// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "CursorMemorySubsystem.generated.h"

/**
 * Subsystem used to store the memory of cursor locations in the bag between menu visits.
 */
UCLASS()
class POKEMONUI_API UCursorMemorySubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    /**
     * Get the bag pocket cursor memory
     * @return The memory of the cursor position within the pockets of the bag
     */
    const TMap<FName, int32> &GetBagPocketMemory() const;

    /**
     * Update the new position of the index for the pocket
     * @param Pocket The pocket in question
     * @param NewIndex The new index of the given pocket
     */
    void UpdatePocketMemory(FName Pocket, int32 NewIndex);

    /**
     * Get the list of bag pocket names
     * @return The list of names for each pocket in the bag
     */
    const TArray<FName> &GetBagPocketNames() const;

  private:
    /**
     * The memory of the cursor position within the pockets of the bag
     */
    TMap<FName, int32> BagPocketMemory;

    /**
     * The list of names for each pocket in question
     */
    TArray<FName> PocketNames;
};
