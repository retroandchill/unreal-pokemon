// "Unreal Pokémon" created by Retro & Chill.

#include "Components/EnhancedImage.h"
#include "InteractiveUI.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PaperSprite.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "Simple2D/Rendering/MaterialSettings.h"
#include "Slate/SlateBrushAsset.h"
#include "UObject/ConstructorHelpers.h"

DECLARE_CYCLE_STAT(TEXT("Tick Enhanced Image"), STAT_TickEnhancedImage, STATGROUP_InteractiveUI);

UEnhancedImage::UEnhancedImage()
{
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterialRef(
        TEXT("/Simple2D/Materials/MaskedWidgetFlipbookMaterial"));
    SimpleFlipbookBaseMaterial = MaskedMaterialRef.Object;

    FlipbookTicker.BindToOnFinishedPlaying(this, &UEnhancedImage::OnFlipbookFinishedPlaying);
    FlipbookTicker.BindToOnFrameIndexChanged(this, &UEnhancedImage::OnFrameIndexChanged);
}

void UEnhancedImage::SetBrush(const FSlateBrush &InBrush)
{
    SetBrush(InBrush, true);
}

void UEnhancedImage::SetBrush(const FSlateBrush &InBrush, bool bUpdateAssetImage)
{
    Super::SetBrush(InBrush);
    if (bUpdateAssetImage)
    {
        SetSourceImageInternal(InBrush.GetResourceObject());
        bManualSize = true;
    }
}

void UEnhancedImage::SetBrushFromAsset(USlateBrushAsset *Asset)
{
    Super::SetBrushFromAsset(Asset);
    // clang-format off
    SetSourceImageInternal(Retro::Optionals::OfNullable(Asset) |
                           Retro::Optionals::Transform(&USlateBrushAsset::Brush) |
                           Retro::Optionals::Transform(&FSlateBrush::GetResourceObject) |
                           Retro::Optionals::PtrOrNull);
    // clang-format on
    bManualSize = true;
}

