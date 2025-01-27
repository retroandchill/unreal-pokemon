// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Rendering/FlipbookRenderSection.h"

namespace Simple2D {
    FPackedNormal FSimpleFlipbookTangents::PackedNormalX(FVector(1.0f, 0.0f, 0.0f));
    FPackedNormal FSimpleFlipbookTangents::PackedNormalZ(FVector(0.0f, -1.0f, 0.0f));

    void FSimpleFlipbookTangents::SetTangentsFromPaperAxes() {
        PackedNormalX = PaperAxisX;
        PackedNormalZ = -PaperAxisZ;
        // store determinant of basis in w component of normal vector
        PackedNormalZ.Vector.W = GetBasisDeterminantSignByte(PaperAxisX, PaperAxisY, PaperAxisZ);
    }

    FTexture *FFlipbookRenderSection::GetBaseTextureResource() const {
        return BaseTexture != nullptr ? BaseTexture->GetResource() : nullptr;
    }

    bool FFlipbookRenderSection::IsValid() const {
        return (Material != nullptr) && (NumVertices > 0) && (GetBaseTextureResource() != nullptr);
    }
} // namespace Simple2D