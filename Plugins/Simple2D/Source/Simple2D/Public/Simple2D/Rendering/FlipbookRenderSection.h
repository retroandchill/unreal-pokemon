// "Unreal Pokémon" created by Retro & Chill.

#pragma once
#include "Paper2DModule.h"
#include "PaperSprite.h"
#include "SpriteDrawCall.h"
#include "PackedNormal.h"
#include "DynamicMeshBuilder.h"

class FTexture;

namespace Simple2D {
    /** A Paper2D sprite vertex. */
    struct SIMPLE2D_API FSimpleFlipbookTangents {
        FORCEINLINE static FPackedNormal GetPackedNormalX() {
            return PackedNormalX;
        }

        FORCEINLINE static FPackedNormal GetPackedNormalZ() {
            return PackedNormalZ;
        }

      private:
        friend class FSimple2DModule;

        static void SetTangentsFromPaperAxes();

        static FPackedNormal PackedNormalX;
        static FPackedNormal PackedNormalZ;
    };

    /**
     *
     */
    struct SIMPLE2D_API FFlipbookRenderSection {
        UMaterialInterface *Material = nullptr;
        UTexture *BaseTexture = nullptr;
        FAdditionalSpriteTextureArray AdditionalTextures;
        int32 Rows = 1;
        int32 Columns = 1;
        int32 FrameNumber = 0;

        int32 VertexOffset = INDEX_NONE;
        int32 NumVertices = 0;

        FTexture *GetBaseTextureResource() const;

        bool IsValid() const;

        template <typename T>
        void AddVerticesFromDrawCallRecord(const FSpriteDrawCallRecord &Record, int32 StartIndexWithinRecord,
                                           int32 NumVertsToCopy, TArray<FDynamicMeshVertex, T> &Vertices) {
            if (NumVertices == 0) {
                VertexOffset = Vertices.Num();
                BaseTexture = Record.BaseTexture;
                AdditionalTextures = Record.AdditionalTextures;
            } else {
                checkSlow((VertexOffset + NumVertices) == Vertices.Num());
                checkSlow(BaseTexture == Record.BaseTexture);
                // Note: Not checking AdditionalTextures for now, since checking BaseTexture should catch most bugs
            }

            NumVertices += NumVertsToCopy;
            Vertices.Reserve(Vertices.Num() + NumVertsToCopy);

            const FColor VertColor(Record.Color);
            for (int32 VertexIndex = StartIndexWithinRecord; VertexIndex < StartIndexWithinRecord + NumVertsToCopy;
                 ++VertexIndex) {
                const FVector4 &SourceVert = Record.RenderVerts[VertexIndex];

                const FVector Pos((PaperAxisX * SourceVert.X) + (PaperAxisY * SourceVert.Y) + Record.Destination);
                const FVector2f UV(static_cast<float>(SourceVert.Z), static_cast<float>(SourceVert.W));

                Vertices.Emplace(static_cast<FVector3f>(Pos), FSimpleFlipbookTangents::GetPackedNormalX().ToFVector3f(),
                                 FSimpleFlipbookTangents::GetPackedNormalZ().ToFVector3f(), UV, VertColor);
            }
        }
    };
} // namespace Simple2D