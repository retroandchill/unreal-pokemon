// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextCommand.generated.h"

class UTextBlock;

/**
 * Delegate for when the user presses confirm
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProcessCommand);


/**
 * Basic text command widget used for handling an option in a menu
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UTextCommand : public UUserWidget {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the window
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit UTextCommand(const FObjectInitializer& ObjectInitializer);

	TSharedRef<SWidget> RebuildWidget() override;

	/**
	 * Select this command and execute the bound delegate
	 */
	void Select();

private:
	/**
	 * The displayed text widget to the player
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayTextWidget;

	/**
	 * The text of the command in the menu
	 */
	UPROPERTY(EditAnywhere, Category = "Command")
	FText DisplayText;

	/**
	 * The delegate bound to when the command is selected
	 */
	UPROPERTY(BlueprintAssignable, Category = "Selection|Confirm")
	FProcessCommand CommandDelegate;
};
