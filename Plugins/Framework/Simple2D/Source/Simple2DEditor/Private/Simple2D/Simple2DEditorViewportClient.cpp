// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Simple2DEditorViewportClient.h"
#include "CanvasTypes.h"
#include "ImageUtils.h"
#include "Paper2DModule.h"

namespace Simple2D
{

    FSimple2DEditorViewportClient::FSimple2DEditorViewportClient(
        const TWeakPtr<SEditorViewport> &InEditorViewportWidget)
        : FEditorViewportClient(nullptr, nullptr, InEditorViewportWidget)
    {
        const bool XX = FMath::IsNearlyEqual(PaperAxisX.X, static_cast<FVector::FReal>(1.0f));
        const bool XY = FMath::IsNearlyEqual(PaperAxisX.Y, static_cast<FVector::FReal>(1.0f));
        const bool YY = FMath::IsNearlyEqual(PaperAxisY.Y, static_cast<FVector::FReal>(1.0f));
        const bool YZ = FMath::IsNearlyEqual(PaperAxisY.Z, static_cast<FVector::FReal>(1.0f));

        ELevelViewportType NewViewportType = LVT_OrthoXZ;
        if (XX && YY)
        {
            NewViewportType = LVT_OrthoXY;
        }
        else if (XX && YZ)
        {
            NewViewportType = LVT_OrthoXZ;
        }
        else if (XY && YZ)
        {
            NewViewportType = LVT_OrthoYZ;
        }
        else
        {
            //@TODO: Unsupported axes
        }
        SetViewModes(VMI_Lit, VMI_Lit);
        FSimple2DEditorViewportClient::SetViewportType(NewViewportType);

        SetInitialViewTransform(LVT_Perspective, -100.0f * PaperAxisZ, PaperAxisZ.Rotation(), DEFAULT_ORTHOZOOM);
    }

    void FSimple2DEditorViewportClient::Tick(float DeltaSeconds)
    {
        if (auto Size = Viewport->GetSizeXY(); bDeferZoomToSprite && (Size.X > 0) && (Size.Y > 0))
        {
            FBox BoundsToFocus = GetDesiredFocusBounds();
            if (ViewportType != LVT_Perspective)
            {
                TGuardValue SaveViewportType(ViewportType, LVT_Perspective);
                FocusViewportOnBox(BoundsToFocus, bDeferZoomToSpriteIsInstant);
            }

            FocusViewportOnBox(BoundsToFocus, bDeferZoomToSpriteIsInstant);
            bDeferZoomToSprite = false;
        }

        FEditorViewportClient::Tick(DeltaSeconds);
    }

    void FSimple2DEditorViewportClient::AddReferencedObjects(FReferenceCollector &Collector)
    {
        FEditorViewportClient::AddReferencedObjects(Collector);
        Collector.AddReferencedObject(CheckerboardTexture);
    }

    void FSimple2DEditorViewportClient::RequestFocusOnSelection(bool bInstant)
    {
        bDeferZoomToSprite = true;
        bDeferZoomToSpriteIsInstant = bInstant;
    }

    void FSimple2DEditorViewportClient::ModifyCheckerboardTextureColors()
    {
        constexpr FColor ColorOne = FColor(128, 128, 128);
        constexpr FColor ColorTwo = FColor(64, 64, 64);
        constexpr int32 CheckerSize = 32;

        DestroyCheckerboardTexture();
        SetupCheckerboardTexture(ColorOne, ColorTwo, CheckerSize);
    }

    void FSimple2DEditorViewportClient::SetupCheckerboardTexture(const FColor &ColorOne, const FColor &ColorTwo,
                                                                 int32 CheckerSize)
    {
        if (CheckerboardTexture == nullptr)
        {
            CheckerboardTexture = FImageUtils::CreateCheckerboardTexture(ColorOne, ColorTwo, CheckerSize);
        }
    }

    void FSimple2DEditorViewportClient::DestroyCheckerboardTexture()
    {
        if (CheckerboardTexture)
        {
            if (CheckerboardTexture->GetResource())
            {
                CheckerboardTexture->ReleaseResource();
            }

            CheckerboardTexture->MarkAsGarbage();
            CheckerboardTexture = nullptr;
        }
    }

    void FSimple2DEditorViewportClient::DrawSelectionRectangles(FViewport *, FCanvas *Canvas)
    {
        for (auto &[TopLeft, Dimensions, Color] : SelectionRectangles)
        {
            const float X = (TopLeft.X - ZoomPos.X) * ZoomAmount;
            const float Y = (TopLeft.Y - ZoomPos.Y) * ZoomAmount;
            const float W = Dimensions.X * ZoomAmount;
            const float H = Dimensions.Y * ZoomAmount;
            constexpr bool bAlphaBlend = true;

            Canvas->DrawTile(X, Y, W, H, 0, 0, 1, 1, Color, GWhiteTexture, bAlphaBlend);
        }
    }
} // namespace Simple2D
