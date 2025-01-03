// "Unreal Pokémon" created by Retro & Chill.

#include "Components/EnhancedImage.h"
#include "PaperSprite.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "Slate/SlateBrushAsset.h"
#include "SPaperFlipbookWidget.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

constexpr int32 ImageWidgetIndex = 0;
constexpr int32 PaperFlipbookWidgetIndex = 1;

void UEnhancedImage::SetBrush(const FSlateBrush &InBrush) {
    SetBrush(InBrush, true);
}

void UEnhancedImage::SetBrush(const FSlateBrush &InBrush, bool bUpdateAssetImage) {
    Super::SetBrush(InBrush);
    if (bUpdateAssetImage) {
        SetSourceImageInternal(InBrush.GetResourceObject());
        bManualSize = true;
    }
}

void UEnhancedImage::SetBrushFromAsset(USlateBrushAsset *Asset) {
    Super::SetBrushFromAsset(Asset);
    // clang-format off
    SetSourceImageInternal(Retro::Optionals::OfNullable(Asset) |
                           Retro::Optionals::Transform<&USlateBrushAsset::Brush>() |
                           Retro::Optionals::Transform<&FSlateBrush::GetResourceObject>() |
                           Retro::Optionals::PtrOrNull);
    // clang-format on
    bManualSize = true;
}

