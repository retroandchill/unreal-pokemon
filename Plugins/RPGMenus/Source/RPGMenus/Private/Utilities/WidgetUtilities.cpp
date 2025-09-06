// "Unreal Pokémon" created by Retro & Chill.
#include "Utilities/WidgetUtilities.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "PaperSprite.h"

void UWidgetUtilities::SetBrushFromAsset(UImage *ImageWidget, UObject *Asset, bool MatchSize)
{
    if (auto Texture = Cast<UTexture2D>(Asset); Texture != nullptr)
    {
        ImageWidget->SetBrushFromTexture(Texture, MatchSize);
    }
    else if (auto Material = Cast<UMaterialInterface>(Asset); Material != nullptr)
    {
        ImageWidget->SetBrushFromMaterial(Material);
    }
    else if (Asset != nullptr && Asset->Implements<USlateTextureAtlasInterface>())
    {
        ImageWidget->SetBrushFromAtlasInterface(Asset, MatchSize);
    }
    else
    {
        ImageWidget->SetBrushFromAsset(nullptr);
    }
}

void UWidgetUtilities::SetTextStyleOverride(URichTextBlock *TextBlock, TSubclassOf<UCommonTextStyle> Style)
{
    if (Style == nullptr)
    {
        TextBlock->ClearAllDefaultStyleOverrides();
        return;
    }

    auto DefaultObject = Style.GetDefaultObject();
    FTextBlockStyle DefaultStyle;
    DefaultObject->ToTextBlockStyle(DefaultStyle);
    TextBlock->SetDefaultTextStyle(DefaultStyle);
}