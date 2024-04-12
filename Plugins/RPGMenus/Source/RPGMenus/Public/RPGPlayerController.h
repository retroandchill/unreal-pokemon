// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Screens/Screen.h"
#include "RPGPlayerController.generated.h"

struct FInputActionInstance;
class UScreen;
class USelectableWidget;
class UInputAction;
class UInputMappingContext;

/**
 * Player controller responsible for RPG menu related actions and behaviors.
 */
UCLASS(Blueprintable)
class RPGMENUS_API ARPGPlayerController : public APlayerController {
    GENERATED_BODY()

  public:
    explicit ARPGPlayerController(const FObjectInitializer &ObjectInitializer);

  protected:
    void BeginPlay() override;

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
        auto Screen = CreateWidget<T>(this, ScreenClass);
        Screen->AddToViewport();
        ScreenStack.Add(Screen);

        if (ScreenStack.Num() == 1) {
            SetInputMode(FInputModeUIOnly());
            SetShowMouseCursor(true);
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
     * The internal stack of screens used to handle the input.
     */
    UPROPERTY()
    TArray<TObjectPtr<UScreen>> ScreenStack;
};
