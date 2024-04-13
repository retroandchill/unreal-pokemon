// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

#include "Command.generated.h"

class ARPGPlayerController;
/**
 * The struct that holds the command information
 */
UCLASS(Blueprintable, EditInlineNew)
class RPGMENUS_API UCommand : public UObject {
    GENERATED_BODY()

  public:
    /**
     * Get the internal ID of the command
     * @return The unique ID given to the command
     */
    UFUNCTION(BlueprintPure, Category = Commands)
    FName GetID() const;

    /**
     * Get the displayed text for the command
     * @return The text displayed to the user
     */
    UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Commands)
    FText GetText() const;

    /**
     * Get if this command should actually be added to the window
     * @return Will this command be displayed?
     */
    UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Commands)
    bool IsEnabled() const;

    /**
     * Create a basic command using the supplied text
     * @param Text The text to display. Is normalized to serve as the name symbol.
     * @return The created command
     */
    static UCommand *CreateBasicCommand(const FText &Text);

    /**
     * Create a basic command using the supplied id, text, and handler
     * @param ID The ID of the command
     * @param Text The text to display. Is normalized to serve as the name symbol.
     * @param Handler The handler object
     * @return The created command
     */
    static UCommand *CreateBasicCommand(FName ID, const FText &Text, UObject *Handler = nullptr);

    /**
     * Get the handler object for the command.
     * @tparam T The expected type of the handler
     * @return The handler (if exists and the types match, otherwise nullptr)
     */
    template <typename T>
    T *GetHandler() const {
        return Cast<T>(Handler);
    }

    /**
     * Execute any special effects that this command may have associated with it
     * @param Controller The player controller used to execute any processing
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Events)
    void ExecuteCommand(ARPGPlayerController *Controller);

  protected:
    /**
     * Get the original text without any additional formatting brought about by subclasses
     * @return The text displayed to the user for the command in question
     */
    UFUNCTION(BlueprintPure, Category = Commands)
    FText GetOriginalText() const;

  private:
    /**
     * The unique ID given to the command
     */
    UPROPERTY(EditAnywhere, Category = Commands)
    FName ID;

    /**
     * The text displayed to the user for the command in question
     */
    UPROPERTY(EditAnywhere, Category = Commands)
    FText Text;

    /**
     * The handler object associated with this class
     */
    UPROPERTY()
    TObjectPtr<UObject> Handler;
};
