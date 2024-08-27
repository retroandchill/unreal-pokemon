#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Styling/SlateTypes.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

struct FGridMapEditorUISettings;
class FGridMapEditorMode;

class SGridMapEditorSettingsWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGridMapEditorSettingsWidget) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, FGridMapEditorUISettings* GridMapUISettings, FGridMapEditorMode* GridMapEditorMode);

private:
	EVisibility GetVisibility_SettingsTab() const;

	void OnCheckStateChanged_DrawUpdatedTiles(ECheckBoxState InState);
	ECheckBoxState GetCheckState_DrawUpdatedTiles() const;

	FReply OnRebuildAllTiles();

private:
	FGridMapEditorMode* EditorMode;
	FGridMapEditorUISettings* UISettings;
};
