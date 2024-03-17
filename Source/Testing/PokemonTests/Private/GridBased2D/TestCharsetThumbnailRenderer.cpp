// "Unreal Pokémon" created by Retro & Chill.


#include "GridBased2D/TestCharsetThumbnailRenderer.h"

#include "CommonDefines.h"
#include "PaperFlipbook.h"
#include "Characters/Charset.h"
#include "Memory/GCPointer.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "Charset/CharsetThumbnailRenderer.h"

#include "Utilities/fakeit.hpp"

using namespace fakeit;

// Sets default values
ATestCharsetThumbnailRenderer::ATestCharsetThumbnailRenderer() {
	OnTestStart.AddDynamic(this, &ATestCharsetThumbnailRenderer::RunTest);
}

void ATestCharsetThumbnailRenderer::RunTest() {
	TGCPointer Renderer(NewObject<UCharsetThumbnailRenderer>());
	TGCPointer InvalidCharset(NewObject<UCharset>());
	TEST_ASSERT(AssertIsValid(ValidCharset, TEXT("Valid Charset Must Exist")));
	TEST_ASSERT(AssertIsValid(ValidCharset->GetDownSprite(), TEXT("Valid Charset Must Have a Down Sprite")));

	TEST_ASSERT(AssertTrue(Renderer->GetThumbnailRenderFrequency(nullptr) == EThumbnailRenderFrequency::Realtime,
		TEXT("Realtime Thumbnails")));
	TEST_ASSERT(AssertTrue(Renderer->GetThumbnailRenderFrequency(InvalidCharset.Get()) == EThumbnailRenderFrequency::Realtime,
		TEXT("Realtime Thumbnails")));
	TEST_ASSERT(AssertTrue(Renderer->GetThumbnailRenderFrequency(ValidCharset) == EThumbnailRenderFrequency::Realtime,
		TEXT("Realtime Thumbnails")));
	
	TEST_ASSERT(AssertFalse(Renderer->CanVisualizeAsset(nullptr), TEXT("Can Visualize")));
	TEST_ASSERT(AssertFalse(Renderer->CanVisualizeAsset(InvalidCharset.Get()), TEXT("Can Visualize")));
	TEST_ASSERT(AssertTrue(Renderer->CanVisualizeAsset(ValidCharset), TEXT("Can Visualize")));

	// TODO: Figure out a way to test draw
	
	FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test Passed!"));
}

