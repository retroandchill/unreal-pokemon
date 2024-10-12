// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonLazyImage.h"
#include "Images/ImageAsset.h"

#include "EnhancedImage.generated.h"

class SPaperFlipbookWidget;

// Event for a non-looping flipbook finishing play
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlipbookFinishedPlaying);

/**
 * 
 */
UCLASS()
class RPGMENUS_API UEnhancedImage : public UCommonLazyImage {
    GENERATED_BODY()

public:
    void SetBrush(const FSlateBrush& InBrush) override;
    
private:
    void SetBrush(const FSlateBrush& InBrush, bool bUpdateAssetImage);

public:
    void SetBrushFromAsset(USlateBrushAsset* Asset) override;
    void SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize = false) override;
    void SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize = false) override;
    void SetBrushFromMaterial(UMaterialInterface* Material) override;
    void SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion, bool bMatchSize = false) override;

    UFUNCTION(BlueprintCallable, Category = Image)
    virtual void SetBrushFromPaperFlipbook(UPaperFlipbook* Flipbook, bool bMatchSize = false);

    UFUNCTION(BlueprintCallable, Category = Image)
    virtual void SetBrushFromImageAsset(const FImageAsset& ImageAsset, bool bMatchSize = false);

    UFUNCTION(BlueprintCallable, Category = LazyImage)
    void SetBrushFromLazyPaperFlipbook(const TSoftObjectPtr<UPaperFlipbook>& LazyFlipbook, bool bMatchSize = false);

    UFUNCTION(BlueprintCallable, Category = LazyImage)
    void SetBrushFromLazyImageAsset(const FSoftImageAsset& LazyImage, bool bMatchSize = false);

protected:
    TSharedRef<SWidget> RebuildImageWidget() override;
    void SynchronizeProperties() override;
    void ReleaseSlateResources(bool bReleaseChildren) override;
    
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

private:
    void SetSourceImageInternal(UObject* Object);

    int32 GetWidgetIndex() const;

    void OnFlipbookFinishedPlaying() const;

    /**
     * Event called whenever a non-looping flipbook finishes playing (either reaching the beginning or the end,
     * depending on the play direction)
     */
    UPROPERTY(BlueprintAssignable, Category = "PaperFlipbook|Widget|Event")
    FOnFlipbookFinishedPlaying OnFinishedPlaying;
    
    UPROPERTY(EditAnywhere, Category = Appearance)
    FImageAsset SourceImage;

    /** Are we currently playing . */
    UPROPERTY(Category = Appearance, EditAnywhere)
    bool bAutoPlay = true;

    /** Current play rate of the flipbook */
    UPROPERTY(Category = Appearance, EditAnywhere)
    float PlayRate = 1.f;

    /** Whether the flipbook should loop when it reaches the end, or stop */
    UPROPERTY(Category = Appearance, EditAnywhere)
    bool bLooping = true;
    
    UPROPERTY(Category = Appearance, EditAnywhere)
    bool bManualSize = false;

    TSharedPtr<SWidgetSwitcher> Switcher;
    TSharedPtr<SPaperFlipbookWidget> FlipbookWidget;
};
