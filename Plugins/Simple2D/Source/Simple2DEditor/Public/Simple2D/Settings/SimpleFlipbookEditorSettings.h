// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SimpleFlipbookEditorSettings.generated.h"

/**
 * 
 */
UCLASS(config=EditorPerProjectUserSettings)
class SIMPLE2DEDITOR_API USimpleFlipbookEditorSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    /**
     * Background color in the flipbook editor
     */
    UPROPERTY(config, EditAnywhere, Category=Background, meta=(HideAlphaChannel))
    FColor BackgroundColor = FColor(55, 55, 55);

    /**
     * Should the grid be shown by default when the editor is opened?
     */
    UPROPERTY(config, EditAnywhere, Category=Background)
    bool bShowGridByDefault = false;

};
