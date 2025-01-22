// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Rendering/FlipbookTextureOverrideRenderProxy.h"

namespace Simple2D {
    const FName FFlipbookTextureOverrideRenderProxy::TextureParameterName(TEXT("SpriteTexture"));
    const FName FFlipbookTextureOverrideRenderProxy::AdditionalTextureParameterRootName(TEXT("SpriteAdditionalTexture"));
    
#if WITH_EDITOR
    FFlipbookTextureOverrideRenderProxy::FFlipbookTextureOverrideRenderProxy(const FMaterialRenderProxy *InParent,
        const UTexture *InBaseTexture, FAdditionalSpriteTextureArray InAdditionalTextures,
        const FFlipbookRenderSceneProxyTextureOverrideMap &InTextureOverrideList)
    #else
    FFlipbookTextureOverrideRenderProxy::FFlipbookTextureOverrideRenderProxy(const FMaterialRenderProxy *InParent,
        const UTexture *InBaseTexture, FAdditionalSpriteTextureArray InAdditionalTextures) 
    #endif
        : FMaterialRenderProxy(InParent ? InParent->GetMaterialName() : TEXT("FSpriteTextureOverrideRenderProxy")),
          Parent(InParent), BaseTexture(InBaseTexture), AdditionalTextures(InAdditionalTextures),
    #if WITH_EDITOR
          TextureOverrideList(InTextureOverrideList),
    #endif
          ParentMaterialSerialNumber(INDEX_NONE) {

    }

    bool FFlipbookTextureOverrideRenderProxy::CheckValidity(const FMaterialRenderProxy *InCurrentParent) {
        if (InCurrentParent != Parent)
        {
            Parent = InCurrentParent;
            ParentMaterialSerialNumber = INDEX_NONE;
        }

        bool bValid = true;

        if (ParentMaterialSerialNumber != Parent->GetExpressionCacheSerialNumber())
        {
            bValid = false;
            ParentMaterialSerialNumber = Parent->GetExpressionCacheSerialNumber();
        }

        return bValid;
    }

    void FFlipbookTextureOverrideRenderProxy::Reinitialize(const FMaterialRenderProxy *InParent,
        const UTexture *InBaseTexture, FAdditionalSpriteTextureArray InAdditionalTextures) {
        if ((InParent != Parent) || (InBaseTexture != BaseTexture) || (InAdditionalTextures != AdditionalTextures))
        {
            Parent = InParent;
            BaseTexture = InBaseTexture;
            AdditionalTextures = InAdditionalTextures;
            ParentMaterialSerialNumber = INDEX_NONE;
        }
    }

    void FFlipbookTextureOverrideRenderProxy::InitPrimitiveResource(FRHICommandListBase &RHICmdList) {
        // Don't need to create anything
    }

    void FFlipbookTextureOverrideRenderProxy::ReleasePrimitiveResource() {
        // Don't need to release anything
    }

    const FMaterial * FFlipbookTextureOverrideRenderProxy::GetMaterialNoFallback(
        ERHIFeatureLevel::Type InFeatureLevel) const {
        return Parent->GetMaterialNoFallback(InFeatureLevel);
    }

    const FMaterialRenderProxy * FFlipbookTextureOverrideRenderProxy::GetFallback(
        ERHIFeatureLevel::Type InFeatureLevel) const {
        return Parent->GetFallback(InFeatureLevel);
    }

    bool FFlipbookTextureOverrideRenderProxy::GetParameterValue(EMaterialParameterType Type,
        const FHashedMaterialParameterInfo &ParameterInfo, FMaterialParameterValue &OutValue,
        const FMaterialRenderContext &Context) const {
        if (Type == EMaterialParameterType::Texture)
        {
            if (ParameterInfo.Name == TextureParameterName)
            {
                OutValue = ApplyEditorOverrides(BaseTexture);
                return true;
            }

            for (int32 AdditionalSlotIndex = 0; AdditionalSlotIndex < AdditionalTextures.Num(); ++AdditionalSlotIndex)
            {
                FName AdditionalSlotName = AdditionalTextureParameterRootName;
                AdditionalSlotName.SetNumber(AdditionalSlotIndex + 1);
                if (ParameterInfo.Name == AdditionalSlotName)
                {
                    OutValue = ApplyEditorOverrides(AdditionalTextures[AdditionalSlotIndex]);
                    return true;
                }
            }
        }

        return Parent->GetParameterValue(Type, ParameterInfo, OutValue, Context);
    }

    const UTexture * FFlipbookTextureOverrideRenderProxy::ApplyEditorOverrides(const UTexture *InTexture) const {
#if WITH_EDITOR
        if (TextureOverrideList.Num() > 0)
        {
            if (const UTexture* const* OverridePtr = TextureOverrideList.Find(InTexture))
            {
                return *OverridePtr;
            }
        }

        return InTexture;
#else
		return InTexture;
#endif
    }
}