#include "TileBitsetCustomization.h"
#include "DetailWidgetRow.h"
#include "EditorStyleSet.h"
#include "IDetailChildrenBuilder.h"
#include "TileSet.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/Anchors.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "GridMapEditor"

class SGridTilePreviewWidget : public SCompoundWidget {
  public:
    SLATE_BEGIN_ARGS(SGridTilePreviewWidget) {
    }
    SLATE_END_ARGS()

    void Construct(const FArguments &InArgs, TSharedRef<IPropertyHandle> StructPropertyHandle, uint32 bit,
                   FAnchors Anchors) {
        ChildSlot
            [SNew(SButton)
                 .ButtonStyle(FAppStyle::Get(), "SimpleSharpButton")
                 .ButtonColorAndOpacity(FLinearColor(FColor(40, 40, 40)))
                 .OnClicked(this, &SGridTilePreviewWidget::OnTileClicked, StructPropertyHandle, bit)
                 .ContentPadding(FMargin(2.0f, 2.0f))
                     [SNew(SVerticalBox)

                      + SVerticalBox::Slot().AutoHeight()
                            [SNew(SBorder)
                                 .BorderImage(FAppStyle::GetBrush("UMGEditor.AnchorGrid"))
                                 .Padding(
                                     0)[SNew(SBox)
                                            .WidthOverride(32)
                                            .HeightOverride(32)
                                            .HAlign(HAlign_Center)
                                            .VAlign(VAlign_Center)[SNew(SBox).WidthOverride(32).HeightOverride(
                                                32)[SNew(SBorder).Padding(
                                                1)[SNew(SConstraintCanvas)

                                                   + SConstraintCanvas::Slot()
                                                         .Anchors(Anchors)
                                                         .Offset(FMargin(0, 0, Anchors.IsStretchedHorizontal() ? 0 : 15,
                                                                         Anchors.IsStretchedVertical() ? 0 : 15))
                                                         .Alignment(FVector2D(
                                                             Anchors.IsStretchedHorizontal() ? 0 : Anchors.Minimum.X,
                                                             Anchors.IsStretchedVertical() ? 0 : Anchors.Minimum.Y))
                                                             [SNew(SImage)
                                                                  .Visibility(
                                                                      this, &SGridTilePreviewWidget::GetVisibility_Tile,
                                                                      StructPropertyHandle, bit)
                                                                  .Image(FAppStyle::Get().GetBrush(
                                                                      "UMGEditor.AnchoredWidget"))]]]]]]]];
    }

  private:
    EVisibility GetVisibility_Tile(TSharedRef<IPropertyHandle> PropertyHandle, uint32 bit) const {
        uint32 currentBits = GetCurrentBits(PropertyHandle);
        if (currentBits & bit)
            return EVisibility::Visible;
        return EVisibility::Hidden;
    }

    FReply OnTileClicked(TSharedRef<IPropertyHandle> StructPropertyHandle, uint32 bit) {
        uint32 currentBits = GetCurrentBits(StructPropertyHandle);

        FString bitString;
        StructPropertyHandle->GetValueAsFormattedString(bitString);

        // is it already set?
        if ((currentBits & bit) == bit) { // remove it
            currentBits ^= bit;
        }
        // otherwise add it
        else {
            currentBits |= bit;
        }

        // update the property
        SetCurrentBits(StructPropertyHandle, currentBits);
        ensure(GetCurrentBits(StructPropertyHandle) == currentBits);

        return FReply::Handled();
    }

    uint32 GetCurrentBits(TSharedRef<IPropertyHandle> PropertyHandle) const {
        TArray<void *> StructPtrs;
        PropertyHandle->AccessRawData(StructPtrs);

        if (StructPtrs.Num() > 0) {
            FGridMapTileBitset *SelectedBitset = (FGridMapTileBitset *)StructPtrs[0];
            if (SelectedBitset) {
                return SelectedBitset->Bitset;
            }
        }

        return 0;
    }

