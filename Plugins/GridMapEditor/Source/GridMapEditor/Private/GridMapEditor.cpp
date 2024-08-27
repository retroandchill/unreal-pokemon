// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GridMapEditor.h"
#include "AssetToolsModule.h"
#include "GridMapEditCommands.h"
#include "GridMapEditorMode.h"
#include "GridMapTileSetAssetTypeActions.h"
#include "GridMapStyleSet.h"
#include "Interfaces/IPluginManager.h"
#include "PropertyEditorModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "ThumbnailRendering/ThumbnailManager.h"
#include "TileBitsetCustomization.h"
#include "TileSet.h"
#include "TileSet_ThumbnailRenderer.h"

#define LOCTEXT_NAMESPACE "FGridMapEditorModule"

// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
void FGridMapEditorModule::StartupModule()
{
	// Icons
	FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("GridMapEditor"))->GetContentDir() + "/";
	StyleSet = MakeShareable(new FGridMapStyleSet(ContentDir));
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());

	// Property Customization
	RegisterCustomPropertyTypeLayout("GridMapTileBitset", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTileBitsetCustomization::MakeInstance));

	// Custom thumbnail renderes
	UThumbnailManager::Get().RegisterCustomRenderer(UGridMapTileSet::StaticClass(), UTileSet_ThumbnailRenderer::StaticClass());

	// Register asset types
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	GridMapAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("GridMapEditor")), LOCTEXT("GridMapAssetCategory", "Grid Map"));
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FGridMapTileSetAssetTypeActions(GridMapAssetCategory)));

	// Modes
	FEditorModeRegistry::Get().RegisterMode<FGridMapEditorMode>(FGridMapEditorMode::EM_GridMapEditorModeId, LOCTEXT("GridMapEditorModeName", "GridMapEditorMode"), FSlateIcon("GridMapStyle", "GridMapEditor.Tab", "GridMapEditor.Tab.Small"), true);
	FGridMapEditCommands::Register();
}

// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
// we call this function before unloading the module.
void FGridMapEditorModule::ShutdownModule()
{	
	FEditorModeRegistry::Get().UnregisterMode(FGridMapEditorMode::EM_GridMapEditorModeId);

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		// Unregister all structures
		for (auto It = RegisteredPropertyTypes.CreateConstIterator(); It; ++It)
		{
			if (It->IsValid())
			{
				PropertyModule.UnregisterCustomPropertyTypeLayout(*It);
			}
		}

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		StyleSet.Reset();
	}
}

void FGridMapEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}


void FGridMapEditorModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);

	RegisteredPropertyTypes.Add(PropertyTypeName);

	static FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGridMapEditorModule, GridMapEditor)