// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Rendering/SimpleFlipbookVertexBuffer.h"

void Simple2D::FSimpleFlipbookVertexBuffer::SetDynamicUsage(bool bInDynamicUsage) {
    bDynamicUsage = bInDynamicUsage;
}

void Simple2D::FSimpleFlipbookVertexBuffer::CreateBuffers(FRHICommandListBase &RHICmdList, int32 NumVertices) {
    // Make sure we don't have dangling buffers
    if (NumAllocatedVertices > 0) {
        ReleaseBuffers();
    }

    // The buffer will always be a shader resource, but they can be static/dynamic depending of the usage
    const EBufferUsageFlags Usage = BUF_ShaderResource | (bDynamicUsage ? BUF_Dynamic : BUF_Static);
    NumAllocatedVertices = NumVertices;

    uint32 PositionSize = NumAllocatedVertices * sizeof(FVector3f);
    // create vertex buffer
    {
        FRHIResourceCreateInfo CreateInfo(TEXT("PaperSpritePositionBuffer"));
        PositionBuffer.VertexBufferRHI = RHICmdList.CreateVertexBuffer(PositionSize, Usage, CreateInfo);
        if (RHISupportsManualVertexFetch(GMaxRHIShaderPlatform)) {
            PositionBufferSRV =
                RHICmdList.CreateShaderResourceView(PositionBuffer.VertexBufferRHI, sizeof(float), PF_R32_FLOAT);
        }
    }

    uint32 TangentSize = NumAllocatedVertices * 2 * sizeof(FPackedNormal);
    // create vertex buffer
    {
        FRHIResourceCreateInfo CreateInfo(TEXT("PaperSpriteTangentBuffer"));
        TangentBuffer.VertexBufferRHI = RHICmdList.CreateVertexBuffer(TangentSize, Usage, CreateInfo);
        if (RHISupportsManualVertexFetch(GMaxRHIShaderPlatform)) {
            TangentBufferSRV = RHICmdList.CreateShaderResourceView(TangentBuffer.VertexBufferRHI, sizeof(FPackedNormal),
                                                                   PF_R8G8B8A8_SNORM);
        }
    }

    uint32 TexCoordSize = NumAllocatedVertices * sizeof(FVector2f);
    // create vertex buffer
    {
        FRHIResourceCreateInfo CreateInfo(TEXT("PaperSpriteTexCoordBuffer"));
        TexCoordBuffer.VertexBufferRHI = RHICmdList.CreateVertexBuffer(TexCoordSize, Usage, CreateInfo);
        if (RHISupportsManualVertexFetch(GMaxRHIShaderPlatform)) {
            TexCoordBufferSRV =
                RHICmdList.CreateShaderResourceView(TexCoordBuffer.VertexBufferRHI, sizeof(FVector2f), PF_G32R32F);
        }
    }

    uint32 ColorSize = NumAllocatedVertices * sizeof(FColor);
    // create vertex buffer
    {
        FRHIResourceCreateInfo CreateInfo(TEXT("PaperSpriteColorBuffer"));
        ColorBuffer.VertexBufferRHI = RHICmdList.CreateVertexBuffer(ColorSize, Usage, CreateInfo);
        if (RHISupportsManualVertexFetch(GMaxRHIShaderPlatform)) {
            ColorBufferSRV =
                RHICmdList.CreateShaderResourceView(ColorBuffer.VertexBufferRHI, sizeof(FColor), PF_R8G8B8A8);
        }
    }

    // Create Index Buffer
    {
        FRHIResourceCreateInfo CreateInfo(TEXT("PaperSpriteIndexBuffer"));
        IndexBuffer.IndexBufferRHI =
            RHICmdList.CreateIndexBuffer(sizeof(uint32), Vertices.Num() * sizeof(uint32), Usage, CreateInfo);
    }
}

void Simple2D::FSimpleFlipbookVertexBuffer::ReleaseBuffers() {
    PositionBuffer.ReleaseRHI();
    TangentBuffer.ReleaseRHI();
    TexCoordBuffer.ReleaseRHI();
    ColorBuffer.ReleaseRHI();
    IndexBuffer.ReleaseRHI();

    TangentBufferSRV.SafeRelease();
    TexCoordBufferSRV.SafeRelease();
    ColorBufferSRV.SafeRelease();
    PositionBufferSRV.SafeRelease();

    NumAllocatedVertices = 0;
}

