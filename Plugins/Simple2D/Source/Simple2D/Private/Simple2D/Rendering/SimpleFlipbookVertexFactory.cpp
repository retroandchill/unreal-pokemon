// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Rendering/SimpleFlipbookVertexFactory.h"
#include "Simple2D/Rendering/SimpleFlipbookVertexBuffer.h"

Simple2D::FSimpleFlipbookVertexFactory::FSimpleFlipbookVertexFactory(ERHIFeatureLevel::Type FeatureLevel)
    : FLocalVertexFactory(FeatureLevel, "FSimpleFlipbookVertexFactory")
{
}

void Simple2D::FSimpleFlipbookVertexFactory::Init(FRHICommandListBase &RHICmdList,
                                                  const FSimpleFlipbookVertexBuffer *InVertexBuffer)
{
    FDataType VertexData;
    VertexData.NumTexCoords = 1;

    // SRV setup
    VertexData.LightMapCoordinateIndex = 0;
    VertexData.TangentsSRV = InVertexBuffer->TangentBufferSRV;
    VertexData.TextureCoordinatesSRV = InVertexBuffer->TexCoordBufferSRV;
    VertexData.ColorComponentsSRV = InVertexBuffer->ColorBufferSRV;
    VertexData.PositionComponentSRV = InVertexBuffer->PositionBufferSRV;

    // Vertex Streams
    VertexData.PositionComponent = FVertexStreamComponent(&InVertexBuffer->PositionBuffer, 0, sizeof(FVector3f),
                                                          VET_Float3, EVertexStreamUsage::Default);
    VertexData.TangentBasisComponents[0] =
        FVertexStreamComponent(&InVertexBuffer->TangentBuffer, 0, 2 * sizeof(FPackedNormal), VET_PackedNormal,
                               EVertexStreamUsage::ManualFetch);
    VertexData.TangentBasisComponents[1] =
        FVertexStreamComponent(&InVertexBuffer->TangentBuffer, sizeof(FPackedNormal), 2 * sizeof(FPackedNormal),
                               VET_PackedNormal, EVertexStreamUsage::ManualFetch);
    VertexData.ColorComponent = FVertexStreamComponent(&InVertexBuffer->ColorBuffer, 0, sizeof(FColor), VET_Color,
                                                       EVertexStreamUsage::ManualFetch);
    VertexData.TextureCoordinates.Add(FVertexStreamComponent(&InVertexBuffer->TexCoordBuffer, 0, sizeof(FVector2f),
                                                             VET_Float2, EVertexStreamUsage::ManualFetch));

    SetData(RHICmdList, VertexData);
    VertexBuffer = InVertexBuffer;

    InitResource(RHICmdList);
}