// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Window.generated.h"

class SConstraintCanvas;
class UWindowskin;
/**
 * Basic Window Widget class
 */
UCLASS(BlueprintType, Category = Windows)
class RPGMENUS_API UWindow : public UWidget {
	GENERATED_BODY()

public:
	explicit UWindow(const FObjectInitializer& ObjectInitializer);
	TSharedRef<SWidget> RebuildWidget() override;
	void SynchronizeProperties() override;

private:
	/**
	 * The windowskin asset used to create the window
	 */
	UPROPERTY(EditAnywhere, Category = Windowskin)
	TObjectPtr<UWindowskin> Windowskin;

	/**
	 * The source texture used to make the window
	 */
	FSlateBrush Brush;
};
