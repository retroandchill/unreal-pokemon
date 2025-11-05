// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Windowskin.generated.h"

/**
 * Represents a valid Windowskin the the editor
 */
UCLASS(BlueprintType, HideCategories = (Object))
class RPGMENUS_API UWindowskin : public UObject {
    GENERATED_BODY()

  public:
    /**
     * Get the source texture for the window
     * @return The source texture used to make the window
     */
    UTexture2D *GetSourceTexture() const;

    /**
     * Get the margins for the window
     * @return The size of the margins for the window in pixel offset from the edges
     */
    const FMargin &GetMargins() const;

  private:
    /**
     * The source texture used to make the window
     */
    UPROPERTY(EditAnywhere, Category = Windowskin)
    TObjectPtr<UTexture2D> SourceTexture;

    /**
     * The size of the margins for the window in pixel offset from the edges
     */
    UPROPERTY(EditAnywhere, Category = Windowskin)
    FMargin Margins;
};
