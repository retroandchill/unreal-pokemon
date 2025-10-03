// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonLazyImage.h"
#include "Simple2D/Components/FlipbookTicker.h"

#include "EnhancedImage.generated.h"

class USimpleFlipbook;
class UPaperFlipbook;
class SPaperFlipbookWidget;

// Event for a non-looping flipbook finishing play
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlipbookFinishedPlaying);

/**
 * Represents an enhanced dynamic image widget that supports a variety of brush sources such as textures,
 * materials, and flipbooks. Also implements tickable behavior for managing animated content.
 */
UCLASS()
class INTERACTIVEUI_API UEnhancedImage : public UCommonLazyImage, public FTickableGameObject
{
    GENERATED_BODY()

  public:
    /**
     * Initializes a new instance of the UEnhancedImage class. Sets up internal references for materials,
     * delegates for flipbook events, and initializes the flipbook ticker system for handling animated content.
     *
     * @return A constructed instance of the UEnhancedImage class with default configurations.
     */
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

    /**
     * Sets the brush resource for the enhanced image using the specified Paper Flipbook.
     * The first frame of the flipbook is used as the brush image, and the flipbook ticker
     * system is updated to support animated content rendering.
     *
     * @param Flipbook The Paper Flipbook to use as the brush source. If nullptr, the brush will be cleared.
     * @param bMatchSize Whether to adjust the image widget size to match the dimensions of the flipbook's sprite.
     */
    UFUNCTION(BlueprintCallable, Category = Image)
    virtual void SetBrushFromPaperFlipbook(UPaperFlipbook *Flipbook, bool bMatchSize = false);

    /**
     * Updates the brush resource of the enhanced image to use the specified simple flipbook.
     * Allows dynamic representation of flipbook animations and, optionally, updates the widget size to match the
     * flipbook's sprite dimensions.
     *
     * @param Flipbook The simple flipbook to set as the brush resource. If nullptr, clears the brush and stops the
     * flipbook animation.
     * @param bMatchSize When true, adjusts the widget's size to match the sprite dimensions of the flipbook.
     */
    UFUNCTION(BlueprintCallable, Category = Image)
    virtual void SetBrushFromSimpleFlipbook(USimpleFlipbook *Flipbook, bool bMatchSize = false);

    /**
     * Updates the enhanced image's brush to utilize a lazily-loaded Paper Flipbook asset.
     * Once the flipbook asset is asynchronously loaded, the first frame of the flipbook is used as the brush,
     * and the flipbook ticker system is updated to support animated rendering.
     *
     * @param LazyFlipbook A soft reference to a Paper Flipbook asset to be used as the source for the brush.
     * If null, the brush will be cleared.
     * @param bMatchSize Indicates whether to resize the image widget to match the dimensions of the flipbook's sprite.
     */
    UFUNCTION(BlueprintCallable, Category = LazyImage)
    void SetBrushFromLazyPaperFlipbook(const TSoftObjectPtr<UPaperFlipbook> &LazyFlipbook, bool bMatchSize = false);

    /**
     * Asynchronously sets the brush resource for the enhanced image using a lazily-loaded simple flipbook.
     * The flipbook will be loaded at runtime, and its first frame will be used as the brush image.
     * Additionally, supports resizing the widget to match the sprite dimensions of the flipbook.
     *
     * @param LazyFlipbook A soft reference to a simple flipbook asset to be loaded and used as the brush. If null, the
     * brush will be cleared.
     * @param bMatchSize Indicates whether to resize the image widget to match the dimensions of the flipbook's sprite.
     */
    UFUNCTION(BlueprintCallable, Category = LazyImage)
    void SetBrushFromLazySimpleFlipbook(const TSoftObjectPtr<USimpleFlipbook> &LazyFlipbook, bool bMatchSize = false);

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

    /**
     * Defines the source object used to render the image for the enhanced widget.
     * Supports a variety of asset classes, including textures, material interfaces,
     * texture atlases, and flipbooks, enabling both static and animated content rendering.
     *
     * Allowed asset classes include:
     * - Texture
     * - Material Interface
     * - Slate Texture Atlas Interface
     * - Paper Flipbook
     * - Simple 2D Flipbook
     *
     * Media Texture assets are disallowed to ensure compatibility with the rendering system.
     *
     * Display thumbnails are enabled for easy preview of the selected asset in the editor.
     */
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

    /**
     * Defines the base material used for rendering a simple flipbook animation. This material serves as the foundation
     * for visualizing flipbook sprites and supports customization through material interface properties.
     */
    UPROPERTY(Category = Appearance, EditAnywhere)
    TObjectPtr<UMaterialInterface> SimpleFlipbookBaseMaterial;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> SimpleFlipbookMaterialInstance;
};
