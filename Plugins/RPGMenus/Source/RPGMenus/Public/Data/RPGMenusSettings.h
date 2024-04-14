// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "RPGMenusSettings.generated.h"

/**
 * The settings class for the RPG Menus module.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "RPG Menus Settings"))
class RPGMENUS_API URPGMenusSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Load and return the cursor sound asset.
     * @return The default sound played when the player moves the cursor in a window.
     */
    UFUNCTION(BlueprintPure, Category = "Settings|Sound")
    USoundBase *GetCursorSound() const;

    /**
     * Load and return the confirm sound asset.
     * @return The default sound played when the player confirms a choice in a window.
     */
    UFUNCTION(BlueprintPure, Category = "Settings|Sound")
    USoundBase *GetConfirmSound() const;

    /**
     * Load and return the cancel sound asset.
     * @return The default sound played when the player cancels on selection in a window.
     */
    UFUNCTION(BlueprintPure, Category = "Settings|Sound")
    USoundBase *GetCancelSound() const;

  private:
    /**
     * The default sound played when the player moves the cursor in a window.
     */
    UPROPERTY(EditAnywhere, Config, Category = "Sound", meta = (AllowedClasses = "SoundBase"))
    FSoftObjectPath CursorSound;

    /**
     * The default sound played when the player confirms a choice in a window.
     */
    UPROPERTY(EditAnywhere, Config, Category = "Sound", meta = (AllowedClasses = "SoundBase"))
    FSoftObjectPath ConfirmSound;

    /**
     * The default sound played when the player cancels on selection in a window.
     */
    UPROPERTY(EditAnywhere, Config, Category = "Sound", meta = (AllowedClasses = "SoundBase"))
    FSoftObjectPath CancelSound;
};
