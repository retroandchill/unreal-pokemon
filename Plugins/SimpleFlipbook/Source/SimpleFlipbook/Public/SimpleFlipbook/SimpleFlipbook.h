// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "UObject/Object.h"
#include "SimpleFlipbook.generated.h"

class UPaperSprite;

/**
 * Represents a keyframe in a flipbook animation.
 */
USTRUCT()
struct FSimpleFlipbookKeyFrame {
    GENERATED_BODY()

    /**
     * Specifies the number of frames that will be displayed consecutively in a flipbook animation for a particular keyframe.
     */
    UPROPERTY()
    int32 FrameRun;

    /**
     * Index of the sprite within an animation keyframe in a flipbook.
     */
    UPROPERTY()
    int32 SpriteIndex;
};

/**
 * @brief Stripped down version of UPaperFlipbook that uses a simple spritesheet texture to get the reference data.
 *
 * This class is designed to be a lightweight version of the UPaperFlipbook class, utilizing a single reference sprite
 * in order to get the necessary render geometry needed for the component.
 */
UCLASS()
class SIMPLEFLIPBOOK_API USimpleFlipbook : public UObject {
    GENERATED_BODY()

public:
    /**
     * Default constructor for the USimpleFlipbook class.
     * Initializes the DefaultMaterial with a specified masked unlit sprite material.
     *
     * @return A new instance of the USimpleFlipbook class with the DefaultMaterial configured.
     */
    USimpleFlipbook();

    /**
     * Retrieves the source texture associated with the sprite.
     *
     * @return A pointer to the UTexture2D object representing the source texture.
     */
    UFUNCTION(BlueprintPure, Category = Sprite)
    UTexture2D *GetSourceTexture() const {
        return SourceTexture;
    }

    /**
     * Retrieves the number of rows in the flipbook.
     *
     * @return The number of rows in the flipbook as an integer.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetRows() const {
        return Rows;
    }

    /**
     * Retrieves the number of columns in the flipbook.
     *
     * @return The number of columns in the flipbook as an integer.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    int32 GetColumns() const {
        return Columns;
    }

    /**
     * Retrieves the frames per second for the flipbook animation.
     *
     * @return The frames per second as a float.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    float GetFramesPerSecond() const {
        return FramesPerSecond;
    }

    /**
     * Retrieves the default material associated with the flipbook.
     *
     * @return A pointer to the UMaterialInterface object representing the default material.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UMaterialInterface *GetDefaultMaterial() const {
        return DefaultMaterial;
    }

    /**
     * Retrieves the reference sprite associated with the flipbook.
     *
     * @return A pointer to the UPaperSprite object representing the reference sprite.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UPaperSprite *GetReferenceSprite() const {
        return ReferenceSprite;
    }

    /**
     * Retrieves the collision source mode for the flipbook.
     *
     * @return The collision source mode as a TEnumAsByte<EFlipbookCollisionMode::Type>.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    TEnumAsByte<EFlipbookCollisionMode::Type> GetCollisionSource() const {
        return CollisionSource;
    }

    /**
     * Retrieves the total number of frames in the flipbook animation.
     *
     * @return The total number of frames as an integer.
     */
    UFUNCTION(BlueprintPure, Category = Sprite)
    int32 GetNumFrames() const;

    /**
     * Calculates the total duration of the flipbook animation.
     *
     * @return The total duration of the flipbook animation as a float. Returns 0.0 if FramesPerSecond is 0.
     */
    UFUNCTION(BlueprintPure, Category = Sprite)
    float GetTotalDuration() const;

    /**
     * Retrieves the index of the keyframe at the specified time in the flipbook animation.
     *
     * @param Time The time in the animation for which to retrieve the keyframe index.
     * @param bClampToEnds Whether to clamp the index to the first or last keyframe if the time is out of bounds.
     * @return The index of the keyframe at the specified time, or INDEX_NONE if the time is out of bounds and not clamped.
     */
    UFUNCTION(BlueprintPure, Category = Sprite)
    int32 GetKeyFrameIndexAtTime(float Time, bool bClampToEnds = false) const;

    /**
     * Computes and retrieves the render bounds for the flipbook.
     *
     * @return A FBoxSphereBounds object representing the render bounds of the flipbook.
     */
    FBoxSphereBounds GetRenderBounds() const;

    /**
     * Gets the number of keyframes in the flipbook animation.
     *
     * @return The number of keyframes as an integer.
     */
    UFUNCTION(BlueprintCallable, Category="Sprite")
    int32 GetNumKeyFrames() const {
        return KeyFrames.Num();
    }

    /**
     * Checks whether the provided index is a valid keyframe index.
     *
     * @param Index The index of the keyframe to check.
     * @return true if the index is valid; otherwise, false.
     */
    UFUNCTION(BlueprintCallable, Category="Sprite")
    bool IsValidKeyFrameIndex(int32 Index) const {
        return KeyFrames.IsValidIndex(Index);
    }

    /**
     * Retrieves the keyframe at the specified index, ensuring the index is within valid bounds.
     *
     * @param Index The zero-based index of the keyframe to retrieve.
     * @return A reference to the keyframe at the specified index.
     */
    const FSimpleFlipbookKeyFrame &GetKeyFrameChecked(int32 Index) const {
        return KeyFrames[Index];
    }

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

private:
    /**
     * Holds a reference to the source texture used by the sprite.
     * This is a UTexture2D object that can be accessed via the GetSourceTexture function.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetSourceTexture, Category = Sprite)
    TObjectPtr<UTexture2D> SourceTexture;

    /**
     * Specifies the number of rows in the flipbook sprite sheet.
     * The minimum value is clamped to 1 both in code and in the UI.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetRows, Category = Sprite, meta = (ClampMin = 1, UIMin = 1))
    int32 Rows = 1;

    /**
     * Defines the number of columns in the flipbook sprite sheet.
     * The minimum number of columns is clamped to 1 both in the code and in the UI.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetColumns, Category = Sprite, meta = (ClampMin = 1, UIMin = 1))
    int32 Columns = 1;

    /**
     * Defines the frame rate for a sprite animation.
     * Can be edited anywhere in the editor with a range from 0 to 1000 frames per second.
     * Default value is set to 15 frames per second.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetFramesPerSecond, Category = Sprite,
        meta = (ClampMin = 0, ClampMax = 1000))
    float FramesPerSecond = 15.f;

    /**
     * Specifies the default material used for the sprite.
     * This material can be modified from within the editor and accessed through a blueprint getter.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDefaultMaterial, Category = Sprite)
    TObjectPtr<UMaterialInterface> DefaultMaterial;

    /**
     * A reference to a UPaperSprite used within the Sprite category.
     * Editable anywhere and accessible through the GetReferenceSprite blueprint getter.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetReferenceSprite, Category = Sprite)
    TObjectPtr<UPaperSprite> ReferenceSprite;

    /**
     * Specifies the collision mode for the flipbook animation.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetCollisionSource, Category = Sprite,
        meta = (InvalidEnumValues = EachFrameCollision))
    TEnumAsByte<EFlipbookCollisionMode::Type> CollisionSource = EFlipbookCollisionMode::FirstFrameCollision;

    /**
     * Array of keyframes used in a sprite animation flipbook.
     * The keyframes dictate the changes in the sprite at specific times.
     */
    UPROPERTY(EditAnywhere, Category = Sprite)
    TArray<FSimpleFlipbookKeyFrame> KeyFrames;
};