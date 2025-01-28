// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookDetailsCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "MaterialExpressionSpriteTextureSampler.h"
#include "PropertyCustomizationHelpers.h"
#include "Simple2D/Assets/SimpleFlipbook.h"

namespace Simple2D {

    TSharedRef<IDetailCustomization> FSimpleFlipbookDetailsCustomization::MakeInstance() {
        return MakeShared<FSimpleFlipbookDetailsCustomization>();
    }

    void FSimpleFlipbookDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) {
        auto &SpriteCategory = DetailBuilder.EditCategory("Sprite", FText::GetEmpty(), ECategoryPriority::Important);

        // Show other normal properties in the sprite category so that desired ordering doesn't get messed up
        BuildTextureSection(SpriteCategory, DetailBuilder);
        SpriteCategory.AddProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, Rows));
        SpriteCategory.AddProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, Columns));
        SpriteCategory.AddProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, FramesPerSecond));
        SpriteCategory.AddProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, KeyFrames));

        SpriteCategory.AddProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, DefaultMaterial));
        SpriteCategory.AddProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, PixelsPerUnrealUnit));

        // Show/hide the custom pivot point based on the pivot mode
        auto PivotModeProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, PivotMode));
        auto CustomPivotPointProperty =
            DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, CustomPivotPoint));
        auto CustomPivotPointVisibility =
            TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateStatic(
                &FSimpleFlipbookDetailsCustomization::GetCustomPivotVisibility, PivotModeProperty.ToSharedPtr()));
        SpriteCategory.AddProperty(PivotModeProperty);
        SpriteCategory.AddProperty(CustomPivotPointProperty).Visibility(CustomPivotPointVisibility);
    }

    void FSimpleFlipbookDetailsCustomization::ExtractSpriteSamplerLabels(FText &SourceTextureOverrideLabel,
                                                                         class UMaterial *DefaultMaterial) {
        // Get a list of sprite samplers
        TArray<const UMaterialExpressionSpriteTextureSampler *> SpriteSamplerExpressions;
        DefaultMaterial->GetAllExpressionsOfType(SpriteSamplerExpressions);

        // Turn that into a set of labels
        for (const UMaterialExpressionSpriteTextureSampler *Sampler : SpriteSamplerExpressions) {
            if (Sampler->SlotDisplayName.IsEmpty()) {
                continue;
            }

            if (Sampler->bSampleAdditionalTextures) {
                AdditionalTextureLabels.FindOrAdd(Sampler->AdditionalSlotIndex) = Sampler->SlotDisplayName;
            } else {
                SourceTextureOverrideLabel = Sampler->SlotDisplayName;
            }
        }
    }

    void FSimpleFlipbookDetailsCustomization::BuildTextureSection(IDetailCategoryBuilder &SpriteCategory,
                                                                  IDetailLayoutBuilder &DetailLayout) {
        // Grab information about the material
        auto DefaultMaterialProperty =
            DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, DefaultMaterial));

        FText SourceTextureOverrideLabel;
        if (UObject * DefaultMaterialAsObject;
            DefaultMaterialProperty->GetValue(DefaultMaterialAsObject) == FPropertyAccess::Success) {
            if (auto DefaultMaterialInterface = Cast<UMaterialInterface>(DefaultMaterialAsObject);
                DefaultMaterialInterface != nullptr) {
                if (auto DefaultMaterial = DefaultMaterialInterface->GetMaterial(); DefaultMaterial != nullptr) {
                    ExtractSpriteSamplerLabels(SourceTextureOverrideLabel, DefaultMaterial);
                }
            }
        }

        // Create the base texture widget
        auto SourceTextureProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, SourceTexture));
        DetailLayout.HideProperty(SourceTextureProperty);
        // clang-format off
        SpriteCategory.AddCustomRow(SourceTextureProperty->GetPropertyDisplayName())
            .NameContent()
            [
                CreateTextureNameWidget(SourceTextureProperty, SourceTextureOverrideLabel)
            ]
            .ValueContent()
            .MaxDesiredWidth(TOptional<float>())
            [
                SourceTextureProperty->CreatePropertyValueWidget()
            ];
        // clang-format on

        // Create the additional textures widget
        auto AdditionalSourceTexturesProperty =
            DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleFlipbook, AdditionalSourceTextures));
        auto AdditionalSourceTexturesBuilder = MakeShared<FDetailArrayBuilder>(AdditionalSourceTexturesProperty);
        AdditionalSourceTexturesBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(
            this, &FSimpleFlipbookDetailsCustomization::GenerateAdditionalTextureWidget));
        SpriteCategory.AddCustomBuilder(AdditionalSourceTexturesBuilder);
    }

    void FSimpleFlipbookDetailsCustomization::GenerateAdditionalTextureWidget(
        TSharedRef<IPropertyHandle> PropertyHandle, int32 ArrayIndex, IDetailChildrenBuilder &ChildrenBuilder) {
        auto &TextureRow = ChildrenBuilder.AddProperty(PropertyHandle);

        FText ExtraText;
        if (const auto *ExtraTextPtr = AdditionalTextureLabels.Find(ArrayIndex); ExtraTextPtr != nullptr) {
            ExtraText = *ExtraTextPtr;
        }

        FNumberFormattingOptions NoCommas;
        NoCommas.UseGrouping = false;
        auto SlotDesc = FText::Format(NSLOCTEXT("Simple2D", "AdditionalTextureSlotIndex", "Slot #{0}"),
                                      FText::AsNumber(ArrayIndex, &NoCommas));

        TextureRow.DisplayName(SlotDesc);

        TextureRow.ShowPropertyButtons(false);

        // clang-format off
        TextureRow.CustomWidget(false)
            .NameContent()
            [
                CreateTextureNameWidget(PropertyHandle, ExtraText)
            ]
            .ValueContent()
            .MaxDesiredWidth(TOptional<float>())
            [
                PropertyHandle->CreatePropertyValueWidget()
            ];
        // clang-format on
    }

    TSharedRef<SWidget>
    FSimpleFlipbookDetailsCustomization::CreateTextureNameWidget(TSharedPtr<IPropertyHandle> PropertyHandle,
                                                                 const FText &OverrideText) {
        auto PropertyNameWidget = PropertyHandle->CreatePropertyNameWidget();
        if (OverrideText.IsEmpty()) {
            return PropertyNameWidget;
        }

        // clang-format off
        return SNew(SVerticalBox)
            +SVerticalBox::Slot()
            [
                PropertyNameWidget
            ]
            +SVerticalBox::Slot()
            .Padding(FMargin(0.0f, 4.0f, 0.0f, 0.0f))
            [
                SNew(STextBlock)
                .Font(IDetailLayoutBuilder::GetDetailFont())
                .Text(OverrideText)
            ];
        // clang-format on
    }

    EVisibility FSimpleFlipbookDetailsCustomization::GetCustomPivotVisibility(TSharedPtr<IPropertyHandle> Property) {
        if (Property.IsValid()) {
            uint8 ValueAsByte;
            if (auto Result = Property->GetValue(ValueAsByte); Result == FPropertyAccess::Success) {
                return static_cast<ESpritePivotMode::Type>(ValueAsByte) == ESpritePivotMode::Custom
                           ? EVisibility::Visible
                           : EVisibility::Collapsed;
            }
        }

        // If there are multiple values, show all properties
        return EVisibility::Visible;
    }
} // namespace Simple2D