// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RangeV3.h"
#include "Structs/UnrealStruct.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Object.h"

#include "EffectContainer.generated.h"

/**
 * @class UEffectContainer
 * @brief A container class for managing and applying various effects within the application.
 *
 * The UEffectContainer class provides functionality to store,
 * manage, and apply effects, which can be visual, gameplay-related,
 * or other operations that influence the application.
 * It acts as a centralized unit for organizing and executing effect logic,
 * ensuring consistent behavior across the system.
 *
 * Responsibilities includes:
 * - Storing multiple effects.
 * - Providing interfaces to add/remove effects.
 * - Applying effects to target objects or entities.
 * - Managing effect lifecycles.
 *
 * This class is intended to be used in scenarios where effects are
 * dynamically added or removed and need to be systematically applied or processed.
 */
UCLASS(meta = (NullableEnable))
class RPGCORE_API UEffectContainer : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @function GetEffectKeys
     * @brief Retrieves a collection of keys associated with the stored effects.
     *
     * This method provides access to the keys identifying the effects
     * currently managed within the container. The keys can be used to
     * query, modify, or remove specific effects as needed.
     *
     * @return A collection of keys representing the effects stored within the container.
     */
    auto GetEffectKeys() const
    {
        return EffectData |
               ranges::views::transform([](const TPair<FName, FInstancedStruct> &Pair) { return Pair.Key; });
    }

    /**
     * @brief Retrieves the total number of effects currently stored in the container.
     *
     * This method provides the count of effects managed within the container.
     * It is useful for determining the quantity of active or available effects,
     * enabling better control or monitoring of the effect system.
     *
     * @return The number of effects currently stored in the container.
     */
    UFUNCTION(BlueprintPure, Category = "Effects")
    int32 GetEffectCount() const
    {
        return EffectData.Num();
    }

    /**
     * @class HasEffect
     * @brief Determines whether a specific effect is currently active or present.
     *
     * The HasEffect class provides functionality to check the existence
     * or activation state of a specific effect within the system. It is
     * used to verify if certain effects are applied to objects or entities,
     * aiding in conditional logic and state management for effects.
     *
     * This class is typically utilized in systems where dynamic checks
     * for effects are needed to trigger dependent behavior or interactions.
     */
    UFUNCTION(BlueprintPure, Category = "Effects")
    bool HasEffect(FName Effect) const;

    /**
     * @brief Attempts to retrieve the type of a specified effect.
     *
     * This method searches for the given `Effect` within the container
     * and attempts to retrieve the associated effect type's script structure.
     * If the effect is found, it returns the script structure corresponding to that effect.
     *
     * @param Effect The name identifier of the effect for which the type is to be retrieved.
     * @return The script structure of the effect type if found, or nullptr if the effect does not exist.
     */
    UFUNCTION(BlueprintPure, Category = "Effects", meta = (Nullable))
    const UScriptStruct *TryGetEffectType(FName Effect) const;

    /**
     * @brief Attempts to retrieve the data of a specified effect if it matches the given type.
     *
     * This method searches for the given `Effect` in the container and tries to retrieve
     * its associated data if the effect exists and its script structure matches the expected struct type.
     *
     * @param Effect The name identifier of the effect to retrieve.
     * @param Data The found data.
     * @return Whether or not the effect was found
     */
    UFUNCTION(BlueprintPure, CustomThunk, Category = "Effects", meta = (CustomStructureParam = Data))
    bool TryGetEffectData(FName Effect, int32 &Data) const;
    DECLARE_FUNCTION(execTryGetEffectData);

    /**
     * @brief Attempts to retrieve the data of a specified effect if it matches the given type.
     *
     * This method searches for the given `Effect` in the container and tries to retrieve
     * its associated data if the effect exists and its script structure matches the expected type `T`.
     *
     * @tparam T The struct type that's stored in the effect
     * @param Effect The name identifier of the effect to retrieve.
     * @return A pointer to the data of the effect as type `T` if found and valid, or nullptr otherwise.
     */
    template <UEStruct T>
    T *TryGetEffectData(const FName Effect) const
    {
        auto *Data = EffectData.Find(Effect);
        if (Data == nullptr || Data->GetScriptStruct() != GetScriptStruct<T>())
            return nullptr;

        return &Data->Get<T>();
    }

    /**
     * @brief Sets or updates the data associated with a given effect name.
     *
     * This method allows assigning or modifying effect-specific data in the container.
     * If an entry for the specified effect already exists, it may update the data depending
     * on the specified conditions. If no such entry exists, a new effect data entry is created.
     *
     * @param Effect The unique identifier (name) for the effect.
     * @param Data The data to associate with the effect.
     * @param AllowTypeChange Optional parameter indicating whether the type of the existing
     *        effect data is allowed to change. Defaults to false.
     * @return True if the data was successfully set or updated, false otherwise.
     */
    UFUNCTION(BlueprintCallable, CustomThunk, Category = "Effects", meta = (CustomStructureParam = Data))
    bool SetEffectData(FName Effect, const int32 &Data, bool AllowTypeChange = false);
    DECLARE_FUNCTION(execSetEffectData);

    /**
     * @brief Sets or updates the data associated with a given effect name.
     *
     * This method allows assigning or modifying effect-specific data in the container.
     * If an entry for the specified effect already exists, it may update the data depending
     * on the specified conditions. If no such entry exists, a new effect data entry is created.
     *
     * @tparam T The struct type that we're placing in
     * @param Effect The unique identifier (name) for the effect.
     * @param Data The data to associate with the effect.
     * @param AllowTypeChange Optional parameter indicating whether the type of the existing
     *        effect data is allowed to change. Defaults to false.
     * @return True if the data was successfully set or updated, false otherwise.
     */
    template <UEStruct T>
    bool SetEffectData(const FName Effect, T &&Data, bool AllowTypeChange = false)
    {
        if (auto *ExistingData = EffectData.Find(Effect); ExistingData != nullptr)
        {
            if (AllowTypeChange || ExistingData->GetScriptStruct() == GetScriptStruct<T>())
            {
                ExistingData->InitializeAs<std::remove_cvref_t<T>>(Forward<T>(Data));
                return true;
            }

            return false;
        }

        EffectData.Emplace(Effect, FInstancedStruct::Make<std::remove_cvref_t<T>>(Forward<T>(Data)));
        return true;
    }

    /**
     * @brief Resets the specified effect to its initial state in the container.
     *
     * This method searches for the specified effect within the container
     * and resets its associated data to its default values, restoring its
     * initial configuration. If the effect is not found, no action is taken.
     *
     * Useful for reinitializing effects that require consistent behavior
     * or need to be reused in their default state.
     *
     * @param Effect The unique identifier (FName) of the effect to be reset.
     */
    UFUNCTION(BlueprintCallable, Category = "Effects")
    void ResetEffect(FName Effect);

    /**
     * @brief Resets all effects within the effect container to their default state.
     *
     * The ResetAllEffects method iterates through all stored effects in the container
     * and resets each one to its initial or default state. This operation ensures
     * that no residual effect data or state is retained, effectively clearing and
     * initializing all active effects.
     *
     * Common scenarios for calling this method include:
     * - Preparing the system for a new session or level.
     * - Clearing all ongoing effects after a major event or state change.
     * - Ensuring the consistency of effects state during application runtime.
     */
    UFUNCTION(BlueprintCallable, Category = "Effects")
    void ResetAllEffects();

    /**
     * @brief Removes an effect from the container based on its identifier.
     *
     * This method allows for the removal of a specific effect by its name from the
     * container. It ensures that the specified effect is no longer managed or applied
     * by the container.
     *
     * It is intended for use when an effect is no longer needed or requires explicit
     * deactivation/removal from the system.
     *
     * @param Effect The name of the effect to be removed from the container.
     */
    UFUNCTION(BlueprintCallable, Category = "Effects")
    void RemoveEffect(FName Effect);

    /**
     * @brief Clears all effects stored in the container.
     *
     * The ClearEffects method removes all stored effects within the UEffectContainer,
     * resetting the container to an empty state. This operation ensures that any
     * previously applied effects are no longer managed or available for processing.
     *
     * This method is commonly used when the effects need to be entirely reset, such as
     * when transitioning states in the application or cleaning up resources.
     *
     * Responsibilities:
     * - Empties the internal effect storage.
     * - Ensures no lingering effects remain in the container post-execution.
     */
    UFUNCTION(BlueprintCallable, Category = "Effects")
    void ClearEffects();

  private:
    UPROPERTY()
    TMap<FName, FInstancedStruct> EffectData;
};
