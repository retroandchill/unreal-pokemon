﻿#include "Asserts.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Data/Windowskin.h"
#include "Misc/AutomationTest.h"
#include "Utilities/GCPointer.h"
#include "Windowskin/WindowskinThumbnailRenderer.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestWindowskinThumbnailRenderer, "Unit Tests.RPGMenus.TestWindowskinThumbnailRenderer",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestWindowskinThumbnailRenderer::RunTest(const FString &Parameters) {
    TGCPointer Renderer(NewObject<UWindowskinThumbnailRenderer>());
    TGCPointer InvalidWindowskin(NewObject<UWindowskin>());

    auto WindowskinAsset = FAssetRegistryModule::GetRegistry().GetAssetByObjectPath(
        FSoftObjectPath(TEXT("/RPGMenus/Tests/Resources/choice_1_Win.choice_1_Win")));
    auto ValidWindowskin = Cast<UWindowskin>(WindowskinAsset.GetAsset());
    UE_ASSERT_NOT_NULL(ValidWindowskin);
    UE_ASSERT_NOT_NULL(ValidWindowskin->GetSourceTexture());

    UE_CHECK_EQUAL(EThumbnailRenderFrequency::Realtime, Renderer->GetThumbnailRenderFrequency(nullptr));
    UE_CHECK_EQUAL(EThumbnailRenderFrequency::Realtime, Renderer->GetThumbnailRenderFrequency(InvalidWindowskin.Get()));
    UE_CHECK_EQUAL(EThumbnailRenderFrequency::Realtime, Renderer->GetThumbnailRenderFrequency(ValidWindowskin));

    UE_CHECK_FALSE(Renderer->CanVisualizeAsset(nullptr));
    UE_CHECK_FALSE(Renderer->CanVisualizeAsset(InvalidWindowskin.Get()));
    UE_CHECK_TRUE(Renderer->CanVisualizeAsset(ValidWindowskin));

    // TODO: Figure out a way to test draw

    return true;
}