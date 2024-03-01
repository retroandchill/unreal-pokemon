// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Actions/Action.h"
#include "UObject/Object.h"
#include "MenuAction.generated.h"

class UScreen;

/**
 * Action for when the player tries to pull up a menu and add it to the stack.
 */
UCLASS(Blueprintable, EditInlineNew)
class UNREALPOKEMON_API UMenuAction : public UAction {
	GENERATED_BODY()

public:
	void PerformAction_Implementation(UObject* Owner) override;

private:
	/**
	 * The class for the menu to bring up.
	 */
	UPROPERTY(EditAnywhere, Category = Menus)
	TSubclassOf<UScreen> MenuClass;
};
