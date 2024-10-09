// "Unreal Pokémon" created by Retro & Chill.


#include "Components/EnhancedImage.h"
#include "PaperSprite.h"
#include "SPaperFlipbookWidget.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

constexpr int32 ImageWidgetIndex = 0;
constexpr int32 PaperFlipbookWidgetIndex = 1;

void UEnhancedImage::SetBrush(const FSlateBrush &InBrush) {
    Super::SetBrush(InBrush);
    SetSourceImageInternal(InBrush.GetResourceObject());
}

void UEnhancedImage::SetBrushFromAsset(USlateBrushAsset *Asset) {
    Super::SetBrushFromAsset(Asset);
    SetSourceImageInternal(Asset->Brush.GetResourceObject());
}

void UEnhancedImage::SetBrushFromTexture(UTexture2D *Texture, bool bMatchSize) {
    Super::SetBrushFromTexture(Texture, bMatchSize);
    SourceImage.Set(Texture);
}

void UEnhancedImage::SetBrushFromTextureDynamic(UTexture2DDynamic *Texture, bool bMatchSize) {
    Super::SetBrushFromTextureDynamic(Texture, bMatchSize);
    SourceImage.Set(Texture);
}

void UEnhancedImage::SetBrushFromMaterial(UMaterialInterface *Material) {
    Super::SetBrushFromMaterial(Material);
    SourceImage.Set(Material);
}

void UEnhancedImage::SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion,
    bool bMatchSize) {
    Super::SetBrushFromAtlasInterface(AtlasRegion, bMatchSize);
    SourceImage.Set(AtlasRegion);
}

void UEnhancedImage::SetBrushFromImageAsset(const FImageAsset &ImageAsset, bool bMatchSize) {
    SourceImage = ImageAsset;
    switch (ImageAsset.GetTypeIndex()) {
    case FImageAsset::GetTypeIndex<UTexture2D>():
        SetBrushFromTexture(&ImageAsset.Get<UTexture2D>());
        break;
    case FImageAsset::GetTypeIndex<UTexture2DDynamic>():
        SetBrushFromTextureDynamic(&ImageAsset.Get<UTexture2DDynamic>());
        break;
    case FImageAsset::GetTypeIndex<UMaterialInterface>():
        SetBrushFromMaterial(&ImageAsset.Get<UMaterialInterface>());
        break;
    case FImageAsset::GetTypeIndex<ISlateTextureAtlasInterface>():
        SetBrushFromAtlasInterface(ImageAsset.Get<ISlateTextureAtlasInterface>()._getUObject());
        break;
    case FImageAsset::GetTypeIndex<UPaperFlipbook>():
        SetBrushFromAtlasInterface(ImageAsset.Get<UPaperFlipbook>().GetSpriteAtFrame(0));
        break;
    default:
        SetBrushFromAsset(nullptr);
    }
}

TSharedRef<SWidget> UEnhancedImage::RebuildImageWidget() {
    FlipbookWidget = SNew(SPaperFlipbookWidget);
    // clang-format off
    Switcher = SNew(SWidgetSwitcher)
        .WidgetIndex_UObject(this, &UEnhancedImage::GetWidgetIndex)
        + SWidgetSwitcher::Slot() [
            Super::RebuildImageWidget()
        ] + SWidgetSwitcher::Slot() [
            FlipbookWidget.ToSharedRef()
        ];
    // clang-format on
    return Switcher.ToSharedRef();
}

void UEnhancedImage::SynchronizeProperties() {
    Super::SynchronizeProperties();

    auto ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
    if (FlipbookWidget.IsValid()) {
        auto &CurrentBrush = GetBrush();
        FlipbookWidget->OverrideBrushSize(CurrentBrush.GetImageSize(), true);
        FlipbookWidget->SetBrushMirroring(CurrentBrush.GetMirroring());
        FlipbookWidget->SetBrushTiling(CurrentBrush.GetTiling());
        FlipbookWidget->SetColorAndOpacity(ColorAndOpacityBinding);
        FlipbookWidget->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDown));
        FlipbookWidget->SetOnFinishedPlaying(BIND_UOBJECT_DELEGATE(FSimpleDelegate, OnFlipbookFinishedPlaying));
        FlipbookWidget->SetFlipbook(SourceImage.TryGet<UPaperFlipbook>().GetPtrOrNull());
        FlipbookWidget->SetPlayRate(PlayRate);
        FlipbookWidget->SetLooping(bLooping);

        if (bAutoPlay) {
            FlipbookWidget->Play();
        } else {
            FlipbookWidget->Stop();
        }
    }
}

void UEnhancedImage::ReleaseSlateResources(bool bReleaseChildren) {
    Super::ReleaseSlateResources(bReleaseChildren);
    FlipbookWidget.Reset();
}

#if WITH_EDITOR
void UEnhancedImage::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UEnhancedImage, SourceImage)) {
        auto UpdateBrush = GetBrush();
        if (SourceImage.IsType<UPaperFlipbook>()) {
            auto &Flipbook = SourceImage.Get<UPaperFlipbook>();
            UpdateBrush.SetResourceObject(Flipbook.GetSpriteAtFrame(0));
        } else {
            UpdateBrush.SetResourceObject(SourceImage.TryGet().GetPtrOrNull());
        }
    } else if (PropertyChangedEvent.GetPropertyName() == "Brush") {
        if (auto &BrushData = GetBrush(); BrushData.GetResourceObject() == nullptr) {
            SourceImage.Reset();
        } else {
            SourceImage.Set(BrushData.GetResourceObject());
        }
    }
}
#endif

void UEnhancedImage::SetSourceImageInternal(UObject *Object) {
    if (Object != nullptr) {
        SourceImage.Set(Object);
    } else {
        SourceImage.Reset();
    }
}

int32 UEnhancedImage::GetWidgetIndex() const {
    return SourceImage.IsType<UPaperFlipbook>() ? PaperFlipbookWidgetIndex : ImageWidgetIndex;
}

void UEnhancedImage::OnFlipbookFinishedPlaying() const {
    OnFinishedPlaying.Broadcast();
}