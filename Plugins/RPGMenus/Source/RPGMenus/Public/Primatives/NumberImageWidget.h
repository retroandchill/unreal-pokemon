// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"

#include "NumberImageWidget.generated.h"

/**
 * Widget that can display an unsigned integer as a set of images.
 */
UCLASS()
class RPGMENUS_API UNumberImageWidget : public UWidget {
    GENERATED_BODY()

  public:
    UNumberImageWidget();

    void SetNumber(uint32 NewNumber);

  protected:
    TSharedRef<SWidget> RebuildWidget() override;
    void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
    void PostEditChangeChainProperty(FPropertyChangedChainEvent &PropertyChangedEvent) override;
#endif

  private:
    void RefreshNumber();
    
    /**
     * The number to display in the widget.
     */
    UPROPERTY(EditAnywhere, Category = Number)
    uint32 Number = 0;

    /**
     * The images for the numbers that are being edited
     */
    UPROPERTY(EditAnywhere, EditFixedSize, Category = Number, meta = (EditFixedOrder))
    TArray<FSlateBrush> NumberImages;

    /**
     * The box that serves as the widget contents
     */
    TSharedPtr<SHorizontalBox> ImageBox;

    /**
     * The actual images held by this widget
     */
    TArray<TSharedPtr<SImage>> Images;
};
