// "Unreal Pokémon" created by Retro & Chill.

#pragma once

namespace Simple2D {
    class FSimpleFlipbookVertexBuffer;
    /**
     * 
     */
    class SIMPLE2D_API FSimpleFlipbookVertexFactory : public FLocalVertexFactory {
    public:
        explicit FSimpleFlipbookVertexFactory(ERHIFeatureLevel::Type FeatureLevel);
        
        /* Initializes this factory with a given vertex buffer. */
        void Init(FRHICommandListBase& RHICmdList, const FSimpleFlipbookVertexBuffer* InVertexBuffer);

    private:
        /* Vertex buffer used to initialize this factory. */
        const FSimpleFlipbookVertexBuffer* VertexBuffer = nullptr;
    };
}