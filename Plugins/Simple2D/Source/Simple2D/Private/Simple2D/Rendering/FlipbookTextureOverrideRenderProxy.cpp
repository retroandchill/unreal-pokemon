// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Rendering/FlipbookTextureOverrideRenderProxy.h"
#include "MovieSceneSequencePlayer.h"

namespace Simple2D {
    const FName FFlipbookTextureOverrideRenderProxy::TextureParameterName(TEXT("SpriteTexture"));
    const FName FFlipbookTextureOverrideRenderProxy::AdditionalTextureParameterRootName(TEXT("SpriteAdditionalTexture"));
    const FName FFlipbookTextureOverrideRenderProxy::RowsParameterName(TEXT("Rows"));
    const FName FFlipbookTextureOverrideRenderProxy::ColumnsParameterName(TEXT("Columns"));
    const FName FFlipbookTextureOverrideRenderProxy::FrameParameterName(TEXT("Frame"));
    
#if WITH_EDITOR
    FFlipbookTextureOverrideRenderProxy::FFlipbookTextureOverrideRenderProxy(const FMaterialRenderProxy *InParent,
        const UTexture *InBaseTexture, FAdditionalSpriteTextureArray InAdditionalTextures, int32 Rows, int32 Columns, int32 FrameNumber,
        const FFlipbookRenderSceneProxyTextureOverrideMap &InTextureOverrideList)
    #else
    FFlipbookTextureOverrideRenderProxy::FFlipbookTextureOverrideRenderProxy(const FMaterialRenderProxy *InParent,
        const UTexture *InBaseTexture, FAdditionalSpriteTextureArray InAdditionalTextures, int32 Rows, int32 Columns, int32 FrameNumber) 
    #endif
        : FMaterialRenderProxy(InParent ? InParent->GetMaterialName() : TEXT("FSpriteTextureOverrideRenderProxy")),
          Parent(InParent), BaseTexture(InBaseTexture), AdditionalTextures(InAdditionalTextures), Rows(Rows), Columns(Columns), FrameNumber(FrameNumber),
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
                                                           const UTexture *InBaseTexture, FAdditionalSpriteTextureArray InAdditionalTextures, int32 NewRows, int32 NewColumns, int32
                                                           NewFrameNumber) {
        if ((InParent != Parent) || (InBaseTexture != BaseTexture) || (InAdditionalTextures != AdditionalTextures))
        {
            Parent = InParent;
            BaseTexture = InBaseTexture;
            AdditionalTextures = InAdditionalTextures;
            ParentMaterialSerialNumber = INDEX_NONE;
            Rows = NewRows;
            Columns = NewColumns;
            FrameNumber = NewFrameNumber;
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
        } else if (Type == EMaterialParameterType::Scalar) {
            if (ParameterInfo.Name == RowsParameterName) {
                OutValue = static_cast<float>(Rows);
                    return true;
            }
            
            if (ParameterInfo.Name == ColumnsParameterName) {
                OutValue = static_cast<float>(Columns);
                return true;
            }
            
            if (ParameterInfo.Name == FrameParameterName) {
                OutValue = static_cast<float>(FrameNumber);
                return true;
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

    bool FFlipbookTextureOverrideRenderProxy::FrameDataMatches(int32 OtherRows, int32 OtherColumns,
                                                               int32 OtherFrame) const {
        return Rows == OtherRows && Columns == OtherColumns && FrameNumber == OtherFrame;
    }

    void FFlipbookTextureOverrideRenderProxy::UpdateFrameData(int32 NewRows, int32 NewColumns, int32 NewFrame) {
        Rows = NewRows;
        Columns = NewColumns;
        FrameNumber = NewFrame;
        InvalidateUniformExpressionCache(false);
    }
}