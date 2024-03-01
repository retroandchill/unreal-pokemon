// "Unreal Pokémon" created by Retro & Chill.
#pragma once


#include "CoreMinimal.h"
#include "Screens/Screen.h"
#include "PokemonSelectScreen.generated.h"

class UCommandWindow;
class UHelpWindow;
class UPokemonSelectionPane;
/**
 * Screen for when the player needs to select a Pokémon from the menu
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UPokemonSelectScreen : public UScreen {
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

private:
	/**
	 * Callback for when a Pokémon in selected
	 * @param Index The index that was selected
	 */
	UFUNCTION()
	void OnPokemonSelected(int32 Index);

	/**
	 * Display the area the command window is contained in
	 * @param bIsVisible Should the window be shown
	 */
	void ToggleCommandWindowVisibility(bool bIsVisible);
	
	/**
	 * The "Window" that the player selects a Pokémon from
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPokemonSelectionPane> SelectionPane;

	/**
	 * The window that contains the command to call on a Pokémon
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommandWindow> CommandWindow;

	/**
	 * The help window used for when the commands are shown.
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHelpWindow> CommandHelpWindow;
};
