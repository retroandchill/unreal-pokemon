// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Components/SimpleFlipbookComponent.h"
#include "ContentStreaming.h"
#include "Engine/CollisionProfile.h"
#include "Misc/MapErrors.h"
#include "Misc/UObjectToken.h"
#include "Net/UnrealNetwork.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "PhysicsEngine/BodySetup.h"
#include "Simple2D.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Simple2D/Rendering/SimpleFlipbookSceneProxy.h"

DECLARE_CYCLE_STAT(TEXT("Tick Simple Flipbook"), STAT_TickSimpleFlipbook, STATGROUP_Simple2D);

USimpleFlipbookComponent::USimpleFlipbookComponent()
{
    Super::SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

    CastShadow = false;
    bUseAsOccluder = false;
    bCanEverAffectNavigation = false;

    Mobility = EComponentMobility::Movable;
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_DuringPhysics;
    bTickInEditor = true;
}

void USimpleFlipbookComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USimpleFlipbookComponent, SourceFlipbook);
}

bool USimpleFlipbookComponent::SetFlipbook(USimpleFlipbook *NewFlipbook)
{
    if (NewFlipbook != SourceFlipbook)
    {
        // Don't allow changing the sprite if we are "static".
        if (auto ComponentOwner = GetOwner(); ComponentOwner == nullptr || AreDynamicDataChangesAllowed())
        {
            SourceFlipbook = NewFlipbook;

            // We need to also reset the frame and time also
            AccumulatedTime = 0.0f;
            CalculateCurrentFrame();

            // Need to send this to render thread at some point
            MarkRenderStateDirty();

            // Update physics representation right away
            FlipbookChangedPhysicsState();

            // Update this component streaming data.
            IStreamingManager::Get().NotifyPrimitiveUpdated(this);

            // Since we have new mesh, we need to update bounds
            UpdateBounds();

            return true;
        }
    }

    return false;
}

USimpleFlipbook *USimpleFlipbookComponent::GetFlipbook()
{
    return SourceFlipbook;
}

void USimpleFlipbookComponent::SetSpriteColor(FLinearColor NewColor)
{
    // Can't set color on a static component
    if (AreDynamicDataChangesAllowed() && (SpriteColor != NewColor))
    {
        SpriteColor = NewColor;

        MarkRenderDynamicDataDirty();
    }
}

void USimpleFlipbookComponent::Play()
{
    Activate();
    bReversePlayback = false;
    bPlaying = true;
}

void USimpleFlipbookComponent::PlayFromStart()
{
    SetPlaybackPosition(0.0f);
    Play();
}

void USimpleFlipbookComponent::Reverse()
{
    Activate();
    bReversePlayback = true;
    bPlaying = true;
}

void USimpleFlipbookComponent::ReverseFromEnd()
{
    SetPlaybackPosition(GetFlipbookLength());
    Reverse();
}

void USimpleFlipbookComponent::Stop()
{
    bPlaying = false;
}

bool USimpleFlipbookComponent::IsPlaying() const
{
    return bPlaying;
}

bool USimpleFlipbookComponent::IsReversing() const
{
    return bPlaying && bReversePlayback;
}

void USimpleFlipbookComponent::SetPlaybackPositionInFrames(int32 NewFramePosition)
{
    const float Framerate = GetFlipbookFramerate();
    const float NewTime = (Framerate > 0.0f) ? (static_cast<float>(NewFramePosition) / Framerate) : 0.0f;
    SetPlaybackPosition(NewTime);
}

int32 USimpleFlipbookComponent::GetPlaybackPositionInFrames() const
{
    const float Framerate = GetFlipbookFramerate();
    if (const int32 NumFrames = GetFlipbookLengthInFrames(); NumFrames > 0)
    {
        return FMath::Clamp<int32>(FMath::TruncToInt(AccumulatedTime * Framerate), 0, NumFrames - 1);
    }

    return 0;
}

void USimpleFlipbookComponent::SetPlaybackPosition(float NewPosition)
{
    float OldPosition = AccumulatedTime;
    AccumulatedTime = NewPosition;

    if (OldPosition != AccumulatedTime)
    {
        CalculateCurrentFrame();
    }
}

float USimpleFlipbookComponent::GetPlaybackPosition() const
{
    return AccumulatedTime;
}

void USimpleFlipbookComponent::SetLooping(bool bNewLooping)
{
    bLooping = bNewLooping;
}

bool USimpleFlipbookComponent::IsLooping() const
{
    return bLooping;
}

void USimpleFlipbookComponent::SetPlayRate(float NewRate)
{
    PlayRate = NewRate;
}

