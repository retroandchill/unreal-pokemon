// "Unreal Pokémon" created by Retro & Chill.
#include "GridBased2DEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Characters/Charset.h"
#include "Charset/CharsetAssetActions.h"
#include "Charset/CharsetThumbnailRenderer.h"
#include "Details/GridBasedBillboardCharacterDetails.h"
#include "Details/GridBasedMapDetails.h"

constexpr auto GLoctextNamespace = "FGridBased2DEditorModule";

void FGridBased2DEditorModule::StartupModule() {
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FGridBased2DEditorModule::OnPostEngineInit);

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout("GridBasedMap", FOnGetDetailCustomizationInstance::CreateStatic(&FGridBasedMapDetails::MakeInstance));
	PropertyModule.RegisterCustomClassLayout("GridBasedBillboardCharacter", FOnGetDetailCustomizationInstance::CreateStatic(&FGridBasedBillboardCharacterDetails::MakeInstance));
}

void FGridBased2DEditorModule::OnPostEngineInit() const {
	// Register asset types
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	auto Paper2DAssetCategoryBit = AssetTools.FindAdvancedAssetCategory(FName(TEXT("Paper2D")));
	AssetTools.RegisterAssetTypeActions(MakeShared<FCharsetAssetActions>(Paper2DAssetCategoryBit));

	// Register thubmnails
	UThumbnailManager::Get().RegisterCustomRenderer(UCharset::StaticClass(), UCharsetThumbnailRenderer::StaticClass());
}

void FGridBased2DEditorModule::ShutdownModule() {
	// No special shutdown required
}

IMPLEMENT_MODULE(FGridBased2DEditorModule, GridBased2DEditor)
