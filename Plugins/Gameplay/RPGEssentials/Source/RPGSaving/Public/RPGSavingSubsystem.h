// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RPGEntitySaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "RPGSavingSubsystem.generated.h"

class UComponentSaver;
class UEntitySaver;

/**
 * URPGSavingSubsystem is responsible for managing the saving and loading of RPG entities and their components.
 * It provides methods to save and load data for individual entities and components, with an underlying
 * subsystem-based implementation.
 */
UCLASS(DisplayName = "RPG Saving Subsystem")
class RPGSAVING_API URPGSavingSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    /**
     * Saves the state of the specified RPG entity and returns a handle to its saved data.
     * This function uses the appropriate entity saver to serialize the entity's data. If no
     * saver is found for the entity's class, a null optional is returned.
     *
     * @param Entity The RPG entity to be saved. Must be a valid entity and cannot be null.
     * @return An optional handle to the saved data for the entity. Returns null if saving fails
     *         or no suitable entity saver is available.
     */
    UFUNCTION(meta = (ScriptMethod))
    TOptional<FRPGEntitySaveDataHandle> SaveEntity(const URPGEntity *Entity) const;

    /**
     * Saves the state of the specified RPG component and returns a handle to its saved data.
     * This function utilizes the appropriate component saver to serialize the component's data.
     * If no saver is available for the component's class or the operation fails, a null optional is returned.
     *
     * @param Component The RPG component to be saved. Must be a valid component and cannot be null.
     * @return An optional handle to the saved data for the component. Returns null if saving
     *         fails or no suitable component saver is available.
     */
    UFUNCTION(meta = (ScriptMethod))
    TOptional<FRPGComponentSaveDataHandle> SaveComponent(const URPGComponent *Component) const;

    /**
     * Loads the state of the specified RPG entity using the provided save data handle.
     * This function retrieves the appropriate entity saver and restores the entity's state.
     * If no suitable entity saver is found, the operation fails.
     *
     * @param Entity The RPG entity to be loaded. Must be a valid entity and cannot be null.
     * @param SaveData The handle containing the saved data to load into the entity.
     * @return A boolean value indicating whether the operation was successful. Returns false
     *         if no suitable entity saver is available or if the operation fails.
     */
    UFUNCTION(meta = (ScriptMethod))
    bool LoadEntity(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

    /**
     * Loads the state of the specified RPG component using the provided save data handle.
     * This function retrieves the appropriate component saver and restores the component's state.
     * If no suitable component saver is found, the operation fails.
     *
     * @param Component The RPG component to be loaded. Must be a valid component and cannot be null.
     * @param SaveData The handle containing the saved data to load into the component.
     * @return A boolean value indicating whether the operation was successful. Returns false
     *         if no suitable component saver is available or if the operation fails.
     */
    UFUNCTION(meta = (ScriptMethod))
    bool LoadComponent(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

  private:
    UEntitySaver *GetEntitySaver(const TSubclassOf<URPGEntity> EntityClass) const;

    UComponentSaver *GetComponentSaver(const TSubclassOf<URPGComponent> ComponentClass) const;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UEntitySaver>> EntitySavers;

    UPROPERTY(Transient)
    mutable TMap<TSubclassOf<URPGEntity>, TObjectPtr<UEntitySaver>> EntitySaverCache;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UComponentSaver>> ComponentSavers;

    UPROPERTY(Transient)
    mutable TMap<TSubclassOf<URPGComponent>, TObjectPtr<UComponentSaver>> ComponentSaverCache;
};