float USimpleFlipbookComponent::GetPlayRate() const
{
    return PlayRate;
}

void USimpleFlipbookComponent::SetNewTime(float NewTime)
{
    SetPlaybackPosition(NewTime);
}

float USimpleFlipbookComponent::GetFlipbookLength() const
{
    return SourceFlipbook != nullptr ? SourceFlipbook->GetTotalDuration() : 0.0f;
}

int32 USimpleFlipbookComponent::GetFlipbookLengthInFrames() const
{
    return SourceFlipbook != nullptr ? SourceFlipbook->GetNumFrames() : 0;
}

float USimpleFlipbookComponent::GetFlipbookFramerate() const
{
    return SourceFlipbook != nullptr ? SourceFlipbook->GetFramesPerSecond() : 15.0f;
}

void USimpleFlipbookComponent::OnRep_SourceFlipbook(USimpleFlipbook *OldFlipbook)
{
    if (OldFlipbook != SourceFlipbook)
    {
        // Force SetFlipbook to change the animation (by default it won't change)
        USimpleFlipbook *NewFlipbook = SourceFlipbook;
        SourceFlipbook = nullptr;

        SetFlipbook(NewFlipbook);
    }
}

void USimpleFlipbookComponent::CalculateCurrentFrame()
{
    const int32 LastCachedFrame = CachedFrameIndex;
    CachedFrameIndex =
        (SourceFlipbook != nullptr) ? SourceFlipbook->GetKeyFrameIndexAtTime(AccumulatedTime) : INDEX_NONE;

    if (CachedFrameIndex != LastCachedFrame)
    {
        // Update children transforms in case we have anything attached to an animated socket
        UpdateChildTransforms();

        // Indicate we need to send new dynamic data.
        MarkRenderDynamicDataDirty();
    }
}

void USimpleFlipbookComponent::TickFlipbook(float DeltaTime)
{
    bool bIsFinished = false;

    if (bPlaying)
    {
        ProcessPlaying(DeltaTime, bIsFinished);
    }

    // Notify user that the flipbook finished playing
    if (bIsFinished)
    {
        OnFinishedPlaying.Broadcast();
    }
}

void USimpleFlipbookComponent::ProcessPlaying(float DeltaTime, bool &bIsFinished)
{
    const float TimelineLength = GetFlipbookLength();
    const float EffectiveDeltaTime = DeltaTime * (bReversePlayback ? -PlayRate : PlayRate);

    float NewPosition = AccumulatedTime + EffectiveDeltaTime;

    if (EffectiveDeltaTime > 0.0f)
    {
        if (NewPosition > TimelineLength)
        {
            if (bLooping)
            {
                // If looping, play to end, jump to start, and set target to somewhere near the beginning.
                SetPlaybackPosition(TimelineLength);
                SetPlaybackPosition(0.0f);

                if (TimelineLength > 0.0f)
                {
                    while (NewPosition > TimelineLength)
                    {
                        NewPosition -= TimelineLength;
                    }
                }
                else
                {
                    NewPosition = 0.0f;
                }
            }
            else
            {
                // If not looping, snap to end and stop playing.
                NewPosition = TimelineLength;
                Stop();
                bIsFinished = true;
            }
        }
    }
    else
    {
        if (NewPosition < 0.0f)
        {
            if (bLooping)
            {
                // If looping, play to start, jump to end, and set target to somewhere near the end.
                SetPlaybackPosition(0.0f);
                SetPlaybackPosition(TimelineLength);

                if (TimelineLength > 0.0f)
                {
                    while (NewPosition < 0.0f)
                    {
                        NewPosition += TimelineLength;
                    }
                }
                else
                {
                    NewPosition = 0.0f;
                }
            }
            else
            {
                // If not looping, snap to start and stop playing.
                NewPosition = 0.0f;
                Stop();
                bIsFinished = true;
            }
        }
    }

    SetPlaybackPosition(NewPosition);
}

void USimpleFlipbookComponent::FlipbookChangedPhysicsState()
{
    // If the frame has changed and we're using animated collision, we need to recreate that state as well
    RecreatePhysicsState();

    // We just totally changed the physics setup so update overlaps too
    UpdateOverlaps();
}

void USimpleFlipbookComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                             FActorComponentTickFunction *ThisTickFunction)
{
    SCOPE_CYCLE_COUNTER(STAT_TickSimpleFlipbook)

    // Advance time
    TickFlipbook(DeltaTime);

    // Update the frame and push it to the renderer if necessary
    CalculateCurrentFrame();
}

