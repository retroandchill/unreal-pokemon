// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonLazyImage.h"
#include "Images/ImageAsset.h"
#include "Simple2D/Components/FlipbookTicker.h"

#include "EnhancedImage.generated.h"

class SPaperFlipbookWidget;

// Event for a non-looping flipbook finishing play
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlipbookFinishedPlaying);

/**
 *
 */
UCLASS()
class INTERACTIVEUI_API UEnhancedImage : public UCommonLazyImage, public FTickableGameObject
{
    GENERATED_BODY()

  public:
    UEnhancedImage();

    void SetBrush(const FSlateBrush &InBrush) override;

  private:
    void SetBrush(const FSlateBrush &InBrush, bool bUpdateAssetImage);

  public:
    void SetBrushFromAsset(USlateBrushAsset *Asset) override;
    void SetBrushFromTexture(UTexture2D *Texture, bool bMatchSize = false) override;
    void SetBrushFromTextureDynamic(UTexture2DDynamic *Texture, bool bMatchSize = false) override;
    void SetBrushFromMaterial(UMaterialInterface *Material) override;
    void SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion,
                                    bool bMatchSize = false) override;

    UFUNCTION(BlueprintCallable, Category = Image)
    virtual void SetBrushFromPaperFlipbook(UPaperFlipbook *Flipbook, bool bMatchSize = false);

    UFUNCTION(BlueprintCallable, Category = Image)
    virtual void SetBrushFromSimpleFlipbook(USimpleFlipbook *Flipbook, bool bMatchSize = false);

    UFUNCTION(BlueprintCallable, Category = LazyImage)
    FVoidCoroutine SetBrushFromLazyPaperFlipbook(const TSoftObjectPtr<UPaperFlipbook> &LazyFlipbook,
                                                 bool bMatchSize = false,
                                                 FForceLatentCoroutine ForceLatent = FForceLatentCoroutine());

    UFUNCTION(BlueprintCallable, Category = LazyImage)
    FVoidCoroutine SetBrushFromLazySimpleFlipbook(const TSoftObjectPtr<USimpleFlipbook> &LazyFlipbook,
                                                  bool bMatchSize = false,
                                                  FForceLatentCoroutine ForceLatent = FForceLatentCoroutine());

    void Tick(float DeltaTime) override;
    TStatId GetStatId() const override;
    bool IsTickable() const override;
    bool IsTickableInEditor() const override;

  protected:
    void SynchronizeProperties() override;

#if WITH_EDITOR
    void PostLoad() override;
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

  private:
    void SetSourceImageInternal(UObject *Object);

    void CreateSimpleFlipbookMaterialInstance(const USimpleFlipbook &Flipbook);

    void OnFlipbookFinishedPlaying() const;

    void OnFrameIndexChanged(std::any KeyFrame);

    /**
     * Event called whenever a non-looping flipbook finishes playing (either reaching the beginning or the end,
     * depending on the play direction)
     */
    UPROPERTY(BlueprintAssignable, Category = "PaperFlipbook|Widget|Event")
    FOnFlipbookFinishedPlaying OnFinishedPlaying;

    // clang-format off
    UPROPERTY(
        EditAnywhere, Category = Appearance,
        meta = (DisplayThumbnail = "true",
                AllowedClasses =
                    "/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface,/Script/Paper2D.PaperFlipbook,/Script/Simple2D.SimpleFlipbook",
                DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
    TObjectPtr<UObject> SourceImage;
    // clang-format on

    /** Are we currently playing . */
    UPROPERTY(Category = Appearance, EditAnywhere)
    uint8 bAutoPlay : 1 = true;

    /** Current play rate of the flipbook */
    UPROPERTY(Category = Appearance, EditAnywhere)
    float PlayRate = 1.f;

    /** Whether the flipbook should loop when it reaches the end, or stop */
    UPROPERTY(Category = Appearance, EditAnywhere)
    uint8 bLooping : 1 = true;

    UPROPERTY(Category = Appearance, EditAnywhere)
    uint8 bManualSize : 1 = false;

    Simple2D::FFlipbookTicker FlipbookTicker;

    UPROPERTY(Category = Appearance, EditAnywhere)
    TObjectPtr<UMaterialInterface> SimpleFlipbookBaseMaterial;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> SimpleFlipbookMaterialInstance;
};