void UEnhancedImage::SetBrushFromTexture(UTexture2D *Texture, bool bMatchSize)
{
    Super::SetBrushFromTexture(Texture, bMatchSize);
    SourceImage.Set(Texture);
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromTextureDynamic(UTexture2DDynamic *Texture, bool bMatchSize)
{
    Super::SetBrushFromTextureDynamic(Texture, bMatchSize);
    SourceImage.Set(Texture);
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromMaterial(UMaterialInterface *Material)
{
    Super::SetBrushFromMaterial(Material);
    SourceImage.Set(Material);
    bManualSize = true;
}

void UEnhancedImage::SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion,
                                                bool bMatchSize)
{
    Super::SetBrushFromAtlasInterface(AtlasRegion, bMatchSize);
    SourceImage.Set(AtlasRegion);
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromPaperFlipbook(UPaperFlipbook *Flipbook, bool bMatchSize)
{
    Super::SetBrushFromAtlasInterface(Flipbook != nullptr ? Flipbook->GetSpriteAtFrame(0) : nullptr, bMatchSize);
    FlipbookTicker.SetFlipbook(Flipbook);
    SourceImage.Set(Flipbook);
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromSimpleFlipbook(USimpleFlipbook *Flipbook, bool bMatchSize)
{
    if (Flipbook != nullptr)
    {
        if (SourceImage != Flipbook || SimpleFlipbookMaterialInstance == nullptr)
        {
            CreateSimpleFlipbookMaterialInstance(*Flipbook);
        }
        Super::SetBrushFromMaterial(SimpleFlipbookMaterialInstance);
    }
    else
    {
        Super::SetBrushFromMaterial(nullptr);
    }

    FlipbookTicker.SetFlipbook(Flipbook);
    SourceImage.Set(Flipbook);
    bManualSize = !bMatchSize;

    if (bMatchSize)
    {
        if (auto Texture = Flipbook->GetSourceTexture(); !bManualSize && Texture != nullptr)
        {
            SetDesiredSizeOverride(
                FVector2D(Texture->GetSizeX() / Flipbook->GetColumns(), Texture->GetSizeY() / Flipbook->GetRows()));
        }
    }
}

void UEnhancedImage::SetBrushFromImageAsset(const FImageAsset &ImageAsset, bool bMatchSize)
{
    switch (ImageAsset.GetTypeIndex())
    {
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
        SetBrushFromSimpleFlipbook(&ImageAsset.Get<USimpleFlipbook>(), bMatchSize);
        break;
    default:
        SetBrushFromAsset(nullptr);
        break;
    }
}

FVoidCoroutine UEnhancedImage::SetBrushFromLazyPaperFlipbook(const TSoftObjectPtr<UPaperFlipbook> &LazyFlipbook,
                                                             bool bMatchSize, FForceLatentCoroutine)
{
    if (!LazyFlipbook.IsNull())
    {
        auto LoadedAsset = co_await UE5Coro::Latent::AsyncLoadObject(LazyFlipbook);
        ensureMsgf(LoadedAsset != nullptr, TEXT("Failed to load %s"), *LazyFlipbook.ToSoftObjectPath().ToString());
        SetBrushFromPaperFlipbook(LoadedAsset, bMatchSize);
    }
    else
    {
        // Hack to get into the private method that is inaccessible
        SetBrushFromLazyDisplayAsset(LazyFlipbook, bMatchSize);
    }
}

FVoidCoroutine UEnhancedImage::SetBrushFromLazySimpleFlipbook(const TSoftObjectPtr<USimpleFlipbook> &LazyFlipbook,
                                                              bool bMatchSize, FForceLatentCoroutine)
{
    if (!LazyFlipbook.IsNull())
    {
        auto LoadedAsset = co_await UE5Coro::Latent::AsyncLoadObject(LazyFlipbook);
        ensureMsgf(LoadedAsset != nullptr, TEXT("Failed to load %s"), *LazyFlipbook.ToSoftObjectPath().ToString());
        SetBrushFromSimpleFlipbook(LoadedAsset, bMatchSize);
    }
    else
    {
        // Hack to get into the private method that is inaccessible
        SetBrushFromLazyDisplayAsset(LazyFlipbook, bMatchSize);
    }
}

FVoidCoroutine UEnhancedImage::SetBrushFromLazyImageAsset(const FSoftImageAsset &LazyImage, bool bMatchSize,
                                                          FForceLatentCoroutine)
{
    if (auto &SoftPointer = LazyImage.ToSoftObjectPtr(); !SoftPointer.IsNull())
    {
        auto LoadedAsset = co_await LazyImage.LoadAsync();
        ensureMsgf(LoadedAsset.IsSet(), TEXT("Failed to load %s"), *LazyImage.ToSoftObjectPath().ToString());
        SetBrushFromImageAsset(LoadedAsset.GetValue(), bMatchSize);
    }
    else
    {
        // Hack to get into the private method that is inaccessible
        SetBrushFromLazyDisplayAsset(SoftPointer, bMatchSize);
    }
}

void UEnhancedImage::Tick(float DeltaTime)
{
    if (!SourceImage.IsType<UPaperFlipbook>() && !SourceImage.IsType<USimpleFlipbook>())
    {
        return;
    }

    SCOPE_CYCLE_COUNTER(STAT_TickEnhancedImage)

    FlipbookTicker.TickFlipbook(DeltaTime);
    FlipbookTicker.CalculateCurrentFrame();
}

TStatId UEnhancedImage::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UEnhancedImage, STATGROUP_Tickables)
}

bool UEnhancedImage::IsTickable() const
{
    return SourceImage.IsType<UPaperFlipbook>() || SourceImage.IsType<USimpleFlipbook>();
}

bool UEnhancedImage::IsTickableInEditor() const
{
    return true;
}

void UEnhancedImage::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (SourceImage != nullptr)
    {
        SourceImage.Visit([this]<typename T>(T *Flipbook) {
            if constexpr (Simple2D::Flipbook<T>)
            {
                FlipbookTicker.SetFlipbook(Flipbook);
            }
        });
    }
    FlipbookTicker.SetPlayRate(PlayRate);
    FlipbookTicker.SetLooping(bLooping);
    if (bAutoPlay)
    {
        FlipbookTicker.Play();
    }
    else
    {
        FlipbookTicker.Stop();
    }
}

#if WITH_EDITOR
void UEnhancedImage::PostLoad()
{
    Super::PostLoad();

    if (SourceImage.IsType<USimpleFlipbook>())
    {
        const auto &Flipbook = SourceImage.Get<USimpleFlipbook>();
        CreateSimpleFlipbookMaterialInstance(Flipbook);
        SetBrushResourceObject(SimpleFlipbookMaterialInstance);
    }
}

