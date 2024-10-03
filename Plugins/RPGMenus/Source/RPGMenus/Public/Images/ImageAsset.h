// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "Ranges/Utilities/VariantObject.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "UObject/Object.h"
#include "ImageAsset.generated.h"

/**
 * Struct that contains a wrapper around an image asset that can be used inside an image widget.
 * TODO: Add support for UPaperFlipbook
 */
USTRUCT(BlueprintType)
struct RPGMENUS_API FImageAsset
#if CPP
    : UE::Ranges::TVariantObject<UTexture, UMaterialInterface, ISlateTextureAtlasInterface>, FGCObject {
#else
    {
#endif
    GENERATED_BODY()
    
    static constexpr bool bHasIntrusiveUnsetOptionalState = true;

    FImageAsset() = default;

    template <typename... A>
        requires std::constructible_from<TVariantObject, A...>
    explicit FImageAsset(A&&... Args) : TVariantObject(Forward<A>(Args)...) {}
    
    void AddReferencedObjects(FReferenceCollector &Collector) override;
    FString GetReferencerName() const override;

  private:
    friend struct TOptional<FImageAsset>;

    explicit FImageAsset(FIntrusiveUnsetOptionalState State);
    FImageAsset& operator=(FIntrusiveUnsetOptionalState State) final;
    
};

static_assert(UE::Ranges::VariantObjectStruct<FImageAsset>);