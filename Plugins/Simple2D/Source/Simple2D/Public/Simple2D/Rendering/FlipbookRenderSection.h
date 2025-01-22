// "Unreal Pokémon" created by Retro & Chill.

#pragma once
#include "Paper2DModule.h"
#include "PaperSprite.h"
#include "SpriteDrawCall.h"


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
        UMaterialInterface *Material;
        UTexture *BaseTexture;
        FAdditionalSpriteTextureArray AdditionalTextures;

        int32 VertexOffset;
        int32 NumVertices;

        FFlipbookRenderSection()
            : Material(nullptr)
              , BaseTexture(nullptr)
              , VertexOffset(INDEX_NONE)
              , NumVertices(0) {
        }

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
            for (int32 VertexIndex = StartIndexWithinRecord; VertexIndex < StartIndexWithinRecord + NumVertsToCopy; ++
                 VertexIndex) {
                const FVector4 &SourceVert = Record.RenderVerts[VertexIndex];

                const FVector Pos((PaperAxisX * SourceVert.X) + (PaperAxisY * SourceVert.Y) + Record.Destination);
                const FVector2f UV(SourceVert.Z, SourceVert.W); // LWC_TODO: Precision loss

                Vertices.Emplace((FVector3f)Pos, FSimpleFlipbookTangents::GetPackedNormalX().ToFVector3f(),
                                 FSimpleFlipbookTangents::GetPackedNormalZ().ToFVector3f(), UV, VertColor);
            }
        }

        template <typename T>
        void AddVertex(float X, float Y, float U, float V, const FVector &Origin, const FColor &Color,
                       TArray<FDynamicMeshVertex, T> &Vertices) {
            const FVector Pos((PaperAxisX * X) + (PaperAxisY * Y) + Origin);

            Vertices.Emplace(FVector3f(Pos), FSimpleFlipbookTangents::GetPackedNormalX().ToFVector3f(),
                             FSimpleFlipbookTangents::GetPackedNormalZ().ToFVector3f(), FVector2f(U, V), Color);
            ++NumVertices;
        }

        template <typename T>
        void AddVertex(float X, float Y, float U, float V, const FVector &Origin, const FColor &Color,
                       const FPackedNormal &TangentX, const FPackedNormal &TangentZ,
                       TArray<FDynamicMeshVertex, T> &Vertices) {
            const FVector Pos((PaperAxisX * X) + (PaperAxisY * Y) + Origin);

            Vertices.Emplace(FVector3f(Pos), TangentX.ToFVector3f(), TangentZ.ToFVector3f(), FVector2f(U, V), Color);
            ++NumVertices;
        }
    };
}