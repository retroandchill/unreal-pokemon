// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "SimpleFlipbookComponent.generated.h"


class USimpleFlipbook;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFlipbookFinishedPlaying);

UCLASS(ShowCategories=(Mobility, ComponentReplication), ClassGroup=Simple2D, meta=(BlueprintSpawnableComponent))
class SIMPLE2D_API USimpleFlipbookComponent : public UMeshComponent {
    GENERATED_BODY()

public:
    USimpleFlipbookComponent();
    
    /** Change the flipbook used by this instance (will reset the play time to 0 if it is a new flipbook). */
    UFUNCTION(BlueprintCallable, Category="Sprite")
    virtual bool SetFlipbook(USimpleFlipbook *NewFlipbook);

    /** Gets the flipbook used by this instance. */
    UFUNCTION(BlueprintPure, Category="Sprite")
    virtual USimpleFlipbook *GetFlipbook();

    /** Returns the current color of the sprite */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    FLinearColor GetSpriteColor() const {
        return SpriteColor;
    }

    /** Set color of the sprite */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetSpriteColor(FLinearColor NewColor);

    /** Start playback of flipbook */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void Play();

    /** Start playback of flipbook from the start */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void PlayFromStart();

    /** Start playback of flipbook in reverse */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void Reverse();

    /** Start playback of flipbook in reverse from the end */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void ReverseFromEnd();

    /** Stop playback of flipbook */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void Stop();

    /** Get whether this flipbook is playing or not. */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    bool IsPlaying() const;

    /** Get whether we are reversing or not */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    bool IsReversing() const;

    /** Jump to a position in the flipbook (expressed in frames). If bFireEvents is true, event functions will fire, otherwise they will not. */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void SetPlaybackPositionInFrames(int32 NewFramePosition, bool bFireEvents);

    /** Get the current playback position (in frames) of the flipbook */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    int32 GetPlaybackPositionInFrames() const;

    /** Jump to a position in the flipbook (expressed in seconds). If bFireEvents is true, event functions will fire, otherwise they will not. */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void SetPlaybackPosition(float NewPosition, bool bFireEvents);

    /** Get the current playback position (in seconds) of the flipbook */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    float GetPlaybackPosition() const;

    /** true means we should loop, false means we should not. */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void SetLooping(bool bNewLooping);

    /** Get whether we are looping or not */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    bool IsLooping() const;

    /** Sets the new play rate for this flipbook */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void SetPlayRate(float NewRate);

    /** Get the current play rate for this flipbook */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    float GetPlayRate() const;

    /** Set the new playback position time to use */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    void SetNewTime(float NewTime);

    /** Get length of the flipbook (in seconds) */
    UFUNCTION(BlueprintCallable, Category="Components|Flipbook")
    float GetFlipbookLength() const;

    /** Get length of the flipbook (in frames) */
    UFUNCTION(BlueprintCallable, Category = "Components|Flipbook")
    int32 GetFlipbookLengthInFrames() const;

    /** Get the nominal framerate that the flipbook will be played back at (ignoring PlayRate), in frames per second */
    UFUNCTION(BlueprintCallable, Category = "Components|Flipbook")
    float GetFlipbookFramerate() const;

protected:
    UFUNCTION()
    void OnRep_SourceFlipbook(USimpleFlipbook *OldFlipbook);

    void CalculateCurrentFrame();

    void TickFlipbook(float DeltaTime);
    void FlipbookChangedPhysicsState();

private:
    //disable parallel add to scene for paper2d
    void CreateRenderState_Concurrent(FRegisterComponentContext *Context) override {
        Super::CreateRenderState_Concurrent(nullptr);
    }

public:
    void TickComponent(float DeltaTime, enum ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;
    void SendRenderDynamicData_Concurrent() override;
    const UObject *AdditionalStatObject() const override;
#if WITH_EDITOR
    void CheckForErrors() override;
#endif

    FPrimitiveSceneProxy *CreateSceneProxy() override;
    FBoxSphereBounds CalcBounds(const FTransform &LocalToWorld) const override;
    void GetUsedTextures(TArray<UTexture *> &OutTextures, EMaterialQualityLevel::Type QualityLevel) override;
    UMaterialInterface *GetMaterial(int32 MaterialIndex) const override;
    int32 GetNumMaterials() const override;

private:
    /** Flipbook currently being played */
    UPROPERTY(Category=Sprite, EditAnywhere, meta=(DisplayThumbnail = "true"), ReplicatedUsing=OnRep_SourceFlipbook)
    TObjectPtr<USimpleFlipbook> SourceFlipbook;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInterface> BaseMaterial;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;

    /** Current play rate of the flipbook */
    UPROPERTY(Category=Sprite, EditAnywhere)
    float PlayRate = 1.f;

    /** Whether the flipbook should loop when it reaches the end, or stop */
    UPROPERTY()
    uint32 bLooping : 1 = true;

    /** If playback should move the current position backwards instead of forwards */
    UPROPERTY()
    uint32 bReversePlayback : 1 = false;

    /** Are we currently playing (moving Position) */
    UPROPERTY()
    uint32 bPlaying : 1 = true;

    /** Current position in the timeline */
    UPROPERTY()
    float AccumulatedTime = 0.0f;

    /** Last frame index calculated */
    UPROPERTY()
    int32 CachedFrameIndex = INDEX_NONE;

    /** Vertex color to apply to the frames */
    UPROPERTY(BlueprintGetter = GetSpriteColor, BlueprintSetter = SetSpriteColor, Interp, Category=Sprite)
    FLinearColor SpriteColor = FLinearColor::White;

    /** Event called whenever a non-looping flipbook finishes playing (either reaching the beginning or the end, depending on the play direction) */
    UPROPERTY(BlueprintAssignable)
    FFlipbookFinishedPlaying OnFinishedPlaying;

};