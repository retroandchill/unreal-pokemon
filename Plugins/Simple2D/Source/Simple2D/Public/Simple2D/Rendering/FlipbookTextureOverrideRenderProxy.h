// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialRenderProxy.h"
#include "PaperSprite.h"

namespace Simple2D {
    using FFlipbookRenderSceneProxyTextureOverrideMap = TMap<const UTexture *, const UTexture *>;

    class SIMPLE2D_API FFlipbookTextureOverrideRenderProxy : public FDynamicPrimitiveResource,
                                                             public FMaterialRenderProxy {
      public:
#if WITH_EDITOR
        FFlipbookTextureOverrideRenderProxy(const FMaterialRenderProxy *InParent, const UTexture *InBaseTexture,
                                            FAdditionalSpriteTextureArray InAdditionalTextures, int32 Rows,
                                            int32 Columns, int32 FrameNumber,
                                            const FFlipbookRenderSceneProxyTextureOverrideMap &InTextureOverrideList);
#else
        FFlipbookTextureOverrideRenderProxy(const FMaterialRenderProxy *InParent, const UTexture *InBaseTexture,
                                            FAdditionalSpriteTextureArray InAdditionalTextures, int32 Rows,
                                            int32 Columns, int32 FrameNumber);
#endif

        bool CheckValidity(const FMaterialRenderProxy *InCurrentParent);

        void Reinitialize(const FMaterialRenderProxy *InParent, const UTexture *InBaseTexture,
                          FAdditionalSpriteTextureArray InAdditionalTextures, int32 NewRows, int32 NewColumns,
                          int32 NewFrameNumber);

        void InitPrimitiveResource(FRHICommandListBase &RHICmdList) override;
        void ReleasePrimitiveResource() override;

        const FMaterial *GetMaterialNoFallback(ERHIFeatureLevel::Type InFeatureLevel) const override;
        const FMaterialRenderProxy *GetFallback(ERHIFeatureLevel::Type InFeatureLevel) const override;
        bool GetParameterValue(EMaterialParameterType Type, const FHashedMaterialParameterInfo &ParameterInfo,
                               FMaterialParameterValue &OutValue, const FMaterialRenderContext &Context) const override;

        const UTexture *ApplyEditorOverrides(const UTexture *InTexture) const;

        bool FrameDataMatches(int32 OtherRows, int32 OtherColumns, int32 OtherFrame) const;
        void UpdateFrameData(int32 NewRows, int32 NewColumns, int32 NewFrame);

      private:
        const FMaterialRenderProxy *Parent;
        const UTexture *BaseTexture;
        FAdditionalSpriteTextureArray AdditionalTextures;
        int32 Rows;
        int32 Columns;
        int32 FrameNumber;

#if WITH_EDITOR
        const FFlipbookRenderSceneProxyTextureOverrideMap &TextureOverrideList;
#endif

        // Used to track if we need to rebuild our texture param setting proxy
        int32 ParentMaterialSerialNumber = INDEX_NONE;
    };
} // namespace Simple2D
