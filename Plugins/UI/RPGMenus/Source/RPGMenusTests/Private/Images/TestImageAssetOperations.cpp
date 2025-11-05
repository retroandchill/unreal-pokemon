#include "Asserts.h"
#include "Assets/AssetLoader.h"
#include "Images/Utilities/ImageAssetUtility.h"
#include "Misc/AutomationTest.h"
#include "PaperSprite.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestImageAssetOperations, "Unit Tests.Images.TestImageAssetOperations",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestImageAssetOperations::RunTest(const FString &Parameters) {
    auto UtilityClass = UAssetLoader::LookupBlueprintClassByName<UImageAssetUtility>(TEXT("/RPGMenus/Tests/Resources"),
                                                                                     TEXT("ImageAssetUtilityImpl"));
    auto Class = UtilityClass.GetPtrOrNull();
    UE_ASSERT_NOT_NULL(Class);

    auto Utility = NewObject<UImageAssetUtility>(GetTransientPackage(), Class->Get());
    UE_ASSERT_NOT_NULL(Utility);

    auto Variants = Utility->CreateImageAssets(Utility->TextureAsset, Utility->SpriteAsset);
    UE_ASSERT_EQUAL(2, Variants.Num());
    UE_ASSERT_TRUE(Variants[0].IsType<UTexture2D>());
    UE_ASSERT_TRUE(Variants[1].IsType<ISlateTextureAtlasInterface>());

    FImageAsset OutAsset;
    UE_CHECK_FALSE(Utility->TryCreateImageAsset(Utility->InvalidAsset, OutAsset));
    UE_CHECK_FALSE(OutAsset.IsValid());

    UE_CHECK_TRUE(Utility->TryCreateImageAsset(Utility->TextureAsset, OutAsset));
    UE_CHECK_TRUE(OutAsset.IsType<UTexture2D>());
    UE_CHECK_NOT_NULL(Utility->GetObjectFromImageAsset(OutAsset));

    UTexture2D *Texture = nullptr;
    UE_CHECK_FALSE(Utility->GetTextureFromImageAsset(Variants[1], Texture));
    UE_CHECK_TRUE(Utility->GetTextureFromImageAsset(Variants[0], Texture));
    UE_CHECK_NOT_NULL(Texture);

    TScriptInterface<ISlateTextureAtlasInterface> Sprite;
    UE_CHECK_FALSE(Utility->GetSpriteFromImageAsset(Variants[0], Sprite));
    UE_CHECK_TRUE(Utility->GetSpriteFromImageAsset(Variants[1], Sprite));
    UE_CHECK_NOT_NULL(Sprite.GetObject());

    auto SoftAsset = Utility->MakeSoftImageAsset(Variants[0]);
    UE_CHECK_EQUAL(FImageAsset::GetTypeIndex<UTexture2D>(), SoftAsset.GetTypeIndex());

    auto MaterialReference = Utility->MakeSoftImageAssetFromMaterial(Utility->MaterialAsset);
    UE_CHECK_EQUAL(FImageAsset::GetTypeIndex<UMaterialInterface>(), MaterialReference.GetTypeIndex());

    FSoftImageAsset OutSoftAsset;
    UE_CHECK_FALSE(Utility->TryMakeSoftImageAsset(Utility->InvalidSoftAsset, OutSoftAsset));
    UE_CHECK_TRUE(Utility->TryMakeSoftImageAsset(Utility->MaterialAsset, OutSoftAsset));
    UE_CHECK_EQUAL(FImageAsset::GetTypeIndex<UMaterialInterface>(), OutSoftAsset.GetTypeIndex());

    FSoftImageAsset InvalidAsset;
    TSoftObjectPtr<UMaterialInterface> Material;
    UE_CHECK_FALSE(Utility->GetSoftMaterialFromImageAsset(InvalidAsset, Material));
    UE_CHECK_TRUE(Utility->GetSoftMaterialFromImageAsset(OutSoftAsset, Material));

    FImageAsset OutLoaded;
    UE_CHECK_FALSE(Utility->LoadSynchronous(InvalidAsset, OutLoaded));
    UE_CHECK_TRUE(Utility->LoadSynchronous(OutSoftAsset, OutLoaded));
    UE_CHECK_TRUE(OutLoaded.IsType<UMaterialInterface>());

    return true;
}