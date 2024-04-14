// "Unreal Pokémon" created by Retro & Chill.

#include "RPGMenus/TestWindowskinThumbnailRenderer.h"
#include "CommonDefines.h"
#include "Data/Windowskin.h"
#include "Memory/GCPointer.h"
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "Utilities/fakeit.hpp"
#include "Windowskin/WindowskinThumbnailRenderer.h"

using namespace fakeit;

void ATestWindowskinThumbnailRenderer::BeginPlay() {
    Super::BeginPlay();
    OnTestStart.AddDynamic(this, &ATestWindowskinThumbnailRenderer::TestRun);
}

void ATestWindowskinThumbnailRenderer::TestRun() {
    TGCPointer Renderer(NewObject<UWindowskinThumbnailRenderer>());
    TGCPointer InvalidWindowskin(NewObject<UWindowskin>());
    TEST_ASSERT(AssertIsValid(ValidWindowskin, TEXT("Valid Windowskin Must Exist")));
    TEST_ASSERT(AssertIsValid(ValidWindowskin->GetSourceTexture(), TEXT("Valid Windowskin Must Have a Texture")));

    TEST_ASSERT(AssertTrue(Renderer->GetThumbnailRenderFrequency(nullptr) == EThumbnailRenderFrequency::Realtime,
                           TEXT("Realtime Thumbnails")));
    TEST_ASSERT(AssertTrue(Renderer->GetThumbnailRenderFrequency(InvalidWindowskin.Get()) ==
                               EThumbnailRenderFrequency::Realtime,
                           TEXT("Realtime Thumbnails")));
    TEST_ASSERT(
        AssertTrue(Renderer->GetThumbnailRenderFrequency(ValidWindowskin) == EThumbnailRenderFrequency::Realtime,
                   TEXT("Realtime Thumbnails")));

    TEST_ASSERT(AssertFalse(Renderer->CanVisualizeAsset(nullptr), TEXT("Can Visualize")));
    TEST_ASSERT(AssertFalse(Renderer->CanVisualizeAsset(InvalidWindowskin.Get()), TEXT("Can Visualize")));
    TEST_ASSERT(AssertTrue(Renderer->CanVisualizeAsset(ValidWindowskin), TEXT("Can Visualize")));

    // TODO: Figure out a way to test draw

    FinishTest(EFunctionalTestResult::Succeeded, TEXT("Test Passed!"));
}
