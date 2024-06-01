// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Screens/Screen.h"
#include "GameFramework/PlayerController.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "RPGMenusSubsystem.generated.h"

/**
 * Subsystem governing the menu stack used by the game.
 */
UCLASS(DisplayName = "RPG Menus Subsystem")
class RPGMENUS_API URPGMenusSubsystem : public ULocalPlayerSubsystem {
    GENERATED_BODY()

public:
    /**
     * Add a screen of the given class to the stack
     * @tparam T The screen class to spawn
     * @param ScreenClass The actually class object to spawn
     * @return The created screen.
     */
    template <typename T>
        requires std::is_base_of_v<UScreen, T>
    T *AddScreenToStack(TSubclassOf<T> ScreenClass = T::StaticClass()) {
        auto PlayerController = GetPlayerController();
        auto Screen = CreateWidget<T>(PlayerController, ScreenClass);
        Screen->AddToViewport();
        ScreenStack.Add(Screen);

        if (ScreenStack.Num() == 1) {
            PlayerController->SetInputMode(FInputModeUIOnly());
            PlayerController->SetShowMouseCursor(true);
        }
        Screen->GiveMenuFocus();

        return Screen;
    }

    /**
     * Conditionally add the given screen class to the stack, if it is not already the class of the one at the top.
     * If that is the case, then simply return that instead.
     * @tparam T The screen class to spawn
     * @param ScreenClass The actually class object to spawn
     * @return The created screen
     */
    template <typename T>
        requires std::is_base_of_v<UScreen, T>
    T *ConditionallyAddScreenToStack(TSubclassOf<T> ScreenClass = T::StaticClass()) {
        if (!ScreenStack.IsEmpty() && ScreenStack.Last()->IsA(ScreenClass)) {
            return CastChecked<T>(ScreenStack.Last());
        }

        return AddScreenToStack(ScreenClass);
    }

    /**
     * Helper function used to create a screen on the stack from Blueprints
     * @param WorldContextObject The world context object needed to get the controller
     * @param ScreenType The screen class to spawn
     * @return The created screen.
     */
    UFUNCTION(BlueprintCallable, BlueprintCosmetic,
              meta = (WorldContext = "WorldContextObject", DisplayName = "Add Screen to Stack",
                      BlueprintInternalUseOnly = "true"),
              Category = "Widget")
    static UScreen *AddScreenToStackHelper(UObject *WorldContextObject, TSubclassOf<UScreen> ScreenType);

    /**
     * Get the screen at the top of the stack and cast it to the supplied type
     * @tparam T The type to check for
     * @return The top widget on the stack (if the types match), otherwise nullptr
     */
    template <typename T = UScreen>
        requires std::is_base_of_v<UScreen, T>
    T *GetTopOfStack() const {
        if (ScreenStack.IsEmpty())
            return nullptr;

        return Cast<T>(ScreenStack.Last());
    }

    /**
     * Get the screen at the top of the stack and cast it to the supplied type
     * @tparam T The type to check for
     * @return The top widget on the stack, otherwise nullptr
     */
    UFUNCTION(BlueprintPure, Category = "Widget")
    UScreen *GetTopScreenOfStack() const;

    /**
     * Remove the current screen from the stack
     * @return The new top of the stack
     */
    UScreen *RemoveScreenFromStack();

    /**
     * Remove the current screen from the stack
     * @param WorldContextObject The world context object needed to get the controller
     * @return The screen to remove from the stack
     */
    UFUNCTION(BlueprintCallable, Category = "Widget", meta = (WorldContext = "WorldContextObject"))
    static UScreen *RemoveScreenFromStack(UObject *WorldContextObject);

private:
    /**
     * Get the player controller that this subsystem underpins
     * @return The player controller that owns this subsystem
     */
    APlayerController* GetPlayerController() const;

    /**
     * Get the subystem from the given world context object
     * @param WorldContextObject The object used to extract the world (and utlimately the player)
     * @return The subsystem (if found)
     */
    static URPGMenusSubsystem* GetSubsystem(UObject* WorldContextObject);
    
    /**
     * The internal stack of screens used to handle the input.
     */
    UPROPERTY()
    TArray<TObjectPtr<UScreen>> ScreenStack;
    
};
