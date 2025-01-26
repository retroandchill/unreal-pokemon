// "Unreal Pokémon" created by Retro & Chill.

#include "Components/EnhancedImage.h"
#include "PaperSprite.h"
#include "RPGMenus.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "Slate/SlateBrushAsset.h"
#include "SPaperFlipbookWidget.h"
#include "Simple2D/Rendering/MaterialSettings.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

DECLARE_CYCLE_STAT(TEXT("Tick Enhanced Image"), STAT_TickEnhancedImage, STATGROUP_RPGMenus);

constexpr int32 ImageWidgetIndex = 0;
constexpr int32 PaperFlipbookWidgetIndex = 1;

UEnhancedImage::UEnhancedImage() {
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterialRef(
        TEXT("/Simple2D/Materials/MaskedUnlitFlipbookMaterial"));
    SimpleFlipbookBaseMaterial = MaskedMaterialRef.Object;
    
    FlipbookTicker.BindToOnFinishedPlaying(this, &UEnhancedImage::OnFlipbookFinishedPlaying);
    FlipbookTicker.BindToOnFrameIndexChanged(this, &UEnhancedImage::OnFrameIndexChanged);
}

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
                           Retro::Optionals::Transform(&USlateBrushAsset::Brush) |
                           Retro::Optionals::Transform(&FSlateBrush::GetResourceObject) |
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
    Super::SetBrushFromAtlasInterface(Flipbook != nullptr ? Flipbook->GetSpriteAtFrame(0) : nullptr, bMatchSize);
    SourceImage.Set(Flipbook);
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromSimpleFlipbook(USimpleFlipbook* Flipbook, bool bMatchSize) {
    if (Flipbook != nullptr) {
        if (SourceImage != Flipbook || SimpleFlipbookMaterialInstance == nullptr) {
            CreateSimpleFlipbookMaterialInstance(*Flipbook);
        }
        Super::SetBrushFromMaterial(SimpleFlipbookMaterialInstance);
    } else {
        Super::SetBrushFromMaterial(nullptr);
    }
    
    SourceImage.Set(Flipbook);
    bManualSize = !bMatchSize;
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
    case FImageAsset::GetTypeIndex<USimpleFlipbook>():
        SetBrushFromLazySimpleFlipbook(&ImageAsset.Get<USimpleFlipbook>(), bMatchSize);
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

void UEnhancedImage::SetBrushFromLazySimpleFlipbook(const TSoftObjectPtr<USimpleFlipbook> &LazyFlipbook,
    bool bMatchSize) {
    if (!LazyFlipbook.IsNull()) {
        RequestAsyncLoad(LazyFlipbook, FStreamableDelegate::CreateWeakLambda(this, [this, LazyFlipbook, bMatchSize] {
                             ensureMsgf(LazyFlipbook.Get(), TEXT("Failed to load %s"),
                                        *LazyFlipbook.ToSoftObjectPath().ToString());
                             SetBrushFromLazySimpleFlipbook(LazyFlipbook.Get(), bMatchSize);
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

void UEnhancedImage::Tick(float DeltaTime) {
    if (!SourceImage.IsType<UPaperFlipbook>() && !SourceImage.IsType<USimpleFlipbook>()) {
        return;
    }

    SCOPE_CYCLE_COUNTER(STAT_TickEnhancedImage);

    FlipbookTicker.TickFlipbook(DeltaTime);
    FlipbookTicker.CalculateCurrentFrame();
}

TStatId UEnhancedImage::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UEnhancedImage, STATGROUP_Tickables)
}

bool UEnhancedImage::IsTickable() const {
    return SourceImage.IsType<UPaperFlipbook>() || SourceImage.IsType<USimpleFlipbook>();
}

bool UEnhancedImage::IsTickableInEditor() const {
    return true;
}

void UEnhancedImage::SynchronizeProperties() {
    Super::SynchronizeProperties();

    SourceImage.Visit([this]<typename T>(T *Flipbook) {
            if constexpr (Simple2D::Flipbook<T>) {
                FlipbookTicker.SetFlipbook(Flipbook);
            }
        });
    FlipbookTicker.SetPlayRate(PlayRate);
    FlipbookTicker.SetLooping(bLooping);
    if (bAutoPlay) {
        FlipbookTicker.Play();
    } else {
        FlipbookTicker.Stop();
    }
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
            case FImageAsset::GetTypeIndex<USimpleFlipbook>(): {
                auto &Flipbook = SourceImage.Get<USimpleFlipbook>();
                auto Texture = Flipbook.GetSourceTexture();
                UpdateBrush.SetImageSize(FVector2D(Texture->GetSizeX() / Flipbook.GetColumns(), Texture->GetSizeY() / Flipbook.GetRows()));
                break;
            }
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



void UEnhancedImage::CreateSimpleFlipbookMaterialInstance(const USimpleFlipbook& Flipbook) {
    SimpleFlipbookMaterialInstance = UMaterialInstanceDynamic::Create(SimpleFlipbookBaseMaterial, this);
    SimpleFlipbookMaterialInstance->SetTextureParameterValue(Simple2D::Flipbooks::TextureParameterName,
                                                             Flipbook.GetSourceTexture());
    SimpleFlipbookMaterialInstance->SetScalarParameterValue(Simple2D::Flipbooks::RowsParameterName,
                                                            static_cast<float>(Flipbook.GetRows()));
    SimpleFlipbookMaterialInstance->SetScalarParameterValue(Simple2D::Flipbooks::ColumnsParameterName,
                                                            static_cast<float>(Flipbook.GetColumns()));
}

int32 UEnhancedImage::GetWidgetIndex() const {
    return SourceImage.IsType<UPaperFlipbook>() ? PaperFlipbookWidgetIndex : ImageWidgetIndex;
}

void UEnhancedImage::OnFlipbookFinishedPlaying() const {
    OnFinishedPlaying.Broadcast();
}

void UEnhancedImage::OnFrameIndexChanged(std::any KeyFrame) {
    if (KeyFrame.type() == typeid(FPaperFlipbookKeyFrame)) {
        if (auto &[Sprite, FrameRun] = std::any_cast<FPaperFlipbookKeyFrame&>(KeyFrame); Sprite != nullptr) {
            const FSlateAtlasData SpriteAtlasData = Sprite->GetSlateAtlasData();
            const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();
            SetBrushResourceObject(Sprite);

            if (!bManualSize) {
                SetDesiredSizeOverride(SpriteSize);
            }
        } else {
            SetBrushResourceObject(nullptr);
        }
    } else if (KeyFrame.type() == typeid(FSimpleFlipbookKeyFrame)) {
        auto &[Index, FrameRun] = std::any_cast<FSimpleFlipbookKeyFrame&>(KeyFrame);
        check(IsValid(SimpleFlipbookMaterialInstance))
        if (auto &Flipbook = SourceImage.Get<USimpleFlipbook>(); Flipbook.IsValidKeyFrameIndex(Index)) {
            SimpleFlipbookMaterialInstance->SetScalarParameterValue(Simple2D::Flipbooks::FrameParameterName, static_cast<float>(Index));
            SetBrushResourceObject(SimpleFlipbookMaterialInstance);
            if (auto Texture = Flipbook.GetSourceTexture(); !bManualSize && Texture != nullptr) {
                SetDesiredSizeOverride(FVector2D(Texture->GetSizeX() / Flipbook.GetColumns(), Texture->GetSizeY() / Flipbook.GetRows()));
            }
        } else {
            SetBrushResourceObject(nullptr);
        }
    } else {
        UE_LOG(LogRPGMenus, Warning, TEXT("Unknown key frame type!"))
    }
}