void UEnhancedImage::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UEnhancedImage, SourceImage))
    {
        if (SourceImage.IsType<USimpleFlipbook>())
        {
            const auto &Flipbook = SourceImage.Get<USimpleFlipbook>();
            CreateSimpleFlipbookMaterialInstance(Flipbook);
            SetBrushResourceObject(SimpleFlipbookMaterialInstance);
        }
        else if (SourceImage.IsType<UPaperFlipbook>())
        {
            const auto &Flipbook = SourceImage.Get<UPaperFlipbook>();
            SetBrushResourceObject(Flipbook.GetSpriteAtFrame(0));
        }
        else
        {
            SetBrushResourceObject(SourceImage.TryGet().GetPtrOrNull());
        }

        if (!bManualSize && SourceImage.IsValid() && !SourceImage.IsType<UMaterialInterface>())
        {
            switch (SourceImage.GetTypeIndex())
            {
            case FImageAsset::GetTypeIndex<UTexture2D>(): {
                const auto &Texture = SourceImage.Get<UTexture2D>();
                SetDesiredSizeOverride(FVector2D(Texture.GetSizeX(), Texture.GetSizeY()));
                break;
            }
            case FImageAsset::GetTypeIndex<UTexture2DDynamic>(): {
                const auto &Texture = SourceImage.Get<UTexture2DDynamic>();
                SetDesiredSizeOverride(FVector2D(Texture.SizeX, Texture.SizeY));
                break;
            }
            case FImageAsset::GetTypeIndex<ISlateTextureAtlasInterface>():
                SetDesiredSizeOverride(
                    SourceImage.Get<ISlateTextureAtlasInterface>().GetSlateAtlasData().GetSourceDimensions());
                break;
            case FImageAsset::GetTypeIndex<UPaperFlipbook>():
                SetDesiredSizeOverride(SourceImage.Get<UPaperFlipbook>().GetSpriteAtFrame(0)->GetSourceSize());
                break;
            case FImageAsset::GetTypeIndex<USimpleFlipbook>(): {
                auto &Flipbook = SourceImage.Get<USimpleFlipbook>();
                auto Texture = Flipbook.GetSourceTexture();
                SetDesiredSizeOverride(
                    FVector2D(Texture->GetSizeX() / Flipbook.GetColumns(), Texture->GetSizeY() / Flipbook.GetRows()));
                break;
            }
            default:
                break;
            }
        }
    }
}
#endif

void UEnhancedImage::SetSourceImageInternal(UObject *Object)
{
    if (Object != nullptr)
    {
        SourceImage.Set(Object);
    }
    else
    {
        SourceImage.Reset();
    }
}

void UEnhancedImage::CreateSimpleFlipbookMaterialInstance(const USimpleFlipbook &Flipbook)
{
    SimpleFlipbookMaterialInstance = UMaterialInstanceDynamic::Create(SimpleFlipbookBaseMaterial, this);
    SimpleFlipbookMaterialInstance->SetTextureParameterValue(Simple2D::Flipbooks::TextureParameterName,
                                                             Flipbook.GetSourceTexture());
    SimpleFlipbookMaterialInstance->SetScalarParameterValue(Simple2D::Flipbooks::RowsParameterName,
                                                            static_cast<float>(Flipbook.GetRows()));
    SimpleFlipbookMaterialInstance->SetScalarParameterValue(Simple2D::Flipbooks::ColumnsParameterName,
                                                            static_cast<float>(Flipbook.GetColumns()));
}

void UEnhancedImage::OnFlipbookFinishedPlaying() const
{
    OnFinishedPlaying.Broadcast();
}

void UEnhancedImage::OnFrameIndexChanged(std::any KeyFrame)
{
    if (KeyFrame.type() == typeid(FPaperFlipbookKeyFrame))
    {
        if (auto &[Sprite, FrameRun] = std::any_cast<FPaperFlipbookKeyFrame &>(KeyFrame); Sprite != nullptr)
        {
            const FSlateAtlasData SpriteAtlasData = Sprite->GetSlateAtlasData();
            const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();
            SetBrushResourceObject(Sprite);

            if (!bManualSize)
            {
                SetDesiredSizeOverride(SpriteSize);
            }
        }
        else
        {
            SetBrushResourceObject(nullptr);
        }
    }
    else if (KeyFrame.type() == typeid(FSimpleFlipbookKeyFrame))
    {
        auto &[Index, FrameRun] = std::any_cast<FSimpleFlipbookKeyFrame &>(KeyFrame);
        check(IsValid(SimpleFlipbookMaterialInstance))
        if (auto &Flipbook = SourceImage.Get<USimpleFlipbook>(); Flipbook.IsValidKeyFrameIndex(Index))
        {
            SimpleFlipbookMaterialInstance->SetScalarParameterValue(Simple2D::Flipbooks::FrameParameterName,
                                                                    static_cast<float>(Index));
            SetBrushResourceObject(SimpleFlipbookMaterialInstance);
            if (auto Texture = Flipbook.GetSourceTexture(); !bManualSize && Texture != nullptr)
            {
                SetDesiredSizeOverride(
                    FVector2D(Texture->GetSizeX() / Flipbook.GetColumns(), Texture->GetSizeY() / Flipbook.GetRows()));
            }
        }
        else
        {
            SetBrushResourceObject(nullptr);
        }
    }
    else
    {
        UE_LOG(LogInteractiveUI, Warning, TEXT("Unknown key frame type!"))
    }
}