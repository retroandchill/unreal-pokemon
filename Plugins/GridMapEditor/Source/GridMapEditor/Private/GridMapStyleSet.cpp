#include "GridMapStyleSet.h"
#include "EditorStyleSet.h"

const FName FGridMapStyleSet::Name("GridMapStyle");

const FVector2D FGridMapStyleSet::Icon14x14(14.0f, 14.0f);
const FVector2D FGridMapStyleSet::Icon16x16(16.0f, 16.0f);
const FVector2D FGridMapStyleSet::Icon20x20(20.0f, 20.0f);
const FVector2D FGridMapStyleSet::Icon40x40(40.0f, 40.0f);
const FVector2D FGridMapStyleSet::Icon64x64(64.0f, 64.0f);
const FVector2D FGridMapStyleSet::Icon512x512(512.0f, 512.0f);

const FMargin FGridMapStyleSet::StandardPadding(6.f, 3.f);
const FMargin FGridMapStyleSet::StandardLeftPadding(6.f, 3.f, 3.f, 3.f);
const FMargin FGridMapStyleSet::StandardRightPadding(3.f, 3.f, 6.f, 3.f);

const FSlateFontInfo FGridMapStyleSet::StandardFont = FEditorStyle::GetFontStyle(TEXT("PropertyWindow.NormalFont"));

FGridMapStyleSet::FGridMapStyleSet(const FString& PluginContentDir)
	: Super(Name)
	, ContentGridMapDir(PluginContentDir)
{
	SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	Set("GridMapEditor.Tab", new FSlateImageBrush(ContentGridMapDir + TEXT("Icons/UIIcons/mode_40.png"), Icon40x40));
	Set("GridMapEditor.Tab.Small", new FSlateImageBrush(ContentGridMapDir + TEXT("Icons/UIIcons/mode_40.png"), Icon16x16));

	Set("GridMapEditCommands.PaintTiles", new FSlateImageBrush(ContentGridMapDir + TEXT("Icons/UIIcons/paint_40.png"), Icon20x20));
	Set("GridMapEditCommands.SelectTiles", new FSlateImageBrush(ContentGridMapDir + TEXT("Icons/UIIcons/icon_GridMap_Select_40x.png"), Icon20x20));
	Set("GridMapEditCommands.TileSettings", new FSlateImageBrush(ContentGridMapDir + TEXT("Icons/UIIcons/icon_GridMap_Settings_40x.png"), Icon20x20));
}
