// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MenuHandler.generated.h"

/**
 * Abstract parent class of all menu handlers, regardless of type
 */
UCLASS(Blueprintable, Abstract, EditInlineNew)
class POKEMONUI_API UMenuHandler : public UObject {
    GENERATED_BODY()

public:
    /**
     * Get the ID of the command
     * @return The ID of the command
     */
    FName GetID() const;

    /**
     * Get the text displayed to the player
     * @return The text displayed to the player
     */
    const FText &GetText() const;

private:
    /**
     * The ID of the command
     */
    UPROPERTY(EditAnywhere, Category = "Menu Handlers")
    FName ID;

    /**
     * The text displayed to the player
     */
    UPROPERTY(EditAnywhere, Category = "Menu Handlers")
    FText Text;

};