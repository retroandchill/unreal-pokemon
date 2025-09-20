// "Unreal Pokémon" created by Retro & Chill.

#include "Components/EnhancedImage.h"
#include "InteractiveUI.h"
#include "OptionalPtr.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PaperSprite.h"
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

void UEnhancedImage::SetBrush(const FSlateBrush &InBrush, const bool bUpdateAssetImage)
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
    SetSourceImageInternal(TOptionalPtr(Asset)
        .Map(&USlateBrushAsset::Brush)
        .Map(&FSlateBrush::GetResourceObject)
        .Get());
    bManualSize = true;
}

void UEnhancedImage::SetBrushFromTexture(UTexture2D *Texture, const bool bMatchSize)
{
    Super::SetBrushFromTexture(Texture, bMatchSize);
    SourceImage = Texture;
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromTextureDynamic(UTexture2DDynamic *Texture, const bool bMatchSize)
{
    Super::SetBrushFromTextureDynamic(Texture, bMatchSize);
    SourceImage = Texture;
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromMaterial(UMaterialInterface *Material)
{
    Super::SetBrushFromMaterial(Material);
    SourceImage = Material;
    bManualSize = true;
}

void UEnhancedImage::SetBrushFromAtlasInterface(const TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion,
                                                const bool bMatchSize)
{
    Super::SetBrushFromAtlasInterface(AtlasRegion, bMatchSize);
    SourceImage = AtlasRegion.GetObject();
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromPaperFlipbook(UPaperFlipbook *Flipbook, const bool bMatchSize)
{
    Super::SetBrushFromAtlasInterface(Flipbook != nullptr ? Flipbook->GetSpriteAtFrame(0) : nullptr, bMatchSize);
    FlipbookTicker.SetFlipbook(Flipbook);
    SourceImage = Flipbook;
    bManualSize = !bMatchSize;
}

void UEnhancedImage::SetBrushFromSimpleFlipbook(USimpleFlipbook *Flipbook, const bool bMatchSize)
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
    SourceImage = Flipbook;
    bManualSize = !bMatchSize;

    if (bMatchSize)
    {
        if (const auto *Texture = Flipbook->GetSourceTexture(); !bManualSize && Texture != nullptr)
        {
            SetDesiredSizeOverride(
                FVector2D(Texture->GetSizeX() / Flipbook->GetColumns(), Texture->GetSizeY() / Flipbook->GetRows()));
        }
    }
}

FVoidCoroutine UEnhancedImage::SetBrushFromLazyPaperFlipbook(const TSoftObjectPtr<UPaperFlipbook> &LazyFlipbook,
                                                             const bool bMatchSize, FForceLatentCoroutine)
{
    if (!LazyFlipbook.IsNull())
    {
        auto *LoadedAsset = co_await UE5Coro::Latent::AsyncLoadObject(LazyFlipbook);
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
        auto *LoadedAsset = co_await UE5Coro::Latent::AsyncLoadObject(LazyFlipbook);
        ensureMsgf(LoadedAsset != nullptr, TEXT("Failed to load %s"), *LazyFlipbook.ToSoftObjectPath().ToString());
        SetBrushFromSimpleFlipbook(LoadedAsset, bMatchSize);
    }
    else
    {
        // Hack to get into the private method that is inaccessible
        SetBrushFromLazyDisplayAsset(LazyFlipbook, bMatchSize);
    }
}

void UEnhancedImage::Tick(float DeltaTime)
{
    if (!SourceImage.IsA<UPaperFlipbook>() && !SourceImage.IsA<USimpleFlipbook>())
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
    return SourceImage.IsA<UPaperFlipbook>() || SourceImage.IsA<USimpleFlipbook>();
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
        if (auto* AsPaperFlipbook = Cast<UPaperFlipbook>(SourceImage.Get()); AsPaperFlipbook != nullptr)
        {
            FlipbookTicker.SetFlipbook(AsPaperFlipbook);
        } else if (auto* AsSimpleFlipbook = Cast<USimpleFlipbook>(SourceImage.Get()); AsSimpleFlipbook != nullptr)
        {
            FlipbookTicker.SetFlipbook(AsSimpleFlipbook);
        }
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

    if (auto *Flipbook = Cast<USimpleFlipbook>(SourceImage); Flipbook != nullptr)
    {
        CreateSimpleFlipbookMaterialInstance(*Flipbook);
        SetBrushResourceObject(SimpleFlipbookMaterialInstance);
    }
}

void UEnhancedImage::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UEnhancedImage, SourceImage))
    {
        if (const auto* AsSimpleFlipbook = Cast<USimpleFlipbook>(SourceImage); AsSimpleFlipbook != nullptr)
        {
            CreateSimpleFlipbookMaterialInstance(*AsSimpleFlipbook);
            SetBrushResourceObject(SimpleFlipbookMaterialInstance);

            if (!bManualSize)
            {
                const auto *Texture = AsSimpleFlipbook->GetSourceTexture();
                SetDesiredSizeOverride(
                    FVector2D(Texture->GetSizeX() / AsSimpleFlipbook->GetColumns(), Texture->GetSizeY() / AsSimpleFlipbook->GetRows()));
            }
        }
        else if (const auto* AsPaperFlipbook = Cast<UPaperFlipbook>(SourceImage); AsPaperFlipbook != nullptr)
        {
            SetBrushResourceObject(AsPaperFlipbook->GetSpriteAtFrame(0));

            if (!bManualSize)
            {
                SetDesiredSizeOverride(AsPaperFlipbook->GetSpriteAtFrame(0)->GetSourceSize());
            }
        }
        else
        {
            SetBrushResourceObject(SourceImage);

            if (bManualSize)
            {
                return;
            }
            
            if (const auto* AsTexture2D = Cast<UTexture2D>(SourceImage); AsTexture2D != nullptr) {
                SetDesiredSizeOverride(FVector2D(AsTexture2D->GetSizeX(), AsTexture2D->GetSizeY()));
            } else if (const auto* AsTexture2DDynamic = Cast<UTexture2DDynamic>(SourceImage); AsTexture2DDynamic != nullptr) {
                SetDesiredSizeOverride(FVector2D(AsTexture2DDynamic->SizeX, AsTexture2DDynamic->SizeY));
            } else if (const auto* AsSlateTextureAtlasInterface = Cast<ISlateTextureAtlasInterface>(SourceImage); AsSlateTextureAtlasInterface != nullptr)
            {
                SetDesiredSizeOverride(AsSlateTextureAtlasInterface->GetSlateAtlasData().GetSourceDimensions());
            }
        }
    }
}
#endif

void UEnhancedImage::SetSourceImageInternal(UObject *Object)
{
    SourceImage = Object;
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
        if (const auto &Flipbook = *CastChecked<USimpleFlipbook>(SourceImage); Flipbook.IsValidKeyFrameIndex(Index))
        {
            SimpleFlipbookMaterialInstance->SetScalarParameterValue(Simple2D::Flipbooks::FrameParameterName,
                                                                    static_cast<float>(Index));
            SetBrushResourceObject(SimpleFlipbookMaterialInstance);
            if (const auto *Texture = Flipbook.GetSourceTexture(); !bManualSize && Texture != nullptr)
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