void USimpleFlipbookComponent::SendRenderDynamicData_Concurrent()
{
    if (SceneProxy != nullptr)
    {
        auto DrawCall = SourceFlipbook != nullptr ? SourceFlipbook->CreateDrawCallRecord(CachedFrameIndex)
                                                  : FSimpleFlipbookDrawCall();
        DrawCall.Color = SpriteColor.ToFColor(false);

        auto InSceneProxy = static_cast<Simple2D::FSimpleFlipbookSceneProxy *>(SceneProxy);
        ENQUEUE_RENDER_COMMAND(FSendPaperRenderComponentDynamicData)
        ([InSceneProxy, DrawCall](FRHICommandListImmediate &) { InSceneProxy->SetFlipbookBounds(DrawCall); });
    }
}

const UObject *USimpleFlipbookComponent::AdditionalStatObject() const
{
    return SourceFlipbook;
}

#if WITH_EDITOR
void USimpleFlipbookComponent::CheckForErrors()
{
    Super::CheckForErrors();

    AActor *Owner = GetOwner();

    for (int32 MaterialIndex = 0; MaterialIndex < GetNumMaterials(); ++MaterialIndex)
    {
        if (auto Material = GetMaterial(MaterialIndex); Material != nullptr && !Material->IsTwoSided())
        {
            FMessageLog("MapCheck")
                .Warning()
                ->AddToken(FUObjectToken::Create(Owner))
                ->AddToken(
                    FTextToken::Create(NSLOCTEXT("Simple2D", "MapCheck_Message_SimpleFlipbookMaterialNotTwoSided",
                                                 "The material applied to the flipbook component is not marked as "
                                                 "two-sided, which may cause lighting artifacts.")))
                ->AddToken(FUObjectToken::Create(Material))
                ->AddToken(FMapErrorToken::Create(FName(TEXT("PaperFlipbookMaterialNotTwoSided"))));
        }
    }
}
#endif

FPrimitiveSceneProxy *USimpleFlipbookComponent::CreateSceneProxy()
{
    auto NewProxy = MakeUnique<Simple2D::FSimpleFlipbookSceneProxy>(this);

    CalculateCurrentFrame();

    auto DrawCall = SourceFlipbook != nullptr && SourceFlipbook->IsValidKeyFrameIndex(CachedFrameIndex)
                        ? SourceFlipbook->CreateDrawCallRecord(CachedFrameIndex)
                        : FSimpleFlipbookDrawCall();
    DrawCall.Color = SpriteColor.ToFColor(false);

    auto InSceneProxy = NewProxy.Get();
    ENQUEUE_RENDER_COMMAND(FCreatePaperFlipbookProxy_SetSprite)
    ([InSceneProxy, DrawCall](FRHICommandListImmediate &) { InSceneProxy->SetFlipbookBounds(DrawCall); });
    return NewProxy.Release();
}

FBoxSphereBounds USimpleFlipbookComponent::CalcBounds(const FTransform &LocalToWorld) const
{
    if (SourceFlipbook != nullptr)
    {
        // Graphics bounds.
        FBoxSphereBounds NewBounds = SourceFlipbook->GetRenderBounds().TransformBy(LocalToWorld);

        // Apply bounds scale
        NewBounds.BoxExtent *= BoundsScale;
        NewBounds.SphereRadius *= BoundsScale;

        return NewBounds;
    }

    return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector::ZeroVector, 0.f);
}

void USimpleFlipbookComponent::GetUsedTextures(TArray<UTexture *> &OutTextures,
                                               EMaterialQualityLevel::Type QualityLevel)
{
    // Get the texture referenced by each keyframe
    if (SourceFlipbook != nullptr)
    {
        if (auto *SourceTexture = SourceFlipbook->GetSourceTexture(); SourceTexture != nullptr)
        {
            OutTextures.AddUnique(SourceTexture);
        }
    }

    // Get any textures referenced by our materials
    Super::GetUsedTextures(OutTextures, QualityLevel);
}

UMaterialInterface *USimpleFlipbookComponent::GetMaterial(int32 MaterialIndex) const
{
    if (OverrideMaterials.IsValidIndex(MaterialIndex) && (OverrideMaterials[MaterialIndex] != nullptr))
    {
        return OverrideMaterials[MaterialIndex];
    }
    if (SourceFlipbook != nullptr)
    {
        return SourceFlipbook->GetDefaultMaterial();
    }

    return nullptr;
}

int32 USimpleFlipbookComponent::GetNumMaterials() const
{
    return FMath::Max<int32>(OverrideMaterials.Num(), 1);
}