// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailPropertyExtensionHandler.h"
#include "IDetailCustomization.h"
#include "EditorUndoClient.h" 	
#include "PropertyHandle.h"

class UPaperZDAnimSequence;

/**
 * Detail customization for the Transition node of the state machine
 */
class FPaperZDAnimSequenceDetailCustomization : public IDetailCustomization, public FSelfRegisteringEditorUndoClient
{
	/* Weak pointer used to refresh the customization when needed. */
	TWeakPtr<IDetailLayoutBuilder> DetailBuilderPtr;

	/* Handle for the data source array. */
	TSharedPtr<IPropertyHandleArray> ArrayHandle;

	/* Array property for the data source. */
	FArrayProperty* DataSourceProperty;

	/* Selected node index. */
	int32 SelectedNodeIndex;

	/* Pointer to the AnimSequence we're customizing. */
	TWeakObjectPtr<UPaperZDAnimSequence> AnimSequencePtr;

	/* Amount of data source entries we're currently able to display. */
	int32 DataSourceEntryCount;

public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	//~Begin IDetailCustomization Interface
	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	//~End IDetailCustomization Interface

	//~Begin FEditorUndoClient Interface
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	//~End FEditorUndoClient Interface

private:
	/* Makes sure the given AnimSequence has a valid initial setup. */
	bool InitDataSourceHandleForAnimSequence(UPaperZDAnimSequence* AnimSequence);

	/* Forces the number of entries on the data source. */
	void SetDataSourceNum(uint32 Num);

	/* Checks if the entry of the data source is different than the default value. */
	bool IsDataSourceEntrySet(int32 Index) const;

	/* If the custom grid widget for directionality should be shown. */
	EVisibility GetDirectionalGridVisibility() const;

	/* If the custom grid widget for directionality should be shown. */
	EVisibility GetDetailVisibility(int32 Index) const;

	/* Returns the amount of nodes to display on the grid. */
	int32 GetNumNodes() const;
	uint32 GetNumNodesUnsigned() const;

	/* Returns the value for the "NumNodes" spinner. */
	TOptional<uint32> GetValueForNumNodesNumericEntry() const;

	/* Returns the angle offset on the directional sequence. */
	float GetAngleOffset() const;

	/* Called to select a specific AnimDataSource entry. */
	void SelectDataSourceEntry(int32 Index);

	/* Called to select a specific AnimDataSource entry. */
	void OnPreviewAreaChange(int32 Index);

	/* Called when the directional toggle changes. */
	void OnDirectionalPropertyChange();

	/* Called when the directional toggle changes. */
	void OnCommitedNumNodes(uint32 NewValue, ETextCommit::Type CommitType);

	/* Forces a refresh on the view, killing this customization. */
	void ForceRefresh();

	/* Checks if the customization still supports the data-source after an UNDO/REDO operation has happened. */
	bool CheckPostUndoRedoSupport() const;

	/* Detail text to put below the AnimationData name widget. */
	FText GetAnimationDataDetailText() const;

	/* Value text to use for the node to display. */
	FText GetAnimationDataNodeText(int32 Index) const;
};