    void SetCurrentBits(TSharedRef<IPropertyHandle> PropertyHandle, uint32 NewBits) {
        FString bitString = FString::Printf(TEXT("(Bitset=%u)"), NewBits);
        PropertyHandle->SetValueFromFormattedString(bitString);
    }
};

TSharedRef<IPropertyTypeCustomization> FTileBitsetCustomization::MakeInstance() {
    return MakeShareable(new FTileBitsetCustomization);
}

void FTileBitsetCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
                                               FDetailWidgetRow &HeaderRow,
                                               IPropertyTypeCustomizationUtils &StructCustomizationUtils) {
    // header
    HeaderRow
        .NameContent()[StructPropertyHandle->CreatePropertyNameWidget()]
        // value
        .ValueContent()[SNew(SBorder)
                            .BorderImage(FAppStyle::GetBrush("WhiteBrush"))
                            .BorderBackgroundColor(FLinearColor(FColor(66, 139, 202)))
                            .Padding(0)[SNew(SVerticalBox) +
                                        SVerticalBox::Slot()
                                            .AutoHeight()[SNew(SUniformGridPanel)

                                                          // Top Row
                                                          + SUniformGridPanel::Slot(0, 0) // top left
                                                                [SNew(SGridTilePreviewWidget, StructPropertyHandle,
                                                                      1 << 4, FAnchors(0.5f, 0.5f, 0.5f, 0.5f))]

                                                          + SUniformGridPanel::Slot(1, 0) // top center
                                                                [SNew(SGridTilePreviewWidget, StructPropertyHandle,
                                                                      1 << 0, FAnchors(0.5f, 0.5f, 0.5f, 0.5f))]

                                                          + SUniformGridPanel::Slot(2, 0) // top right
                                                                [SNew(SGridTilePreviewWidget, StructPropertyHandle,
                                                                      1 << 5, FAnchors(0.5f, 0.5f, 0.5f, 0.5f))]

                                                          // Center Row
                                                          + SUniformGridPanel::Slot(0, 1) // center left
                                                                [SNew(SGridTilePreviewWidget, StructPropertyHandle,
                                                                      1 << 1, FAnchors(0.5f, 0.5f, 0.5f, 0.5f))]

                                                          /* Nothing in the center
                                                          + SUniformGridPanel::Slot(1, 1) // center center
                                                          [
                                                              SNew(SAnchorPreviewWidget, AnchorsHandle, AlignmentHandle,
                                                          OffsetsHandle, LOCTEXT("CenterCenter", "Center/Center"),
                                                          FAnchors(0.5, 0.5, 0.5, 0.5))
                                                          ]
                                                          */

                                                          + SUniformGridPanel::Slot(2, 1) // center right
                                                                [SNew(SGridTilePreviewWidget, StructPropertyHandle,
                                                                      1 << 2, FAnchors(0.5f, 0.5f, 0.5f, 0.5f))]

                                                          // Bottom Row
                                                          + SUniformGridPanel::Slot(0, 2) // bottom left
                                                                [SNew(SGridTilePreviewWidget, StructPropertyHandle,
                                                                      1 << 6, FAnchors(0.5f, 0.5f, 0.5f, 0.5f))]

                                                          + SUniformGridPanel::Slot(1, 2) // bottom center
                                                                [SNew(SGridTilePreviewWidget, StructPropertyHandle,
                                                                      1 << 3, FAnchors(0.5f, 0.5f, 0.5f, 0.5f))]

                                                          + SUniformGridPanel::Slot(2, 2) // bottom right
                                                                [SNew(SGridTilePreviewWidget, StructPropertyHandle,
                                                                      1 << 7, FAnchors(0.5f, 0.5f, 0.5f, 0.5f))]]]];
}

void FTileBitsetCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
                                                 IDetailChildrenBuilder &ChildBuilder,
                                                 IPropertyTypeCustomizationUtils &StructCustomizationUtils) {
}

#undef LOCTEXT_NAMESPACE