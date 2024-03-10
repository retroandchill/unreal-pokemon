// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "CursorDirection.h"
#include "UObject/Object.h"
#include "SelectionInputs.generated.h"

/**
 * The input data used for selection in the menus.
 */
UCLASS(Blueprintable, EditInlineNew)
class RPGMENUS_API USelectionInputs : public UDataAsset {
	GENERATED_BODY()

public:
	/**
	 * Parse the provided key input and determine what direction to move the cursor (if at all)
	 * @param Key The key that was pressed
	 * @return The direction to move the cursor (if applicable)
	 */
	TOptional<ECursorDirection> ParseDirectionalInputs(const FKey& Key) const;

	/**
	 * Is the provided input the confirm button
	 * @param Key The key that was pressed
	 * @return Is this a confirm input?
	 */
	bool IsConfirmInput(const FKey& Key) const;

	/**
	 * Is the provided input the cancel button
	 * @param Key The key that was pressed
	 * @return Is this a cancel input?
	 */
	bool IsCancelInput(const FKey& Key) const;

private:
	/**
	 * The Up Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Navigation")
	TSet<FKey> UpInputs;

	/**
	 * The Down Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Navigation")
	TSet<FKey> DownInputs;

	/**
	 * The Left Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Navigation")
	TSet<FKey> LeftInputs;

	/**
	 * The Right Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Navigation")
	TSet<FKey> RightInputs;

	/**
	 * The Confirm Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Options")
	TSet<FKey> ConfirmInputs;

	/**
	 * The Cancel Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Options")
	TSet<FKey> CancelInputs;
};
