// "Unreal Pokémon" created by Retro & Chill.

#pragma once
#include "IDetailCustomization.h"

class IDetailCategoryBuilder;

namespace Simple2D
{
    /**
     *
     */
    class SIMPLE2DEDITOR_API FSimpleFlipbookDetailsCustomization : public IDetailCustomization
    {

      public:
        static TSharedRef<IDetailCustomization> MakeInstance();

        void CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) override;

      private:
        void ExtractSpriteSamplerLabels(FText &SourceTextureOverrideLabel, UMaterial *DefaultMaterial);
        void BuildTextureSection(IDetailCategoryBuilder &SpriteCategory, IDetailLayoutBuilder &DetailLayout);
        void GenerateAdditionalTextureWidget(TSharedRef<IPropertyHandle> PropertyHandle, int32 ArrayIndex,
                                             IDetailChildrenBuilder &ChildrenBuilder);
        static TSharedRef<SWidget> CreateTextureNameWidget(TSharedPtr<IPropertyHandle> PropertyHandle,
                                                           const FText &OverrideText);

        static EVisibility GetCustomPivotVisibility(TSharedPtr<IPropertyHandle> Property);

        TMap<int32, FText> AdditionalTextureLabels;
    };
} // namespace Simple2D