void UEnhancedImage::SetBrushFromTexture(UTexture2D *Texture, bool bMatchSize) {
    Super::SetBrushFromTexture(Texture, bMatchSize);
    SourceImage.Set(Texture);
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromTextureDynamic(UTexture2DDynamic *Texture, bool bMatchSize) {
    Super::SetBrushFromTextureDynamic(Texture, bMatchSize);
    SourceImage.Set(Texture);
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromMaterial(UMaterialInterface *Material) {
    Super::SetBrushFromMaterial(Material);
    SourceImage.Set(Material);
    bManualSize = true;
}

void UEnhancedImage::SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion,
                                                bool bMatchSize) {
    Super::SetBrushFromAtlasInterface(AtlasRegion, bMatchSize);
    SourceImage.Set(AtlasRegion);
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromPaperFlipbook(UPaperFlipbook *Flipbook, bool bMatchSize) {
    Super::SetBrushFromAtlasInterface(Flipbook->GetSpriteAtFrame(0), bMatchSize);
    SourceImage.Set(Flipbook);
    bManualSize = !bMatchSize;
    if (FlipbookWidget != nullptr) {
        FlipbookWidget->SetFlipbook(Flipbook);
    }
}

void UEnhancedImage::SetBrushFromImageAsset(const FImageAsset &ImageAsset, bool bMatchSize) {
    SourceImage = ImageAsset;
    switch (ImageAsset.GetTypeIndex()) {
    case FImageAsset::GetTypeIndex<UTexture2D>():
        SetBrushFromTexture(&ImageAsset.Get<UTexture2D>(), bMatchSize);
        break;
    case FImageAsset::GetTypeIndex<UTexture2DDynamic>():
        SetBrushFromTextureDynamic(&ImageAsset.Get<UTexture2DDynamic>(), bMatchSize);
        break;
    case FImageAsset::GetTypeIndex<UMaterialInterface>():
        SetBrushFromMaterial(&ImageAsset.Get<UMaterialInterface>());
        break;
    case FImageAsset::GetTypeIndex<ISlateTextureAtlasInterface>():
        SetBrushFromAtlasInterface(ImageAsset.Get<ISlateTextureAtlasInterface>()._getUObject(), bMatchSize);
        break;
    case FImageAsset::GetTypeIndex<UPaperFlipbook>():
        SetBrushFromPaperFlipbook(&ImageAsset.Get<UPaperFlipbook>(), bMatchSize);
        break;
    default:
        SetBrushFromAsset(nullptr);
        break;
    }
}

void UEnhancedImage::SetBrushFromLazyPaperFlipbook(const TSoftObjectPtr<UPaperFlipbook> &LazyFlipbook,
                                                   bool bMatchSize) {
    if (!LazyFlipbook.IsNull()) {
        RequestAsyncLoad(LazyFlipbook, FStreamableDelegate::CreateWeakLambda(this, [this, LazyFlipbook, bMatchSize] {
                             ensureMsgf(LazyFlipbook.Get(), TEXT("Failed to load %s"),
                                        *LazyFlipbook.ToSoftObjectPath().ToString());
                             SetBrushFromPaperFlipbook(LazyFlipbook.Get(), bMatchSize);
                         }));
    } else {
        // Hack to get into the private method that is inaccessible
        SetBrushFromLazyDisplayAsset(LazyFlipbook, bMatchSize);
    }
}

void UEnhancedImage::SetBrushFromLazyImageAsset(const FSoftImageAsset &LazyImage, bool bMatchSize) {
    if (auto &SoftPointer = LazyImage.ToSoftObjectPtr(); !SoftPointer.IsNull()) {
        RequestAsyncLoad(SoftPointer, FStreamableDelegate::CreateWeakLambda(this, [this, LazyImage, bMatchSize] {
                             ensureMsgf(LazyImage.IsValid(), TEXT("Failed to load %s"),
                                        *LazyImage.ToSoftObjectPath().ToString());
                             auto FoundAsset = LazyImage.LoadSynchronous();
            check(FoundAsset.IsSet())
                             SetBrushFromImageAsset(FoundAsset.GetValue(), bMatchSize);
                         }));
    } else {
        // Hack to get into the private method that is inaccessible
        SetBrushFromLazyDisplayAsset(SoftPointer, bMatchSize);
    }
}

TSharedRef<SWidget> UEnhancedImage::RebuildImageWidget() {
    FlipbookWidget = SNew(SPaperFlipbookWidget);
    // clang-format off
    Switcher = SNew(SWidgetSwitcher)
        .WidgetIndex_UObject(this, &UEnhancedImage::GetWidgetIndex)
        + SWidgetSwitcher::Slot()
        [
            Super::RebuildImageWidget()
        ]
        + SWidgetSwitcher::Slot()
        [
            FlipbookWidget.ToSharedRef()
        ];
    // clang-format on
    return Switcher.ToSharedRef();
}

void UEnhancedImage::SynchronizeProperties() {
    Super::SynchronizeProperties();

    auto ColorAndOpacityBinding = PROPERTY_BINDING_WRAPPED(FSlateColor, ColorAndOpacity);
    if (FlipbookWidget.IsValid()) {
        auto &CurrentBrush = GetBrush();
        FlipbookWidget->OverrideBrushSize(CurrentBrush.GetImageSize(), bManualSize);
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
    Switcher.Reset();
    FlipbookWidget.Reset();
}

#if WITH_EDITOR
void UEnhancedImage::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UEnhancedImage, SourceImage)) {
        auto UpdateBrush = GetBrush();
        if (SourceImage.IsType<UPaperFlipbook>()) {
            const auto &Flipbook = SourceImage.Get<UPaperFlipbook>();
            UpdateBrush.SetResourceObject(Flipbook.GetSpriteAtFrame(0));
        } else {
            UpdateBrush.SetResourceObject(SourceImage.TryGet().GetPtrOrNull());
        }

        if (!bManualSize && SourceImage.IsValid() && !SourceImage.IsType<UMaterialInterface>()) {
            switch (SourceImage.GetTypeIndex()) {
            case FImageAsset::GetTypeIndex<UTexture2D>(): {
                const auto &Texture = SourceImage.Get<UTexture2D>();
                UpdateBrush.SetImageSize(FVector2D(Texture.GetSizeX(), Texture.GetSizeY()));
                break;
            }
            case FImageAsset::GetTypeIndex<UTexture2DDynamic>(): {
                const auto &Texture = SourceImage.Get<UTexture2DDynamic>();
                UpdateBrush.SetImageSize(FVector2D(Texture.SizeX, Texture.SizeY));
                break;
            }
            case FImageAsset::GetTypeIndex<ISlateTextureAtlasInterface>():
                UpdateBrush.SetImageSize(
                    SourceImage.Get<ISlateTextureAtlasInterface>().GetSlateAtlasData().GetSourceDimensions());
                break;
            case FImageAsset::GetTypeIndex<UPaperFlipbook>():
                UpdateBrush.SetImageSize(SourceImage.Get<UPaperFlipbook>().GetSpriteAtFrame(0)->GetSourceSize());
                break;
            default:
                break;
            }
        }
        SetBrush(UpdateBrush, false);
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