void Simple2D::FSimpleFlipbookVertexBuffer::CommitVertexData(FRHICommandListBase &RHICmdList) {
    if (Vertices.Num()) {
        // Check if we have to accommodate the buffer size
        if (NumAllocatedVertices != Vertices.Num()) {
            CreateBuffers(RHICmdList, Vertices.Num());
        }

        // Lock vertices
        FVector3f *PositionBufferData = nullptr;
        uint32 PositionSize = Vertices.Num() * sizeof(FVector3f);
        {
            void *Data = RHICmdList.LockBuffer(PositionBuffer.VertexBufferRHI, 0, PositionSize, RLM_WriteOnly);
            PositionBufferData = static_cast<FVector3f *>(Data);
        }

        FPackedNormal *TangentBufferData = nullptr;
        uint32 TangentSize = Vertices.Num() * 2 * sizeof(FPackedNormal);
        {
            void *Data = RHICmdList.LockBuffer(TangentBuffer.VertexBufferRHI, 0, TangentSize, RLM_WriteOnly);
            TangentBufferData = static_cast<FPackedNormal *>(Data);
        }

        FVector2f *TexCoordBufferData = nullptr;
        uint32 TexCoordSize = Vertices.Num() * sizeof(FVector2f);
        {
            void *Data = RHICmdList.LockBuffer(TexCoordBuffer.VertexBufferRHI, 0, TexCoordSize, RLM_WriteOnly);
            TexCoordBufferData = static_cast<FVector2f *>(Data);
        }

        FColor *ColorBufferData = nullptr;
        uint32 ColorSize = Vertices.Num() * sizeof(FColor);
        {
            void *Data = RHICmdList.LockBuffer(ColorBuffer.VertexBufferRHI, 0, ColorSize, RLM_WriteOnly);
            ColorBufferData = static_cast<FColor *>(Data);
        }

        uint32 *IndexBufferData = nullptr;
        uint32 IndexSize = Vertices.Num() * sizeof(uint32);
        {
            void *Data = RHICmdList.LockBuffer(IndexBuffer.IndexBufferRHI, 0, IndexSize, RLM_WriteOnly);
            IndexBufferData = static_cast<uint32 *>(Data);
        }

        // Fill verts
        for (int32 i = 0; i < Vertices.Num(); i++) {
            PositionBufferData[i] = (FVector3f)Vertices[i].Position;
            TangentBufferData[2 * i + 0] = Vertices[i].TangentX;
            TangentBufferData[2 * i + 1] = Vertices[i].TangentZ;
            ColorBufferData[i] = Vertices[i].Color;
            TexCoordBufferData[i] = Vertices[i].TextureCoordinate[0];
            IndexBufferData[i] = i;
        }

        // Unlock the buffer.
        RHICmdList.UnlockBuffer(PositionBuffer.VertexBufferRHI);
        RHICmdList.UnlockBuffer(TangentBuffer.VertexBufferRHI);
        RHICmdList.UnlockBuffer(TexCoordBuffer.VertexBufferRHI);
        RHICmdList.UnlockBuffer(ColorBuffer.VertexBufferRHI);
        RHICmdList.UnlockBuffer(IndexBuffer.IndexBufferRHI);

        // We clear the vertex data, as it isn't needed anymore
        Vertices.Empty();
    }
}

void Simple2D::FSimpleFlipbookVertexBuffer::InitRHI(FRHICommandListBase &RHICmdList) {
    // Automatically try to create the data and use it
    CommitVertexData(RHICmdList);
}

void Simple2D::FSimpleFlipbookVertexBuffer::ReleaseRHI() {
    PositionBuffer.ReleaseRHI();
    TangentBuffer.ReleaseRHI();
    TexCoordBuffer.ReleaseRHI();
    ColorBuffer.ReleaseRHI();
    IndexBuffer.ReleaseRHI();

    TangentBufferSRV.SafeRelease();
    TexCoordBufferSRV.SafeRelease();
    ColorBufferSRV.SafeRelease();
    PositionBufferSRV.SafeRelease();
}

void Simple2D::FSimpleFlipbookVertexBuffer::InitResource(FRHICommandListBase &RHICmdList) {
    FRenderResource::InitResource(RHICmdList);
    PositionBuffer.InitResource(RHICmdList);
    TangentBuffer.InitResource(RHICmdList);
    TexCoordBuffer.InitResource(RHICmdList);
    ColorBuffer.InitResource(RHICmdList);
    IndexBuffer.InitResource(RHICmdList);
}

void Simple2D::FSimpleFlipbookVertexBuffer::ReleaseResource() {
    FRenderResource::ReleaseResource();
    PositionBuffer.ReleaseResource();
    TangentBuffer.ReleaseResource();
    TexCoordBuffer.ReleaseResource();
    ColorBuffer.ReleaseResource();
    IndexBuffer.ReleaseResource();
}