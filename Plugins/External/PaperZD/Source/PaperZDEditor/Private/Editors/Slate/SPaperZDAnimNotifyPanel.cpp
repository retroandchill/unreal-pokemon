// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDAnimNotifyPanel.h"
#include "Editors/AnimTimeline/PaperZDAnimModel.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack_NotifiesPanel.h"
#include "Editors/Slate/SPaperZDAnimationSourceNotifies.h"
#include "Editors/PaperZDAnimationSourceEditor.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Styling/AppStyle.h"
#include "UnrealExporter.h"
#include "Exporters/Exporter.h"
#include "Factories.h"


#include "PaperZDEditor.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "SScrubWidget.h"
#include "SScrubControlPanel.h"
#include "SPaperZDConfirmDialog.h"
#include "ScopedTransaction.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Fonts/FontMeasure.h"
#include "Editor.h"

#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"


#include "Framework/Application/SlateApplication.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Commands/UICommandList.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/STextEntryPopup.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include "Animation/AnimTypes.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Notifies/PaperZDAnimNotify.h"
#include "Notifies/PaperZDAnimNotifyCustom.h"
#include "Notifies/PaperZDAnimNotifyState.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "PaperZDAnimNotifyPanel"

// Track Panel drawing
const float NotificationTrackHeight = 20.0f;

// AnimNotify Drawing
const float NotifyHeightOffset = 0.f;
const float NotifyHeight = FPaperZDAnimTimelineTrack_NotifiesPanel::NotificationTrackHeight;
const FVector2D ScrubHandleSize(12.0f, 12.0f);
const FVector2D AlignmentMarkerSize(10.f, 20.f);
const FVector2D TextBorderSize(1.f, 1.f);
const float ScrubHandleHitBuffer = 3.0f;


class SPaperZDAnimNotifyNode;

DECLARE_DELEGATE_RetVal_FourParams(FReply, FOnNotifyNodeDragStarted, TSharedRef<SPaperZDAnimNotifyNode>, const FPointerEvent&, const FVector2D&, const bool)
DECLARE_DELEGATE_RetVal_FiveParams(FReply, FOnNotifyNodesDragStarted, TArray<TSharedPtr<SPaperZDAnimNotifyNode>>, TSharedRef<SWidget>, const FVector2D&, const FVector2D&, const bool)
DECLARE_DELEGATE_RetVal(float, FOnGetDraggedNodePos)

DECLARE_DELEGATE(FOnUpdatePanel)
DECLARE_DELEGATE(FOnDeselectAllNotifies)
DECLARE_DELEGATE_RetVal(TArray<UPaperZDAnimNotify_Base*>, FOnGetAllSelectedNotifyObjects)
DECLARE_DELEGATE(FOnDeleteSelectedNotifies)
DECLARE_DELEGATE_TwoParams(FPanTrackRequest, int32, FVector2D)
DECLARE_DELEGATE_FourParams(FPasteNodes, SPaperZDAnimNotifyTrack*, float, EPaperZDNotifyPasteMode, EPaperZDNotifyPasteMultipleMode)

enum EHitPosition {
	EHP_Hit = 0,
	EHP_Above = 1,
	EHP_Below = 2,
	EHP_Right = 4,
	EHP_Left = 8
};

namespace ENotifyStateHandleHit
{
	enum Type
	{
		Start,
		End,
		None
	};
}

/* Text factory used for pasting notify objects into the track. */
class FTextAnimNotifyFactory : public FCustomizableTextObjectFactory
{
	/* Array that will contain the notifies. */
	TArray<UPaperZDAnimNotify_Base*> AnimNotifies;

	/* AnimSequence that will serve as recipient. */
	UPaperZDAnimSequence* AnimSequence;

	/* Track offset to apply. */
	int32 TrackOffset;

	/* Time offset to apply. */
	float TimeOffset;

	/* Time scale to apply. */
	float TimeScale;

public:
	FTextAnimNotifyFactory(UPaperZDAnimSequence* InAnimSequence, int32 InTrackOffset = 0, float InTimeOffset = 0.0f, float InTimeScale = 1.0f)
		: FCustomizableTextObjectFactory(GWarn)
		, AnimSequence(InAnimSequence)
		, TrackOffset(InTrackOffset)
		, TimeOffset(InTimeOffset)
		, TimeScale(InTimeScale)
	{
		check(AnimSequence);
	}

	//Getter for the resulting array
	const TArray<UPaperZDAnimNotify_Base*>& GetAnimNotifies() const { return AnimNotifies; }

	/* True for all notifies. */
	virtual bool CanCreateClass(UClass* ObjectClass, bool& bOmitSubObjs) const override
	{
		return ObjectClass->IsChildOf(UPaperZDAnimNotify_Base::StaticClass());
	}

	virtual void ProcessConstructedObject(UObject* CreatedObject) override
	{
		//Add the notify onto the array if compatible (only non compatibles are custom notifies that have an invalid source notify name)
		UPaperZDAnimNotify_Base* Notify = Cast<UPaperZDAnimNotify_Base>(CreatedObject);
		bool bValid = true;
		if (UPaperZDAnimNotifyCustom* SourceNotify = Cast<UPaperZDAnimNotifyCustom>(Notify))
		{
			bValid = AnimSequence->GetAnimSource()->GetRegisteredNotifyNames().Contains(SourceNotify->Name);
		}

		//Only add if valid
		if (bValid)
		{
			Notify->TrackIndex += TrackOffset;
			Notify->Time *= TimeScale;
			Notify->Time += TimeOffset;

			//Change duration for notify states
			if (UPaperZDAnimNotifyState* StateNotify = Cast<UPaperZDAnimNotifyState>(Notify))
			{
				StateNotify->Duration *= TimeScale;
				StateNotify->Duration += TimeOffset;
			}

			AnimNotifies.Add(Notify);
		}
	}
};

// Read common info from the clipboard
bool ReadNotifyPasteHeader(FString& OutPropertyString, const TCHAR*& OutBuffer, float& OutOriginalTime, float& OutOriginalLength, int32& OutFirstTrackIndex, int32& OutTrackSpan, int32& OutNum)
{
	OutBuffer = NULL;
	OutOriginalTime = -1.f;

	FPlatformApplicationMisc::ClipboardPaste(OutPropertyString);

	if (!OutPropertyString.IsEmpty())
	{
		//Remove header text
		const FString HeaderString(TEXT("ZD_COPY_ANIMNOTIFY"));

		//Check for string identifier in order to determine whether the text represents an FAnimNotifyEvent.
		if (OutPropertyString.StartsWith(HeaderString) && OutPropertyString.Len() > HeaderString.Len())
		{
			int32 HeaderSize = HeaderString.Len();
			OutBuffer = *OutPropertyString;
			OutBuffer += HeaderSize;

			FString ReadLine;
			// Read the original time from the first notify
			FParse::Line(&OutBuffer, ReadLine);
			FParse::Value(*ReadLine, TEXT("OriginalTime="), OutOriginalTime);
			FParse::Value(*ReadLine, TEXT("OriginalLength="), OutOriginalLength);
			FParse::Value(*ReadLine, TEXT("FirstTrackIndex="), OutFirstTrackIndex);
			FParse::Value(*ReadLine, TEXT("TrackSpan="), OutTrackSpan);
			FParse::Value(*ReadLine, TEXT("Num="), OutNum);
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// SPaperZDAnimNotifyNode Declaration
class SPaperZDAnimNotifyNode : public SLeafWidget
{
	SLATE_BEGIN_ARGS(SPaperZDAnimNotifyNode) 
		: _Notify(nullptr)
		, _AnimSequence(nullptr)
		, _OnNodeDragStarted()
		, _OnUpdatePanel()
		, _PanTrackRequest()
		, _OnSelectionChanged()
		, _ViewInputMin()
		, _ViewInputMax()
		, _OnSnapPosition()
	{}
	SLATE_ARGUMENT(UPaperZDAnimNotify_Base*, Notify)
	SLATE_ARGUMENT(UPaperZDAnimSequence*, AnimSequence)
	SLATE_EVENT(FOnNotifyNodeDragStarted, OnNodeDragStarted)
	SLATE_EVENT(FOnUpdatePanel, OnUpdatePanel)
	SLATE_EVENT(FPanTrackRequest, PanTrackRequest)
	SLATE_EVENT(FOnTrackSelectionChanged, OnSelectionChanged)
	SLATE_ATTRIBUTE(float, ViewInputMin)
	SLATE_ATTRIBUTE(float, ViewInputMax)
	SLATE_EVENT(FOnSnapPosition, OnSnapPosition)
	SLATE_ARGUMENT(int, TrackIndex)

	SLATE_END_ARGS()

	/* Constructs this widget. */
	void Construct(const FArguments& InArgs);

	// SWidget interface
 	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
 	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
 	virtual bool SupportsKeyboardFocus() const override;
	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
 	virtual FVector2D ComputeDesiredSize(float) const override;
 	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// End of SWidget interface

 	/* Extra hit test to decide whether or not the duration handles were hit on a state node. */
 	ENotifyStateHandleHit::Type DurationHandleHitTest(const FVector2D& CursorScreenPosition) const;

	/* Cancels any drag operation. */
	void DropCancelled();

	/* Detects any overflow on the anim notify track and requests a track pan */
	float HandleOverflowPan(const FVector2D& ScreenCursorPos, float TrackScreenSpaceXPosition, float TrackScreenSpaceMin, float TrackScreenSpaceMax);

	/* Finds a snap position if possible for the provided scrub handle, if it is not possible, returns -1.0f */
	float GetScrubHandleSnapPosition(float NotifyInputX, ENotifyStateHandleHit::Type HandleToCheck);

	/* Checks if the given mouse position hit this node. */
	bool HitTest(const FGeometry& AllottedGeometry, FVector2D MouseLocalPose) const;

	/* True if all the data of the node is valid, otherwise it contains stale data that shouldn't be accessed directly. */
	bool IsValid() const { return Notify.IsValid(); }

	/* Obtain the notify object that lives inside this node. */
	UPaperZDAnimNotify_Base* GetNotifyObject() const { return Notify.Get(); }

	/** Sets the position the mouse was at when this node was last hit */
	void SetLastMouseDownPosition(const FVector2D& CursorPosition) { MouseDownLastPosition = CursorPosition;}

	/* Draws the handles that are used to move the notify itself or its duration. */
 	void DrawScrubHandle(float ScrubHandleCentre, FSlateWindowElementList& OutDrawElements, int32 ScrubHandleID, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FLinearColor NodeColour) const;

	/* Updates the widget visuals. */
	void UpdateSizeAndPosition(const FGeometry& AllottedGeometry);

	/* Returns the size of this widget. */
 	const FVector2D& GetSize() const { return Size; }

	/** Returns the size of this notify duration in screen space */
	float GetDurationSize() const { return NotifyDurationSizeX; }

	/** Returns the start time associated with the notify on this node.  */
	float GetTime() const { return Notify.IsValid() ? Notify->Time : 0.0f; }

	/* Sets the Time in which the notify will trigger. */
	void SetTime(float InTime);

	/** Returns the duration of the state event if available, or 0.  */
	float GetDuration() const { return Cast<UPaperZDAnimNotifyState>(Notify) ? Cast<UPaperZDAnimNotifyState>(Notify)->Duration : 0.0f; }

	/* Sets the new duration, if this is a state notify. */
	void SetDuration(float InDuration);

	/* The name of the associated notify object. */
	FName GetNotifyName() const { return Notify.IsValid() ? Notify->GetDisplayName() : TEXT(""); }

	/* Tooltip to display on the node when hovering it. */
	FText GetNodeTooltip() const;

	/* Obtain the position of this node in the parent widget. */
 	const FVector2D& GetPosition() const { return Position; }

	/* Returns the track currently associated with this node. */
	int32 GetTrackIndex() const { return TrackIndex; }

	/* Return the screen position of this node. */
	const FVector2D& GetScreenPosition() const { return ScreenPosition; }
	
	/* Left padding of the widget. */
	float GetWidgetPaddingLeft() const	{ return GetPosition().X; }
 
	/* Whether this node is currently being used in a Drag&Drop operation. */
 	bool IsBeingDragged() const { return bBeingDragged; }

	/* Getter for the last snapped time this node used. */
	const float GetLastSnappedTime() const	{ return LastSnappedTime; }

	/* Position of the notify in the widget. */
	FVector2D GetNotifyPosition() const { return FVector2D(NotifyTimePositionX, NotifyHeightOffset); }

	/* Offset from the widget start to the notify position. */
	FVector2D GetNotifyPositionOffset() const { return GetNotifyPosition() - GetPosition(); }

	/* Removes the cached value used for snap time. */
	void ClearLastSnappedTime()
	{
		LastSnappedTime = -1.0f;
	}

	void SetLastSnappedTime(float NewSnapTime)
	{
		LastSnappedTime = NewSnapTime;
	}

public:
	/* The notify that we're representating */
	TWeakObjectPtr<UPaperZDAnimNotify_Base> Notify;
	bool bSelected;

	/* Cached geometry of the owner track, set up by the track itself. */
	FGeometry CachedTrackGeometry;

private:
	/* Sequence that the node belongs to. */
	UPaperZDAnimSequence* AnimSequence;

	FVector2D CachedAllotedGeometrySize;
	FVector2D Position;
	FVector2D Size;
	float NotifyTimePositionX;
	float NotifyDurationSizeX;
	float NotifyScrubHandleCentre;
	FVector2D ScreenPosition;
	FVector2D MouseDownLastPosition;
	int32 TrackIndex;
	float LastSnappedTime;

	//Handle used during duration modification
	ENotifyStateHandleHit::Type CurrentDragHandle;
	int32 DragMarkerTransactionIdx;

	TAttribute<float> ViewInputMin;
	TAttribute<float> ViewInputMax;

	bool bBeingDragged;
	bool bDrawTooltipToRight;

	TSharedPtr<SOverlay> EndMarkerNodeOverlay;

	FSlateFontInfo Font;
	FVector2D TextSize;
	float LabelWidth;

	//Slate Events
	FOnNotifyNodeDragStarted		OnNodeDragStarted;
	FOnDeselectAllNotifies			OnDeselectAllNotifies;
	FOnTrackSelectionChanged		OnSelectionChanged;
	FOnUpdatePanel					OnUpdatePanel;
	FPanTrackRequest				PanTrackRequest;
	FOnSnapPosition					OnSnapPosition;
};

//////////////////////////////////////////////////////////////////////////
// SPaperZDAnimNotifyTrack Declaration

class SPaperZDAnimNotifyTrack : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPaperZDAnimNotifyTrack)
	:	_AnimSequence(nullptr)
	{}
	SLATE_ARGUMENT(UPaperZDAnimSequence *, AnimSequence)
	SLATE_ARGUMENT(int, TrackIndex)
	SLATE_ATTRIBUTE(float, ViewInputMin)
	SLATE_ATTRIBUTE(float, ViewInputMax)
	SLATE_EVENT(FOnNotifyNodesDragStarted, OnNodesDragStarted)
	SLATE_EVENT(FPanTrackRequest, OnRequestTrackPan)
	SLATE_EVENT(FOnGetDraggedNodePos, OnGetDraggedNodePos)
	SLATE_EVENT(FOnUpdatePanel, OnUpdatePanel)
	SLATE_EVENT(FOnDeselectAllNotifies, OnDeselectAllNotifies)
	SLATE_EVENT(FOnGetAllSelectedNotifyObjects, OnGetAllSelectedNotifyObjects)
	SLATE_EVENT(FOnDeleteSelectedNotifies, OnDeleteSelectedNotifies)
	SLATE_EVENT(FOnTrackSelectionChanged, OnSelectionChanged)
	SLATE_EVENT(FOnSnapPosition, OnSnapPosition)
	SLATE_EVENT(FPasteNodes, OnPasteNodes)
	SLATE_EVENT(FPaperZDEditorDelegates::OnInvokeTabSignature, OnInvokeTab)
	SLATE_ARGUMENT(TSharedPtr<FUICommandList>, CommandList)
	SLATE_END_ARGS()

	/** Type used for list widget of tracks */
	void Construct(const FArguments& InArgs);

	// SWidget interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
 	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
	virtual bool SupportsKeyboardFocus() const override
	{
		return true;
	}
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	// End of SWidget interface

	/* Update the nodes to match the data that the panel is observing. */
	void Update();

	/** Returns the cached rendering geometry of this track */
	const FGeometry& GetCachedGeometry() const { return CachedGeometry; }
	FTrackScaleInfo GetCachedScaleInfo() const { return FTrackScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, CachedGeometry.GetLocalSize()); }

	/* Called when a notify node starts being dragged*/
	FReply OnNotifyNodeDragStarted(TSharedRef<SPaperZDAnimNotifyNode> NotifyNode, const FPointerEvent& MouseEvent, const FVector2D& ScreenNodePosition, const bool bDragOnMarker, int32 NotifyIndex);

	/* Obtain the index of the notify that was hit by the cursor, if any. */
	int32 GetHitNotifyNode(const FGeometry& MyGeometry, const FVector2D& CursorPosition);

	// Adds our current selection to the provided array
	void AppendSelectionToArray(TArray<UPaperZDAnimNotify_Base*>& Selection) const;

	// Gets the currently selected SAnimNotifyNode instances
	void AppendSelectedNodeWidgetsToArray(TArray<TSharedPtr<SPaperZDAnimNotifyNode>>& NodeArray) const;

	// Uses the given track space rect and marquee information to refresh selection information
	void RefreshMarqueeSelectedNodes(FSlateRect& Rect, FPaperZDNotifyMarqueeOperation& Marquee);

	/* Get the last clicked time on the track. */
	float GetLastClickedTime() const { return LastClickedTime; }

	void OnPasteNotifyClicked(EPaperZDNotifyPasteMode PasteMode, EPaperZDNotifyPasteMultipleMode MultiplePasteType);
	
 	/* Disconnect the nodes and add them to the given drag node array. */
 	void DisconnectSelectedNodesForDrag(TArray<TSharedPtr<SPaperZDAnimNotifyNode>>& DragNodes);

	/**
	* Deselects all currently selected notify nodes
	* @param bUpdateSelectionSet - Whether we should report a selection change to the panel
	*/
	void DeselectAllNotifyNodes(bool bUpdateSelectionSet = true);

	//Handle the drop of a node on this track, at the given offset position
	void HandleNodeDrop(TSharedPtr<SPaperZDAnimNotifyNode> Node, float Offset);

	/* Create the context menu displayed when right clicking. */
	TSharedPtr<SWidget> GetContextMenu(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	// Returns the index associated with this track object
	int32 GetTrackIndex() const
	{ 
		return TrackIndex;
	}

	// Returns the notify node array
	const TArray<TSharedPtr<SPaperZDAnimNotifyNode>>& GetNotifyNodes() const { return NotifyNodes; }

	/* Select all the nodes inside this track that have a matching UID. */
	void SelectNodesByUniqueIDs(const TSet<uint32>& InUniqueIds, bool bUpdateSelectionSet);

 	// Returns the padding needed to render the notify in the correct track position
 	FMargin GetNotifyTrackPadding(int32 NotifyIndex) const
	{
		float LeftMargin = NotifyNodes[NotifyIndex]->GetWidgetPaddingLeft();
		float RightMargin = CachedGeometry.Size.X - NotifyNodes[NotifyIndex]->GetPosition().X - NotifyNodes[NotifyIndex]->GetSize().X;
		return FMargin(LeftMargin, 0, RightMargin, 0);
	}

	// Builds a UObject selection set and calls the OnSelectionChanged delegate
	void SendSelectionChanged()
	{
		OnSelectionChanged.ExecuteIfBound();
	}

	FVector2D ComputeDesiredSize(float) const override
	{
		FVector2D Size;
		Size.X = 200.0f;
		Size.Y = FPaperZDAnimTimelineTrack_NotifiesPanel::NotificationTrackHeight;

		return Size;
	}

 	/* Translate the given position on the track to time. */
 	float CalculateTime(const FGeometry& MyGeometry, FVector2D Position, bool bInputIsAbsolute = true);

	/* Obtain all the selected notify objects. */
	TArray<UPaperZDAnimNotify_Base*> GetAllSelectedNotifyObjects()
	{
		TArray<UPaperZDAnimNotify_Base*> SelectedNotifies;
		for (TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
		{
			if (Node->bSelected)
			{
				SelectedNotifies.Add(Node->Notify.Get());
			}
		}

		return SelectedNotifies;
	}

	/* Obtain all the selected notify objects while casting to the given class. */
	template <class T>
	TArray<T*> GetAllSelectedNotifyObjects()
	{
		TArray<T*> SelectedNotifies;
		for (TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
		{
			if (Node->bSelected)
			{
				SelectedNotifies.Add(Cast<T>(Node->Notify));
			}
		}

		return SelectedNotifies;
	}

private:
	// Store the tracks geometry for later use
	void UpdateCachedGeometry(const FGeometry& InGeometry);

	/**
	 * Selects a node on the track. Supports multi selection
	 * @param TrackNodeIndex - Index of the node to select.
	 * @param Append - Whether to append to to current selection or start a new one.
	 * @param bUpdateSelection - Whether to immediately inform Persona of a selection change
	 */
	void SelectTrackObjectNode(int32 TrackNodeIndex, bool Append, bool bUpdateSelection = true);

	/**
	 * Toggles the selection status of a notify node, for example when
	 * Control is held when clicking.
	 * @param NotifyIndex - Index of the notify to toggle the selection status of
	 * @param bUpdateSelection - Whether to immediately inform Persona of a selection change
	 */
	void ToggleTrackObjectNodeSelectionStatus(int32 TrackNodeIndex, bool bUpdateSelection = true);

	/**
	 * Deselects requested notify node
	 * @param NotifyIndex - Index of the notify node to deselect
	 * @param bUpdateSelection - Whether to immediately inform Persona of a selection change
	 */
	void DeselectTrackObjectNode(int32 TrackNodeIndex, bool bUpdateSelection = true);

	// Data structure for blueprint notify context menu entries
	struct BlueprintNotifyMenuInfo
	{
		FName NotifyName;
		FString BlueprintPath;
		UClass* BaseClass;
	};

 	//Called from the context menu to create the sub-menues for building notifies
	template<typename NotifyTypeClass>
	void MakeNewNotifyPicker(FMenuBuilder& MenuBuilder, bool bIsReplaceWithMenu = false);
 	void FillNewNotifyMenu(FMenuBuilder& MenuBuilderbool, bool bIsReplaceWithMenu = false);
 	void FillNewNotifyStateMenu(FMenuBuilder& MenuBuilder, bool bIsReplaceWithMenu = false);

	//New notify related
	void OnNewNotifyClicked();
	void AddNewNotify(const FText& NewNotifyName, ETextCommit::Type CommitInfo);

	// Whether we have one node selected
	bool IsSingleNodeSelected();

	/* Creates a new notify at the last clicked location. */
	void CreateNewNotifyAtCursor(FName NewNotifyName, UClass* NotifyClass);

	/* Get the default Notify Name for a given blueprint notify asset. */
	FName MakeBlueprintNotifyName(const FString& InNotifyClassName);

	/* Provides direct access to the notify menu from the context menu */
	void OnManageNotifies();

	/** Opens the supplied blueprint in an editor */
	void OnOpenNotifySource(UBlueprint* InSourceBlueprint) const;

	/** Filters the asset browser by the selected notify */
	void OnFilterSourceNotify(FName InName);

private:
	/* Command list used by the panel. */
	TWeakPtr<FUICommandList> WeakCommandList;

	/* AnimSequence we're currently editing */
	UPaperZDAnimSequence* AnimSequence;

	/* Notify node objects that live inside this track. */
	TArray<TSharedPtr<SPaperZDAnimNotifyNode>> NotifyNodes;

	/* Contains the indices of the node that are currently selected. */
	TArray<int32> SelectedNodeIndices;

	/* Track this node belongs to. */
	int32 TrackIndex;

	/* Cached clicked time. */
	float LastClickedTime;

	//Slate elements where the update draws the nodes
	TSharedPtr<SBorder> TrackArea;
	TSharedPtr<SOverlay> NodeOverlay;

	//Input range, can be changed by panning or externally
	TAttribute<float> ViewInputMin;
	TAttribute<float> ViewInputMax;

	//Slate events
	FOnNotifyNodesDragStarted							OnNodesDragStarted;
	FOnUpdatePanel										OnUpdatePanel;
	FOnDeselectAllNotifies								OnDeselectAllNotifies;
	FOnGetAllSelectedNotifyObjects						OnGetAllSelectedNotifyObjects;
	FOnDeleteSelectedNotifies							OnDeleteSelectedNotifies;
	FOnTrackSelectionChanged							OnSelectionChanged;
	FPanTrackRequest									OnRequestTrackPan;
	FOnGetDraggedNodePos								OnGetDraggedNodePos;
	FOnSnapPosition										OnSnapPosition;
	FPasteNodes											OnPasteNodes;
	FPaperZDEditorDelegates::OnInvokeTabSignature		OnInvokeTab;

	/* The geometry, cached for later use. */
	FGeometry CachedGeometry;
};

//////////////////////////////////////////////////////////////////////////
// FNotifyDragDrop
class FNotifyDragDropOp : public FDragDropOperation
{
public:
	FNotifyDragDropOp(float& InCurrentDragXPosition) : 
		CurrentDragXPosition(InCurrentDragXPosition), 
		SnapTime(-1.f),
		SelectionTimeLength(0.0f)
	{
	}

	enum class EAnimEventTriggerOffsets : uint8
	{
		OffsetBefore,
		OffsetAfter,
		NoOffset
	};

	struct FTrackClampInfo
	{
		int32 TrackPos;
		int32 TrackSnapTestPos;
		TSharedPtr<SPaperZDAnimNotifyTrack> NotifyTrack;
	};

	DRAG_DROP_OPERATOR_TYPE(FNotifyDragDropOp, FDragDropOperation)

	virtual void OnDrop( bool bDropWasHandled, const FPointerEvent& MouseEvent ) override
	{
		if ( bDropWasHandled == false )
		{
			int32 NumNodes = SelectedNodes.Num();

			const FScopedTransaction Transaction(NumNodes > 0 ? LOCTEXT("MoveNotifiesEvent", "Move Anim Notifies") : LOCTEXT("MoveNotifyEvent", "Move Anim Notify"));
			Sequence->Modify();
			
			for(int32 CurrentNode = 0 ; CurrentNode < NumNodes ; ++CurrentNode)
			{
				TSharedPtr<SPaperZDAnimNotifyNode> Node = SelectedNodes[CurrentNode];
				float NodePositionOffset = NodeXOffsets[CurrentNode];
				const FTrackClampInfo& ClampInfo = GetTrackClampInfo(Node->GetScreenPosition());
				ClampInfo.NotifyTrack->HandleNodeDrop(Node, NodePositionOffset);
				Node->DropCancelled();
			}

			Sequence->PostEditChange();
			Sequence->MarkPackageDirty();
			
			OnUpdatePanel.ExecuteIfBound();
		}
		
		FDragDropOperation::OnDrop(bDropWasHandled, MouseEvent);
	}

	virtual void OnDragged( const class FDragDropEvent& DragDropEvent ) override
	{
		// Reset snapped node pointer
		SnappedNode = NULL;

		NodeGroupPosition = DragDropEvent.GetScreenSpacePosition() + DragOffset;

		FTrackClampInfo* SelectionPositionClampInfo = &GetTrackClampInfo(DragDropEvent.GetScreenSpacePosition());
		if((SelectionPositionClampInfo->NotifyTrack->GetTrackIndex() + TrackSpan) >= ClampInfos.Num())
		{
			// Our selection has moved off the bottom of the notify panel, adjust the clamping information to keep it on the panel
			SelectionPositionClampInfo = &ClampInfos[ClampInfos.Num() - TrackSpan - 1];
		}
		
		const FGeometry& TrackGeom = SelectionPositionClampInfo->NotifyTrack->GetCachedGeometry();
		const FTrackScaleInfo& TrackScaleInfo = SelectionPositionClampInfo->NotifyTrack->GetCachedScaleInfo();

		FVector2D SelectionBeginPosition = TrackGeom.LocalToAbsolute(TrackGeom.AbsoluteToLocal(NodeGroupPosition) + SelectedNodes[0]->GetNotifyPositionOffset());
	
		float LocalTrackMin = TrackScaleInfo.InputToLocalX(0.0f);
		float LocalTrackMax = TrackScaleInfo.InputToLocalX(Sequence->GetTotalDuration());
		float LocalTrackWidth = LocalTrackMax - LocalTrackMin;

		// Tracks the movement amount to apply to the selection due to a snap.
		float SnapMovement = 0.0f;
		// Clamp the selection into the track
		float SelectionBeginLocalPositionX = TrackGeom.AbsoluteToLocal(SelectionBeginPosition).X;
		const float ClampedEnd = FMath::Clamp(SelectionBeginLocalPositionX + NodeGroupSize.X, LocalTrackMin, LocalTrackMax);
		const float ClampedBegin = FMath::Clamp(SelectionBeginLocalPositionX, LocalTrackMin, LocalTrackMax);
		if(ClampedBegin > SelectionBeginLocalPositionX)
		{
			SelectionBeginLocalPositionX = ClampedBegin;
		}
		else if(ClampedEnd < SelectionBeginLocalPositionX + NodeGroupSize.X)
		{
			SelectionBeginLocalPositionX = ClampedEnd - NodeGroupSize.X;
		}

		SelectionBeginPosition.X = TrackGeom.LocalToAbsolute(FVector2D(SelectionBeginLocalPositionX, 0.0f)).X;

		// Handle node snaps
		bool bSnapped = false;
		for (int32 NodeIdx = 0; NodeIdx < SelectedNodes.Num() && !bSnapped; ++NodeIdx)
		{
			TSharedPtr<SPaperZDAnimNotifyNode> CurrentNode = SelectedNodes[NodeIdx];

 			// Clear off any snap time currently stored
 			CurrentNode->ClearLastSnappedTime();

			const FTrackClampInfo& NodeClamp = GetTrackClampInfo(CurrentNode->GetScreenPosition());

			FVector2D EventPosition = SelectionBeginPosition + FVector2D(TrackScaleInfo.PixelsPerInput * NodeTimeOffsets[NodeIdx], 0.0f);

			// Look for a snap on the first scrub handle
			FVector2D TrackNodePos = TrackGeom.AbsoluteToLocal(EventPosition);
			const FVector2D OriginalNodePosition = TrackNodePos;
			float SequenceEnd = TrackScaleInfo.InputToLocalX(Sequence->GetTotalDuration());

			// Always clamp the Y to the current track
			SelectionBeginPosition.Y = SelectionPositionClampInfo->TrackPos - 1.0f;

			float SnapX = GetSnapPosition(NodeClamp, TrackNodePos.X, bSnapped);
			if (bSnapped)
			{
				CurrentNode->SetLastSnappedTime(TrackScaleInfo.LocalXToInput(SnapX));

				if (SnapMovement == 0.0f)
				{
					SnapMovement = SnapX - TrackNodePos.X;
					TrackNodePos.X = SnapX;
					SnapTime = TrackScaleInfo.LocalXToInput(SnapX);
					SnappedNode = CurrentNode;
				}
				EventPosition = NodeClamp.NotifyTrack->GetCachedGeometry().LocalToAbsolute(TrackNodePos);
			}

			// If we didn't snap the beginning of the node, attempt to snap the end
			if (!bSnapped)
			{
				FVector2D TrackNodeEndPos = TrackNodePos + CurrentNode->GetDurationSize();
				SnapX = GetSnapPosition(*SelectionPositionClampInfo, TrackNodeEndPos.X, bSnapped);

				// Only attempt to snap if the node will fit on the track
				if (SnapX >= CurrentNode->GetDurationSize())
				{
					if (SnapMovement == 0.0f)
					{
						SnapMovement = SnapX - TrackNodeEndPos.X;
						SnapTime = TrackScaleInfo.LocalXToInput(SnapX) - CurrentNode->GetDuration();
						CurrentNode->SetLastSnappedTime(SnapTime);
						SnappedNode = CurrentNode;
					}
				}
			}
		}

		SelectionBeginPosition.X += SnapMovement;

		CurrentDragXPosition = TrackGeom.AbsoluteToLocal(FVector2D(SelectionBeginPosition.X,0.0f)).X;

		CursorDecoratorWindow->MoveWindowTo(TrackGeom.LocalToAbsolute(TrackGeom.AbsoluteToLocal(SelectionBeginPosition) - SelectedNodes[0]->GetNotifyPositionOffset()));
		NodeGroupPosition = SelectionBeginPosition;

		//scroll view
		float LocalMouseXPos = TrackGeom.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition()).X;
		float LocalViewportMin = 0.0f;
		float LocalViewportMax = TrackGeom.GetLocalSize().X;
		if(LocalMouseXPos < LocalViewportMin && LocalViewportMin > LocalTrackMin - 10.0f)
		{
			float ScreenDelta = FMath::Max(LocalMouseXPos - LocalViewportMin, -10.0f);
			RequestTrackPan.Execute(ScreenDelta, FVector2D(LocalTrackWidth, 1.f));
		}
		else if(LocalMouseXPos > LocalViewportMax && LocalViewportMax < LocalTrackMax + 10.0f)
		{
			float ScreenDelta =  FMath::Max(LocalMouseXPos - LocalViewportMax, 10.0f);
			RequestTrackPan.Execute(ScreenDelta, FVector2D(LocalTrackWidth, 1.f));
		}
	}

	float GetSnapPosition(const FTrackClampInfo& ClampInfo, float WidgetSpaceNotifyPosition, bool& bOutSnapped)
	{
		const FTrackScaleInfo& ScaleInfo = ClampInfo.NotifyTrack->GetCachedScaleInfo();

		const float MaxSnapDist = 5.f;

		float CurrentMinSnapDest = MaxSnapDist;
		float SnapPosition = ScaleInfo.LocalXToInput(WidgetSpaceNotifyPosition);
		bOutSnapped = OnSnapPosition.IsBound() && !FSlateApplication::Get().GetModifierKeys().IsControlDown() && OnSnapPosition.Execute(SnapPosition, MaxSnapDist / ScaleInfo.PixelsPerInput, TArrayView<const FName>());
		SnapPosition = ScaleInfo.InputToLocalX(SnapPosition);

		float WidgetSpaceStartPosition = ScaleInfo.InputToLocalX(0.0f);
		float WidgetSpaceEndPosition = ScaleInfo.InputToLocalX(Sequence->GetTotalDuration());

		if(!bOutSnapped)
		{
			// Didn't snap to a bar, snap to the track bounds
			float SnapDistBegin = FMath::Abs(WidgetSpaceStartPosition - WidgetSpaceNotifyPosition);
			float SnapDistEnd = FMath::Abs(WidgetSpaceEndPosition - WidgetSpaceNotifyPosition);
			if(SnapDistBegin < CurrentMinSnapDest)
			{
				SnapPosition = WidgetSpaceStartPosition;
				bOutSnapped = true;
			}
			else if(SnapDistEnd < CurrentMinSnapDest)
			{
				SnapPosition = WidgetSpaceEndPosition;
				bOutSnapped = true;
			}
		}

		return SnapPosition;
	}

	FTrackClampInfo& GetTrackClampInfo(const FVector2D NodePos)
	{
		int32 ClampInfoIndex = 0;
		int32 SmallestNodeTrackDist = FMath::Abs(ClampInfos[0].TrackSnapTestPos - NodePos.Y);
		for(int32 i = 0; i < ClampInfos.Num(); ++i)
		{
			int32 Dist = FMath::Abs(ClampInfos[i].TrackSnapTestPos - NodePos.Y);
			if(Dist < SmallestNodeTrackDist)
			{
				SmallestNodeTrackDist = Dist;
				ClampInfoIndex = i;
			}
		}
		return ClampInfos[ClampInfoIndex];
	}

	class UPaperZDAnimSequence*					Sequence;				// The owning anim sequence
	FVector2D									DragOffset;				// Offset from the mouse to place the decorator
	TArray<FTrackClampInfo>						ClampInfos;				// Clamping information for all of the available tracks
	float&										CurrentDragXPosition;	// Current X position of the drag operation
	FPanTrackRequest							RequestTrackPan;		// Delegate to request a pan along the edges of a zoomed track
	TArray<float>								NodeTimes;				// Times to drop each selected node at
	float										SnapTime;				// The time that the snapped node was snapped to
	TWeakPtr<SPaperZDAnimNotifyNode>			SnappedNode;			// The node chosen for the snap
	TArray<TSharedPtr<SPaperZDAnimNotifyNode>>	SelectedNodes;			// The nodes that are in the current selection
	TArray<float>								NodeTimeOffsets;		// Time offsets from the beginning of the selection to the nodes.
	TArray<float>								NodeXOffsets;			// Offsets in X from the widget position to the scrub handle for each node.
	FVector2D									NodeGroupPosition;		// Position of the beginning of the selection
	FVector2D									NodeGroupSize;			// Size of the entire selection
	TSharedPtr<SWidget>							Decorator;				// The widget to display when dragging
	float										SelectionTimeLength;	// Length of time that the selection covers
	int32										TrackSpan;				// Number of tracks that the selection spans
	FOnUpdatePanel								OnUpdatePanel;			// Delegate to redraw the notify panel
	FOnSnapPosition								OnSnapPosition;			// Delegate used to snap times

	static TSharedRef<FNotifyDragDropOp> New(
		TArray<TSharedPtr<SPaperZDAnimNotifyNode>>			NotifyNodes, 
		TSharedPtr<SWidget>									Decorator, 
		const TArray<TSharedPtr<SPaperZDAnimNotifyTrack>>&	NotifyTracks, 
		class UPaperZDAnimSequence*							InSequence, 
		const FVector2D&									CursorPosition, 
		const FVector2D&									SelectionScreenPosition, 
		const FVector2D&									SelectionSize, 
		float&												CurrentDragXPosition, 
		FPanTrackRequest&									RequestTrackPanDelegate, 
		FOnSnapPosition&									OnSnapPosition,
		FOnUpdatePanel&										UpdatePanel
		)
	{
		TSharedRef<FNotifyDragDropOp> Operation = MakeShareable(new FNotifyDragDropOp(CurrentDragXPosition));
		Operation->Sequence = InSequence;
		Operation->RequestTrackPan = RequestTrackPanDelegate;
		Operation->OnUpdatePanel = UpdatePanel;

		Operation->NodeGroupPosition = SelectionScreenPosition;
		Operation->NodeGroupSize = SelectionSize;
		Operation->DragOffset = SelectionScreenPosition - CursorPosition;
		Operation->OnSnapPosition = OnSnapPosition;
		Operation->Decorator = Decorator;
		Operation->SelectedNodes = NotifyNodes;
		Operation->TrackSpan = NotifyNodes.Last()->GetTrackIndex() - NotifyNodes[0]->GetTrackIndex();
		
		// Caclulate offsets for the selected nodes
		float BeginTime = MAX_flt;
		for(TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
		{
			float NotifyTime = Node->GetTime();

			if(NotifyTime < BeginTime)
			{
				BeginTime = NotifyTime;
			}
		}

		// Initialise node data
		for(TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
		{
			float NotifyTime = Node->GetTime();

			Node->ClearLastSnappedTime();
			Operation->NodeTimeOffsets.Add(NotifyTime - BeginTime);
			Operation->NodeTimes.Add(NotifyTime);
			Operation->NodeXOffsets.Add(Node->GetNotifyPositionOffset().X);

			// Calculate the time length of the selection. Because it is possible to have states
			// with arbitrary durations we need to search all of the nodes and find the furthest
			// possible point
			Operation->SelectionTimeLength = FMath::Max(Operation->SelectionTimeLength, NotifyTime + Node->GetDurationSize() - BeginTime); //@TODO: GETDURATIONSIZE here instead of time?
		}

		Operation->Construct();

		for(int32 i = 0; i < NotifyTracks.Num(); ++i)
		{
			FTrackClampInfo Info;
			Info.NotifyTrack = NotifyTracks[i];
			const FGeometry& CachedGeometry = Info.NotifyTrack->GetCachedGeometry();
			Info.TrackPos = CachedGeometry.AbsolutePosition.Y;
			Info.TrackSnapTestPos = Info.TrackPos + (CachedGeometry.Size.Y / 2);
			Operation->ClampInfos.Add(Info);
		}

		Operation->CursorDecoratorWindow->SetOpacity(0.5f);
		return Operation;
	}
	
	/** The widget decorator to use */
	virtual TSharedPtr<SWidget> GetDefaultDecorator() const override
	{
		return Decorator;
	}

	FText GetHoverText() const
	{
		FText HoverText = LOCTEXT("Invalid", "Invalid");

		if(SelectedNodes[0].IsValid())
		{
			HoverText = FText::FromName( SelectedNodes[0]->GetNotifyName() );
		}

		return HoverText;
	}
};

//////////////////////////////////////////////////////////////////////////
// SPaperZDAnimNotifyNode Implementation

void SPaperZDAnimNotifyNode::Construct(const FArguments& InArgs)
{
	Notify = InArgs._Notify;
	AnimSequence = InArgs._AnimSequence;
	TrackIndex = InArgs._TrackIndex;
	Font = FCoreStyle::GetDefaultFontStyle("Regular", 10);
	bBeingDragged = false;
	CurrentDragHandle = ENotifyStateHandleHit::None;
	bDrawTooltipToRight = true;
	bSelected = false;
	DragMarkerTransactionIdx = INDEX_NONE;

	OnNodeDragStarted = InArgs._OnNodeDragStarted;
	PanTrackRequest = InArgs._PanTrackRequest;
	OnSelectionChanged = InArgs._OnSelectionChanged;
	OnUpdatePanel = InArgs._OnUpdatePanel;

	ViewInputMin = InArgs._ViewInputMin;
	ViewInputMax = InArgs._ViewInputMax;
	OnSnapPosition = InArgs._OnSnapPosition;

	//If its a notify state, then it needs an end marker.
	if (const UPaperZDAnimNotifyState* StateNotify = Cast<const UPaperZDAnimNotifyState>(Notify))
	{
		SAssignNew(EndMarkerNodeOverlay, SOverlay);
	}

	SetClipping(EWidgetClipping::ClipToBounds);
	SetToolTipText(TAttribute<FText>(this, &SPaperZDAnimNotifyNode::GetNodeTooltip));
}

void SPaperZDAnimNotifyNode::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	ScreenPosition = AllottedGeometry.GetAbsolutePosition();
}

bool SPaperZDAnimNotifyNode::SupportsKeyboardFocus() const
{
	return true;
}

FReply SPaperZDAnimNotifyNode::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D ScreenNodePosition = MyGeometry.GetAbsolutePosition();

	// Whether the drag has hit a duration marker
	bool bDragOnMarker = false;
	bBeingDragged = true;

	//Add Drag Markers
	if (GetDurationSize() > 0.0f)
	{
		// This is a state node, check for a drag on the markers before movement. Use last screen space position before the drag started
		// as using the last position in the mouse event gives us a mouse position after the drag was started.
		ENotifyStateHandleHit::Type MarkerHit = DurationHandleHitTest(MouseDownLastPosition);
		if (MarkerHit == ENotifyStateHandleHit::Start || MarkerHit == ENotifyStateHandleHit::End)
		{
			bDragOnMarker = true;
			bBeingDragged = false;
			CurrentDragHandle = MarkerHit;

			// Modify the owning sequence as we're now dragging the marker and begin a transaction
			check(DragMarkerTransactionIdx == INDEX_NONE);
			DragMarkerTransactionIdx = GEditor->BeginTransaction(NSLOCTEXT("AnimNotifyNode", "AnimSequenceDragTransation", "Drag State Node Marker"));
			Notify->Modify();
		}
	}

	return OnNodeDragStarted.Execute(SharedThis(this), MouseEvent, ScreenNodePosition, bDragOnMarker);
}

FReply SPaperZDAnimNotifyNode::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool bLeftButton = MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	bool bShift = MouseEvent.IsShiftDown();

	if (bLeftButton && CurrentDragHandle != ENotifyStateHandleHit::None)
	{
		// Clear the drag marker and give the mouse back
		CurrentDragHandle = ENotifyStateHandleHit::None;
		
		// Signal selection changing so details panels get updated
		OnSelectionChanged.ExecuteIfBound();

		// End drag transaction before handing mouse back
		check(DragMarkerTransactionIdx != INDEX_NONE);
		GEditor->EndTransaction();
		DragMarkerTransactionIdx = INDEX_NONE;

		OnUpdatePanel.ExecuteIfBound();

		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

FReply SPaperZDAnimNotifyNode::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// Don't do scrub handle dragging if we haven't captured the mouse.
	if (!this->HasMouseCapture()) return FReply::Unhandled();

	if (CurrentDragHandle == ENotifyStateHandleHit::None)
	{
		// We've had focus taken away - realease the mouse
		FSlateApplication::Get().ReleaseAllPointerCapture();
		return FReply::Unhandled();
	}

	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, CachedAllotedGeometrySize);

	float XPositionInTrack = MyGeometry.AbsolutePosition.X - CachedTrackGeometry.AbsolutePosition.X;
	float TrackScreenSpaceXPosition = MyGeometry.AbsolutePosition.X - XPositionInTrack;
	float TrackScreenSpaceOrigin = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(0.0f), 0.0f)).X;
	float TrackScreenSpaceLimit = CachedTrackGeometry.LocalToAbsolute(FVector2D(ScaleInfo.InputToLocalX(AnimSequence->GetTotalDuration()), 0.0f)).X;

	//Minimum duration we will be clamping at, defined at the state object
	const float MinimumStateDuration = UPaperZDAnimNotifyState::MinimumStateDuration;

	if (CurrentDragHandle == ENotifyStateHandleHit::Start)
	{
		// Check track bounds
		float OldDisplayTime = GetTime();

		if (MouseEvent.GetScreenSpacePosition().X >= TrackScreenSpaceXPosition && MouseEvent.GetScreenSpacePosition().X <= TrackScreenSpaceXPosition + CachedAllotedGeometrySize.X)
		{
			float NewDisplayTime = ScaleInfo.LocalXToInput((MouseEvent.GetScreenSpacePosition() - MyGeometry.GetAbsolutePosition() + XPositionInTrack).X);
			float NewDuration = GetDuration() + OldDisplayTime - NewDisplayTime;

			// Check to make sure the duration is not less than the minimum allowed
			if (NewDuration < MinimumStateDuration)
			{
				NewDisplayTime -= MinimumStateDuration - NewDuration;
			}

			SetTime(FMath::Max(0.0f, NewDisplayTime));
			SetDuration(GetDuration() + OldDisplayTime - GetTime());
		}
		else if (GetDuration() > MinimumStateDuration)
		{
			float Overflow = HandleOverflowPan(MouseEvent.GetScreenSpacePosition(), TrackScreenSpaceXPosition, TrackScreenSpaceOrigin, TrackScreenSpaceLimit);

			// Update scale info to the new view inputs after panning
			ScaleInfo.ViewMinInput = ViewInputMin.Get();
			ScaleInfo.ViewMaxInput = ViewInputMax.Get();

			float NewDisplayTime = ScaleInfo.LocalXToInput((MouseEvent.GetScreenSpacePosition() - MyGeometry.GetAbsolutePosition() + XPositionInTrack).X);
			SetTime(FMath::Max(0.0f, NewDisplayTime));
			SetDuration(GetDuration() + OldDisplayTime - GetTime());

			// Adjust in case we went under the minimum
			if (GetDuration() < MinimumStateDuration)
			{
				float EndTimeBefore = GetTime() + GetDuration();
				SetTime(GetTime() + GetDuration() - MinimumStateDuration);
				SetDuration(MinimumStateDuration);
				float EndTimeAfter = GetTime() + GetDuration();
			}
		}

		// Now we know where the marker should be, look for possible snaps on montage marker bars
		{
			float InputStartTime = GetTime();
			float MarkerSnap = GetScrubHandleSnapPosition(InputStartTime, ENotifyStateHandleHit::Start);
			if (MarkerSnap != -1.0f)
			{
				// We're near to a snap bar, adjust our start marker
				OldDisplayTime = GetTime();
				SetTime(MarkerSnap);
				SetDuration(GetDuration() + OldDisplayTime - GetTime());
			}
		}
	}
	else
	{
		if (MouseEvent.GetScreenSpacePosition().X >= TrackScreenSpaceXPosition && MouseEvent.GetScreenSpacePosition().X <= TrackScreenSpaceXPosition + CachedAllotedGeometrySize.X)
		{
			float NewDuration = ScaleInfo.LocalXToInput((MouseEvent.GetScreenSpacePosition() - MyGeometry.GetAbsolutePosition() + XPositionInTrack).X) - GetTime();

			SetDuration(FMath::Max(NewDuration, MinimumStateDuration));
		}
		else if (GetDuration() > MinimumStateDuration)
		{
			float Overflow = HandleOverflowPan(MouseEvent.GetScreenSpacePosition(), TrackScreenSpaceXPosition, TrackScreenSpaceOrigin, TrackScreenSpaceLimit);

			// Update scale info to the new view inputs after panning
			ScaleInfo.ViewMinInput = ViewInputMin.Get();
			ScaleInfo.ViewMaxInput = ViewInputMax.Get();

			float NewDuration = ScaleInfo.LocalXToInput((MouseEvent.GetScreenSpacePosition() - MyGeometry.GetAbsolutePosition() + XPositionInTrack).X) - GetTime();
			SetDuration(FMath::Max(NewDuration, MinimumStateDuration));
		}

		if (GetTime() + GetDuration() > AnimSequence->GetTotalDuration())
		{
			SetDuration(AnimSequence->GetTotalDuration() - GetTime());
		}

		// Now we know where the scrub handle should be, look for possible snaps on montage marker bars
		{
			float InputEndTime = GetTime() + GetDuration();
			float MarkerSnap = GetScrubHandleSnapPosition(InputEndTime, ENotifyStateHandleHit::End);
			if (MarkerSnap != -1.0f)
			{
				// Adjust our end marker
				SetDuration(MarkerSnap - GetTime());
			}
		}
	}

	return FReply::Handled();
}

void SPaperZDAnimNotifyNode::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (CurrentDragHandle != ENotifyStateHandleHit::None)
	{
		// Lost focus while dragging a state node, clear the drag and end the current transaction
		CurrentDragHandle = ENotifyStateHandleHit::None;

		check(DragMarkerTransactionIdx != INDEX_NONE);
		GEditor->EndTransaction();
		DragMarkerTransactionIdx = INDEX_NONE;
	}
}

FText SPaperZDAnimNotifyNode::GetNodeTooltip() const 
{
	const FText Seconds = FText::AsNumber(Notify->Time);
	return FText::Format(LOCTEXT("ZDNodeToolTip", "@ {0} sec"), Seconds);
}

void SPaperZDAnimNotifyNode::SetTime(float InTime)
{
	if (Notify.IsValid())
	{
		Notify->Time = InTime;
	}
}

void SPaperZDAnimNotifyNode::SetDuration(float InDuration)
{
	UPaperZDAnimNotifyState* StateNotify = Cast<UPaperZDAnimNotifyState>(Notify.Get());
	if (StateNotify)
	{
		StateNotify->Duration = InDuration;
	}
}

ENotifyStateHandleHit::Type SPaperZDAnimNotifyNode::DurationHandleHitTest(const FVector2D& CursorTrackPosition) const
{
	ENotifyStateHandleHit::Type MarkerHit = ENotifyStateHandleHit::None;

	// Make sure this node has a duration box (meaning it is a state node)
	if (NotifyDurationSizeX > 0.0f)
	{
		// Test for mouse inside duration box with handles included
		float ScrubHandleHalfWidth = ScrubHandleSize.X / 2.0f;

		// Position and size of the notify node including the scrub handles
		FVector2D NotifyNodePosition(NotifyScrubHandleCentre - ScrubHandleHalfWidth, 0.0f);
		FVector2D NotifyNodeSize(NotifyDurationSizeX + ScrubHandleHalfWidth * 2.0f, NotifyHeight);

		FVector2D MouseRelativePosition(CursorTrackPosition - GetPosition());

		if (MouseRelativePosition.ComponentwiseAllGreaterThan(NotifyNodePosition) &&
			MouseRelativePosition.ComponentwiseAllLessThan(NotifyNodePosition + NotifyNodeSize))
		{
			// Definitely inside the duration box, need to see which handle we hit if any
			if (MouseRelativePosition.X <= (NotifyNodePosition.X + ScrubHandleSize.X))
			{
				// Left Handle
				MarkerHit = ENotifyStateHandleHit::Start;
			}
			else if (MouseRelativePosition.X >= (NotifyNodePosition.X + NotifyNodeSize.X - ScrubHandleSize.X))
			{
				// Right Handle
				MarkerHit = ENotifyStateHandleHit::End;
			}
		}
	}

	return MarkerHit;
}

void SPaperZDAnimNotifyNode::DropCancelled()
{
	bBeingDragged = false;
}

float SPaperZDAnimNotifyNode::HandleOverflowPan(const FVector2D& ScreenCursorPos, float TrackScreenSpaceXPosition, float TrackScreenSpaceMin, float TrackScreenSpaceMax)
{
	float Overflow = 0.0f;

	if (ScreenCursorPos.X < TrackScreenSpaceXPosition && TrackScreenSpaceXPosition > TrackScreenSpaceMin - 10.0f)
	{
		// Overflow left edge
		Overflow = FMath::Min(ScreenCursorPos.X - TrackScreenSpaceXPosition, -10.0f);
	}
	else if (ScreenCursorPos.X > CachedAllotedGeometrySize.X && (TrackScreenSpaceXPosition + CachedAllotedGeometrySize.X) < TrackScreenSpaceMax + 10.0f)
	{
		// Overflow right edge
		Overflow = FMath::Max(ScreenCursorPos.X - (TrackScreenSpaceXPosition + CachedAllotedGeometrySize.X), 10.0f);
	}

	PanTrackRequest.ExecuteIfBound(Overflow, CachedAllotedGeometrySize);

	return Overflow;
}

float SPaperZDAnimNotifyNode::GetScrubHandleSnapPosition(float NotifyInputX, ENotifyStateHandleHit::Type HandleToCheck)
{
	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, CachedAllotedGeometrySize);

	const float MaxSnapDist = 5.0f;

	if (OnSnapPosition.IsBound() && !FSlateApplication::Get().GetModifierKeys().IsControlDown())
	{
		if (OnSnapPosition.Execute(NotifyInputX, MaxSnapDist / ScaleInfo.PixelsPerInput, TArrayView<const FName>()))
		{
			return NotifyInputX;
		}
	}

	return -1.0f;
}

void SPaperZDAnimNotifyNode::UpdateSizeAndPosition(const FGeometry& AllottedGeometry)
{
	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, AllottedGeometry.Size);

	// Cache the geometry information, the alloted geometry is the same size as the track.
	CachedAllotedGeometrySize = AllottedGeometry.Size * AllottedGeometry.Scale;

	NotifyTimePositionX = ScaleInfo.InputToLocalX(GetTime());
	NotifyDurationSizeX = ScaleInfo.PixelsPerInput * GetDuration();

	const TSharedRef< FSlateFontMeasure > FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TextSize = FontMeasureService->Measure(FText::FromName(GetNotifyName()), Font);
	LabelWidth = TextSize.X + (TextBorderSize.X * 2.f) + (ScrubHandleSize.X / 2.f);

	//No branching points 
// 	bool bDrawBranchingPoint = NodeObjectInterface->IsBranchingPoint();
// 	BranchingPointIconSize = FVector2D(TextSize.Y, TextSize.Y);
// 	if (bDrawBranchingPoint)
// 	{
// 		LabelWidth += BranchingPointIconSize.X + TextBorderSize.X * 2.f;
// 	}

	//Calculate scrub handle box size (the notional box around the scrub handle and the alignment marker)
	float NotifyHandleBoxWidth = FMath::Max(ScrubHandleSize.X, AlignmentMarkerSize.X * 2);

	// Work out where we will have to draw the tool tip
	float LeftEdgeToNotify = NotifyTimePositionX;
	float RightEdgeToNotify = AllottedGeometry.Size.X - NotifyTimePositionX;
	bDrawTooltipToRight = NotifyDurationSizeX > 0.0f || ((RightEdgeToNotify > LabelWidth) || (RightEdgeToNotify > LeftEdgeToNotify));

	// Calculate widget width/position based on where we are drawing the tool tip
	Position.X = bDrawTooltipToRight ? (NotifyTimePositionX - (NotifyHandleBoxWidth / 2.f)) : (NotifyTimePositionX - LabelWidth);
	Size = bDrawTooltipToRight ? FVector2D((NotifyDurationSizeX > 0.0f ? NotifyDurationSizeX : FMath::Max(LabelWidth, NotifyDurationSizeX)), NotifyHeight) : FVector2D((LabelWidth + NotifyDurationSizeX), NotifyHeight);
	Size.X += NotifyHandleBoxWidth;

	if (EndMarkerNodeOverlay.IsValid())
	{
		FVector2D OverlaySize = EndMarkerNodeOverlay->GetDesiredSize();
		Size.X += OverlaySize.X;
	}

	// Widget position of the notify marker
	NotifyScrubHandleCentre = bDrawTooltipToRight ? NotifyHandleBoxWidth / 2.f : LabelWidth;
}

FCursorReply SPaperZDAnimNotifyNode::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	// Show resize cursor if the cursor is hovering over either of the scrub handles of a notify state node
	if (IsHovered() && GetDurationSize() > 0.0f)
	{
		FVector2D RelMouseLocation = MyGeometry.AbsoluteToLocal(CursorEvent.GetScreenSpacePosition());

		const float HandleHalfWidth = ScrubHandleSize.X / 2.0f;
		const float DistFromFirstHandle = FMath::Abs(RelMouseLocation.X - NotifyScrubHandleCentre);
		const float DistFromSecondHandle = FMath::Abs(RelMouseLocation.X - (NotifyScrubHandleCentre + NotifyDurationSizeX));

		if (DistFromFirstHandle < HandleHalfWidth || DistFromSecondHandle < HandleHalfWidth || CurrentDragHandle != ENotifyStateHandleHit::None)
		{
			return FCursorReply::Cursor(EMouseCursor::ResizeLeftRight);
		}
	}
	return FCursorReply::Unhandled();
}

FVector2D SPaperZDAnimNotifyNode::ComputeDesiredSize(float) const
{
	return GetSize();
}

bool SPaperZDAnimNotifyNode::HitTest(const FGeometry& AllottedGeometry, FVector2D MouseLocalPose) const
{
	FVector2D LocPosition = GetPosition();
	FVector2D LocSize = GetSize();

	return MouseLocalPose.ComponentwiseAllGreaterOrEqual(LocPosition) && MouseLocalPose.ComponentwiseAllLessOrEqual(LocPosition + LocSize);
}

int32 SPaperZDAnimNotifyNode::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	//Don't draw anything if we have stale data
	if (!Notify.IsValid())
	{
		return LayerId;
	}

	int32 MarkerLayer = LayerId + 1;
	int32 ScrubHandleID = MarkerLayer + 1;
	int32 TextLayerID = ScrubHandleID + 1;
	int32 BranchPointLayerID = TextLayerID + 1;

	// Paint marker node if we have one
	if (EndMarkerNodeOverlay.IsValid())
	{
		FVector2D MarkerSize = EndMarkerNodeOverlay->GetDesiredSize();
		FVector2D MarkerOffset(NotifyDurationSizeX + MarkerSize.X * 0.5f + 5.0f, (NotifyHeight - MarkerSize.Y) * 0.5f);
		EndMarkerNodeOverlay->Paint(Args.WithNewParent(this), AllottedGeometry.MakeChild(MarkerSize, FSlateLayoutTransform(MarkerOffset)), MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	}

	const FSlateBrush* StyleInfo = FAppStyle::GetBrush(TEXT("SpecialEditableTextImageNormal"));

	FText Text = FText::FromName(GetNotifyName());
	FLinearColor NodeColor = GetNotifyObject()->Color.CopyWithNewOpacity(0.5f);
	FLinearColor BoxColor = bSelected ? FAppStyle::GetSlateColor("SelectionColor").GetSpecifiedColor() : NodeColor;

	float HalfScrubHandleWidth = ScrubHandleSize.X / 2.0f;

	// Show duration of AnimNotifyState
	if (NotifyDurationSizeX > 0.f)
	{
		FVector2D DurationBoxSize = FVector2D(NotifyDurationSizeX, TextSize.Y + TextBorderSize.Y * 2.f);
		FVector2D DurationBoxPosition = FVector2D(NotifyScrubHandleCentre, (NotifyHeight - TextSize.Y) * 0.5f);
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(DurationBoxSize, FSlateLayoutTransform(DurationBoxPosition)),
			StyleInfo,
			ESlateDrawEffect::None,
			BoxColor);

		DrawScrubHandle(DurationBoxPosition.X + DurationBoxSize.X, OutDrawElements, ScrubHandleID, AllottedGeometry, MyCullingRect, NodeColor);
	}

	// Background
	FVector2D LabelSize = TextSize + TextBorderSize * 2.f;
	LabelSize.X += HalfScrubHandleWidth;

	FVector2D LabelPosition(bDrawTooltipToRight ? NotifyScrubHandleCentre : NotifyScrubHandleCentre - LabelSize.X, (NotifyHeight - TextSize.Y) * 0.5f);

	if (NotifyDurationSizeX == 0.f)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(LabelSize, FSlateLayoutTransform(LabelPosition)),
			StyleInfo,
			ESlateDrawEffect::None,
			BoxColor);
	}

	// Text
	FVector2D TextPosition = LabelPosition + TextBorderSize;
	if (bDrawTooltipToRight)
	{
		TextPosition.X += HalfScrubHandleWidth;
	}

	FVector2D DrawTextSize;
	DrawTextSize.X = (NotifyDurationSizeX > 0.0f ? FMath::Min(NotifyDurationSizeX - ScrubHandleSize.X, TextSize.X) : TextSize.X);
	DrawTextSize.Y = TextSize.Y;

	FPaintGeometry TextGeometry = AllottedGeometry.ToPaintGeometry(DrawTextSize, FSlateLayoutTransform(TextPosition));
	OutDrawElements.PushClip(FSlateClippingZone(TextGeometry));

	FSlateDrawElement::MakeText(
		OutDrawElements,
		TextLayerID,
		TextGeometry,
		Text,
		Font,
		ESlateDrawEffect::None,
		FLinearColor::Black
	);

	OutDrawElements.PopClip();

	DrawScrubHandle(NotifyScrubHandleCentre, OutDrawElements, ScrubHandleID, AllottedGeometry, MyCullingRect, NodeColor);

	return TextLayerID;
}

void SPaperZDAnimNotifyNode::DrawScrubHandle(float ScrubHandleCentre, FSlateWindowElementList& OutDrawElements, int32 ScrubHandleID, const FGeometry &AllottedGeometry, const FSlateRect& MyClippingRect, FLinearColor NodeColour) const
{
	FVector2D ScrubHandlePosition(ScrubHandleCentre - ScrubHandleSize.X / 2.0f, (NotifyHeight - ScrubHandleSize.Y) / 2.f);
	FSlateDrawElement::MakeBox(
		OutDrawElements,
		ScrubHandleID,
		AllottedGeometry.ToPaintGeometry(ScrubHandleSize, FSlateLayoutTransform(ScrubHandlePosition)),
		FAppStyle::GetBrush(TEXT("Sequencer.KeyDiamond")),
		ESlateDrawEffect::None,
		NodeColour
	);

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		ScrubHandleID,
		AllottedGeometry.ToPaintGeometry(ScrubHandleSize, FSlateLayoutTransform(ScrubHandlePosition)),
		FAppStyle::GetBrush(TEXT("Sequencer.KeyDiamondBorder")),
		ESlateDrawEffect::None,
		bSelected ? FAppStyle::GetSlateColor("SelectionColor").GetSpecifiedColor() : FLinearColor::Black
	);
}

//////////////////////////////////////////////////////////////////////////
// SPaperZDAnimNotifyTrack Implementation
void SPaperZDAnimNotifyTrack::Construct(const FArguments& InArgs)
{
	SetClipping(EWidgetClipping::ClipToBounds);

	WeakCommandList = InArgs._CommandList;
	AnimSequence = InArgs._AnimSequence;
	ViewInputMin = InArgs._ViewInputMin;
	ViewInputMax = InArgs._ViewInputMax;
	OnSelectionChanged = InArgs._OnSelectionChanged;
	TrackIndex = InArgs._TrackIndex;
	OnNodesDragStarted = InArgs._OnNodesDragStarted;
	OnUpdatePanel = InArgs._OnUpdatePanel;
	OnDeselectAllNotifies = InArgs._OnDeselectAllNotifies;
	OnGetAllSelectedNotifyObjects = InArgs._OnGetAllSelectedNotifyObjects;
	OnDeleteSelectedNotifies = InArgs._OnDeleteSelectedNotifies;
	OnSnapPosition = InArgs._OnSnapPosition;
	OnRequestTrackPan = InArgs._OnRequestTrackPan;
	OnGetDraggedNodePos = InArgs._OnGetDraggedNodePos;
	OnPasteNodes = InArgs._OnPasteNodes;
	OnInvokeTab = InArgs._OnInvokeTab;

	ChildSlot
	[
		SAssignNew(TrackArea, SBorder)
		.Visibility(EVisibility::SelfHitTestInvisible)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.Padding(FMargin(0.f, 0.f))
	];

	//Create the contents of the track area
	Update();
}

void SPaperZDAnimNotifyTrack::UpdateCachedGeometry(const FGeometry& InGeometry)
{
	CachedGeometry = InGeometry;

	for (TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
	{
		Node->CachedTrackGeometry = InGeometry;
	}
}

void SPaperZDAnimNotifyTrack::Update()
{
	//Clear all the notifies and reset the track itself.
	NotifyNodes.Empty();
	TrackArea->SetContent(
		SAssignNew(NodeOverlay, SOverlay)
	);
	
	//Obtain a temporary track object and create the node widgets from it
	const FPaperZDAnimTrack& Track = AnimSequence->GetTrackChecked(TrackIndex);
	for (int32 NotifyIndex = 0; NotifyIndex < Track.AnimNotifies.Num(); NotifyIndex++)
	{
		//Create a new visual notify node and assign it
		TSharedPtr<SPaperZDAnimNotifyNode> Node;
		SAssignNew(Node, SPaperZDAnimNotifyNode)
			.Notify(Track.AnimNotifies[NotifyIndex])
			.AnimSequence(AnimSequence)
			.TrackIndex(TrackIndex)
			.OnNodeDragStarted(this, &SPaperZDAnimNotifyTrack::OnNotifyNodeDragStarted, NotifyIndex)
			.OnUpdatePanel(OnUpdatePanel)
			.PanTrackRequest(OnRequestTrackPan)
			.ViewInputMin(ViewInputMin)
			.ViewInputMax(ViewInputMax)
			.OnSnapPosition(OnSnapPosition)
			.OnSelectionChanged(OnSelectionChanged);

		//Create the overlay of this node
		NodeOverlay->AddSlot()
			.Padding(TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SPaperZDAnimNotifyTrack::GetNotifyTrackPadding, NotifyIndex)))
			.VAlign(VAlign_Center)
			[
				Node.ToSharedRef()
			];

		//Add to the array for later referencing
		NotifyNodes.Add(Node);
	}
}

void SPaperZDAnimNotifyTrack::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{ 
	UpdateCachedGeometry(AllottedGeometry);

	//Validate the nodes, we don't want to process them if they have stale data (due to blueprint compilation)
	for (TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
	{
		if (!Node->IsValid())
		{
			OnUpdatePanel.ExecuteIfBound();
			return;
		}
	}
}

int32 SPaperZDAnimNotifyTrack::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FSlateBrush* StyleInfo = FAppStyle::GetBrush(TEXT("Persona.NotifyEditor.NotifyTrackBackground"));
	const FPaperZDAnimTrackMetadata& Track = AnimSequence->GetTrackMetadataChecked(TrackIndex);
	const FPaintGeometry MyGeometry = AllottedGeometry.ToPaintGeometry();
	int32 CustomLayerId = LayerId;

	bool bAnyDraggedNodes = false;
	for (int32 i = 0; i < NotifyNodes.Num(); i++)
	{
		if (NotifyNodes[i].Get()->IsBeingDragged() == false)
		{
			NotifyNodes[i].Get()->UpdateSizeAndPosition(AllottedGeometry);
		}
		else
		{
			bAnyDraggedNodes = true;
		}
	}

	// Draw track bottom border
	if (TrackIndex < AnimSequence->GetNumTracks() - 1)
	{
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			CustomLayerId,
			AllottedGeometry.ToPaintGeometry(),
			TArray<FVector2D>({ FVector2D(0.0f, AllottedGeometry.GetLocalSize().Y), FVector2D(AllottedGeometry.GetLocalSize().X, AllottedGeometry.GetLocalSize().Y) }),
			ESlateDrawEffect::None,
			FLinearColor(0.1f, 0.1f, 0.1f, 0.3f)
		);
	}

	++CustomLayerId;

	float Value = 0.f;
	if (bAnyDraggedNodes && OnGetDraggedNodePos.IsBound())
	{
		Value = OnGetDraggedNodePos.Execute();

		if (Value >= 0.0f)
		{
			float XPos = Value;
			TArray<FVector2D> LinePoints;
			LinePoints.Add(FVector2D(XPos, 0.f));
			LinePoints.Add(FVector2D(XPos, AllottedGeometry.Size.Y));

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				CustomLayerId,
				MyGeometry,
				LinePoints,
				ESlateDrawEffect::None,
				FLinearColor(1.0f, 0.5f, 0.0f)
			);
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, CustomLayerId, InWidgetStyle, bParentEnabled);
}

FCursorReply SPaperZDAnimNotifyTrack::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	if (ViewInputMin.Get() > 0.f || ViewInputMax.Get() < AnimSequence->GetTotalDuration())
	{
		return FCursorReply::Cursor(EMouseCursor::GrabHand);
	}

	return FCursorReply::Unhandled();
}

FReply SPaperZDAnimNotifyTrack::OnNotifyNodeDragStarted(TSharedRef<SPaperZDAnimNotifyNode> NotifyNode, const FPointerEvent& MouseEvent, const FVector2D& ScreenNodePosition, const bool bDragOnMarker, int32 NotifyIndex)
{
	//Make sure that this node is selected
	if (!NotifyNode->bSelected)
	{
		SelectTrackObjectNode(NotifyIndex, MouseEvent.IsShiftDown(), false);
	}

	// Sort our nodes so we're acessing them in time order
	SelectedNodeIndices.Sort([this](const int32& A, const int32& B)
		{
			float TimeA = NotifyNodes[A]->GetTime();
			float TimeB = NotifyNodes[B]->GetTime();

			return TimeA < TimeB;
		});

	// If we're dragging one of the direction markers we don't need to call any further as we don't want the drag drop op
	if (!bDragOnMarker)
	{
		TArray<TSharedPtr<SPaperZDAnimNotifyNode>> NodesToDrag;
		const float FirstNodeX = NotifyNodes[SelectedNodeIndices[0]]->GetPosition().X;

		TSharedRef<SOverlay> DragBox = SNew(SOverlay);
		for (auto Iter = SelectedNodeIndices.CreateIterator(); Iter; ++Iter)
		{
			TSharedPtr<SPaperZDAnimNotifyNode> Node = NotifyNodes[*Iter];
			NodesToDrag.Add(Node);
		}

		FVector2D DecoratorPosition = NodesToDrag[0]->GetPosition();
		DecoratorPosition = CachedGeometry.LocalToAbsolute(DecoratorPosition);
		return OnNodesDragStarted.Execute(NodesToDrag, DragBox, MouseEvent.GetScreenSpacePosition(), DecoratorPosition, bDragOnMarker);
	}
	else
	{
		// Capture the mouse in the node
		return FReply::Handled().CaptureMouse(NotifyNode).UseHighPrecisionMouseMovement(NotifyNode);
	}
}

void SPaperZDAnimNotifyTrack::SelectTrackObjectNode(int32 TrackNodeIndex, bool Append, bool bUpdateSelection)
{
	if (TrackNodeIndex != INDEX_NONE)
	{
		// Deselect all other notifies if necessary.
		if (AnimSequence && !Append)
		{
			OnDeselectAllNotifies.ExecuteIfBound();
		}
		
		// Check to see if we've already selected this node
		if (!SelectedNodeIndices.Contains(TrackNodeIndex))
		{
			// select new one
			if (NotifyNodes.IsValidIndex(TrackNodeIndex))
			{
				TSharedPtr<SPaperZDAnimNotifyNode> Node = NotifyNodes[TrackNodeIndex];
				Node->bSelected = true;
				SelectedNodeIndices.Add(TrackNodeIndex);

				if (bUpdateSelection)
				{
					SendSelectionChanged();
				}
			}
		}
	}
}

void SPaperZDAnimNotifyTrack::ToggleTrackObjectNodeSelectionStatus(int32 TrackNodeIndex, bool bUpdateSelection)
{
	check(NotifyNodes.IsValidIndex(TrackNodeIndex));

	bool bSelected = SelectedNodeIndices.Contains(TrackNodeIndex);
	if (bSelected)
	{
		SelectedNodeIndices.Remove(TrackNodeIndex);
	}
	else
	{
		SelectedNodeIndices.Add(TrackNodeIndex);
	}

	TSharedPtr<SPaperZDAnimNotifyNode> Node = NotifyNodes[TrackNodeIndex];
	Node->bSelected = !Node->bSelected;

	if (bUpdateSelection)
	{
		SendSelectionChanged();
	}
}

void SPaperZDAnimNotifyTrack::DeselectTrackObjectNode(int32 TrackNodeIndex, bool bUpdateSelection)
{
	check(NotifyNodes.IsValidIndex(TrackNodeIndex));
	TSharedPtr<SPaperZDAnimNotifyNode> Node = NotifyNodes[TrackNodeIndex];
	Node->bSelected = false;

	int32 ItemsRemoved = SelectedNodeIndices.Remove(TrackNodeIndex);
	check(ItemsRemoved > 0);

	if (bUpdateSelection)
	{
		SendSelectionChanged();
	}
}

void SPaperZDAnimNotifyTrack::DeselectAllNotifyNodes(bool bUpdateSelectionSet)
{
	for (TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
	{
		Node->bSelected = false;
	}
	SelectedNodeIndices.Empty();

	if (bUpdateSelectionSet)
	{
		SendSelectionChanged();
	}
}

void SPaperZDAnimNotifyTrack::DisconnectSelectedNodesForDrag(TArray<TSharedPtr<SPaperZDAnimNotifyNode>>& DragNodes)
{
	for (int32 i = 0; i < NotifyNodes.Num(); i++)
	{
		TSharedPtr<SPaperZDAnimNotifyNode> Node = NotifyNodes[i];
		if (Node->bSelected)
		{
			NodeOverlay->RemoveSlot(Node.ToSharedRef());
			DragNodes.Add(Node);
		}
	}
}

void SPaperZDAnimNotifyTrack::HandleNodeDrop(TSharedPtr<SPaperZDAnimNotifyNode> Node, float Offset)
{
	//Set track
	const FScopedTransaction Transaction(LOCTEXT("DropNotify", "Move Notify"));
	AnimSequence->Modify();
	AnimSequence->MoveNotify(Node->Notify.Get(), TrackIndex);

	//Set time
	float LocalX = GetCachedGeometry().AbsoluteToLocal(Node->GetScreenPosition() + Offset).X;
	float SnapTime = Node->GetLastSnappedTime();
	float Time = SnapTime != -1.0f ? SnapTime : GetCachedScaleInfo().LocalXToInput(LocalX);

	Node->GetNotifyObject()->Modify();
	Node->GetNotifyObject()->Time = Time;
}


int32 SPaperZDAnimNotifyTrack::GetHitNotifyNode(const FGeometry& MyGeometry, const FVector2D& CursorPosition)
{
	for (int32 I = NotifyNodes.Num() - 1; I >= 0; --I) //Run through from 'top most' Notify to bottom
	{
		if (NotifyNodes[I].Get()->HitTest(MyGeometry, CursorPosition))
		{
			return I;
		}
	}

	return INDEX_NONE;
}

void SPaperZDAnimNotifyTrack::AppendSelectionToArray(TArray<UPaperZDAnimNotify_Base*>& Selection) const
{
	for (TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
	{
		if (Node->bSelected)
		{
			Selection.Add(Node->GetNotifyObject());
		}
	}
}

void SPaperZDAnimNotifyTrack::AppendSelectedNodeWidgetsToArray(TArray<TSharedPtr<SPaperZDAnimNotifyNode>>& NodeArray) const
{
	for (TSharedPtr<SPaperZDAnimNotifyNode> Node : NotifyNodes)
	{
		if (Node->bSelected)
		{
			NodeArray.Add(Node);
		}
	}
}

void SPaperZDAnimNotifyTrack::RefreshMarqueeSelectedNodes(FSlateRect& Rect, FPaperZDNotifyMarqueeOperation& Marquee)
{
	if (Marquee.Operation != FPaperZDNotifyMarqueeOperation::Replace)
	{
		// Maintain the original selection from before the operation
		for (int32 Idx = 0; Idx < NotifyNodes.Num(); ++Idx)
		{
			TSharedPtr<SPaperZDAnimNotifyNode> Notify = NotifyNodes[Idx];
			bool bWasSelected = Marquee.OriginalSelection.Contains(Notify);
			if (bWasSelected)
			{
				SelectTrackObjectNode(Idx, true, false);
			}
			else if (SelectedNodeIndices.Contains(Idx))
			{
				DeselectTrackObjectNode(Idx, false);
			}
		}
	}

	for (int32 Index = 0; Index < NotifyNodes.Num(); ++Index)
	{
		TSharedPtr<SPaperZDAnimNotifyNode> Node = NotifyNodes[Index];
		FSlateRect NodeRect = FSlateRect(Node->GetPosition(), Node->GetPosition() + Node->GetSize());

		if (FSlateRect::DoRectanglesIntersect(Rect, NodeRect))
		{
			// Either select or deselect the intersecting node, depending on the type of selection operation
			if (Marquee.Operation == FPaperZDNotifyMarqueeOperation::Remove)
			{
				if (SelectedNodeIndices.Contains(Index))
				{
					DeselectTrackObjectNode(Index, false);
				}
			}
			else
			{
				SelectTrackObjectNode(Index, true, false);
			}
		}
	}
}

void SPaperZDAnimNotifyTrack::OnPasteNotifyClicked(EPaperZDNotifyPasteMode PasteMode, EPaperZDNotifyPasteMultipleMode MultiplePasteType)
{
	float ClickTime = PasteMode == EPaperZDNotifyPasteMode::MousePosition ? LastClickedTime : -1.0f;
	OnPasteNodes.ExecuteIfBound(this, ClickTime, PasteMode, MultiplePasteType);
}

FReply SPaperZDAnimNotifyTrack::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool bLeftMouseButton = MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	bool bRightMouseButton = MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
	bool bShift = MouseEvent.IsShiftDown();
	bool bCtrl = MouseEvent.IsControlDown();

	if (bRightMouseButton)
	{
		TSharedPtr<SWidget> WidgetToFocus;
		WidgetToFocus = GetContextMenu(MyGeometry, MouseEvent);

		return (WidgetToFocus.IsValid())
			? FReply::Handled().ReleaseMouseCapture().SetUserFocus(WidgetToFocus.ToSharedRef(), EFocusCause::SetDirectly)
			: FReply::Handled().ReleaseMouseCapture();
	}
	else if (bLeftMouseButton)
	{
		FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();
		CursorPos = MyGeometry.AbsoluteToLocal(CursorPos);
		int32 NotifyIndex = GetHitNotifyNode(MyGeometry, CursorPos);
		LastClickedTime = CalculateTime(MyGeometry, MouseEvent.GetScreenSpacePosition());

		if (NotifyIndex == INDEX_NONE)
		{
			// Clicked in empty space, clear selection
			OnDeselectAllNotifies.ExecuteIfBound();
		}
		else
		{
			if (bCtrl)
			{
				ToggleTrackObjectNodeSelectionStatus(NotifyIndex);
			}
			else
			{
				SelectTrackObjectNode(NotifyIndex, bShift);
			}
		}

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SPaperZDAnimNotifyTrack::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();
	CursorPos = MyGeometry.AbsoluteToLocal(CursorPos);
	int32 HitIndex = GetHitNotifyNode(MyGeometry, CursorPos);

	if (HitIndex != INDEX_NONE)
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			// Hit a node, record the mouse position for use later so we can know when / where a
			// drag happened on the node handles if necessary.
			NotifyNodes[HitIndex]->SetLastMouseDownPosition(CursorPos);

			return FReply::Handled().DetectDrag(NotifyNodes[HitIndex].ToSharedRef(), EKeys::LeftMouseButton);
		}
		else if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
		{
			// Hit a node, return handled so we can pop a context menu on mouse up
			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

float SPaperZDAnimNotifyTrack::CalculateTime(const FGeometry& MyGeometry, FVector2D Position, bool bInputIsAbsolute)
{
	if (bInputIsAbsolute)
	{
		Position = MyGeometry.AbsoluteToLocal(Position);
	}
	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0, 0, MyGeometry.Size);
	return FMath::Clamp<float>(ScaleInfo.LocalXToInput(Position.X), 0.f, CachedGeometry.Size.X);
}

TSharedPtr<SWidget> SPaperZDAnimNotifyTrack::GetContextMenu(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{	
	FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();
	int32 NodeIndex = GetHitNotifyNode(MyGeometry, MyGeometry.AbsoluteToLocal(CursorPos));
	LastClickedTime = CalculateTime(MyGeometry, MouseEvent.GetScreenSpacePosition());

	const bool bCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bCloseWindowAfterMenuSelection, WeakCommandList.Pin());
	FUIAction NewAction;

	MenuBuilder.BeginSection("AnimNotify", LOCTEXT("NotifyHeading", "Notify"));
	{
		if (NodeIndex != INDEX_NONE)
		{
			if (!NotifyNodes[NodeIndex]->bSelected)
			{
				SelectTrackObjectNode(NodeIndex, MouseEvent.IsControlDown());
			}

			//Layout for notify edit tooltip window
			if(IsSingleNodeSelected())
			{
				// Add item to directly set notify time
				TSharedRef<SWidget> TimeWidget = 
					SNew( SBox )
					.HAlign( HAlign_Right )
					.ToolTipText(LOCTEXT("SetTimeToolTip", "Set the time of this notify directly"))
					[
						SNew(SBox)
						.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
						.WidthOverride(100.0f)
						[
							SNew(SNumericEntryBox<float>)
							.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
							.MinValue(0.0f)
							.MaxValue(AnimSequence->GetTotalDuration())
							.Value(NotifyNodes[NodeIndex]->GetTime())
							.AllowSpin(false)
							.OnValueCommitted_Lambda([this, NodeIndex](float InValue, ETextCommit::Type InCommitType)
							{
								if (InCommitType == ETextCommit::OnEnter && NotifyNodes.IsValidIndex(NodeIndex))
								{
									const FScopedTransaction Transaction(LOCTEXT("SetNotifyTimeTransaction", "Set Anim Notify trigger time"));
									AnimSequence->Modify();
									NotifyNodes[NodeIndex]->GetNotifyObject()->Modify();

									float NewTime = FMath::Clamp(InValue, 0.0f, AnimSequence->GetTotalDuration() - NotifyNodes[NodeIndex]->GetDuration());
									NotifyNodes[NodeIndex]->GetNotifyObject()->Time = NewTime;
									OnUpdatePanel.ExecuteIfBound();

									FSlateApplication::Get().DismissAllMenus();
								}
							})
						]
					];

				MenuBuilder.AddWidget(TimeWidget, LOCTEXT("TimeMenuText", "Notify Begin Time"));

				// Add item to directly set notify frame
				TSharedRef<SWidget> FrameWidget = 
					SNew( SBox )
					.HAlign( HAlign_Right )
					.ToolTipText(LOCTEXT("SetFrameToolTip", "Set the frame of this notify directly"))
					[
						SNew(SBox)
						.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
						.WidthOverride(100.0f)
						[
							SNew(SNumericEntryBox<int32>)
							.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
							.MinValue(0)
							.MaxValue(AnimSequence->GetNumberOfFrames())
							.Value(AnimSequence->GetFrameAtTime(NotifyNodes[NodeIndex]->GetTime()))
							.AllowSpin(false)						
							.OnValueCommitted_Lambda([this, NodeIndex](int32 InValue, ETextCommit::Type InCommitType)
							{
								if (InCommitType == ETextCommit::OnEnter && NotifyNodes.IsValidIndex(NodeIndex))
								{
									const FScopedTransaction Transaction(LOCTEXT("SetNotifyFrameTransaction", "Set Anim Notify trigger frame index"));
									AnimSequence->Modify();
									NotifyNodes[NodeIndex]->GetNotifyObject()->Modify();

									float NewTime = FMath::Clamp(AnimSequence->GetTimeAtFrame(InValue), 0.0f, AnimSequence->GetTotalDuration() - NotifyNodes[NodeIndex]->GetDuration());
									NotifyNodes[NodeIndex]->GetNotifyObject()->Time = NewTime;
									OnUpdatePanel.ExecuteIfBound();

									FSlateApplication::Get().DismissAllMenus();
								}
							})
						]
					];

				MenuBuilder.AddWidget(FrameWidget, LOCTEXT("FrameMenuText", "Notify Frame"));

				//@NOTE: Pending when skeletal sources exist
// 				// add menu to get threshold weight for triggering this notify
// 				TSharedRef<SWidget> ThresholdWeightWidget =
// 					SNew(SBox)
// 					.HAlign(HAlign_Right)
// 					.ToolTipText(LOCTEXT("MinTriggerWeightToolTip", "The minimum weight to trigger this notify"))
// 					[
// 						SNew(SBox)
// 						.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
// 					.WidthOverride(100.0f)
// 					[
// 						SNew(SNumericEntryBox<float>)
// 						.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
// 					.MinValue(0.0f)
// 					.MaxValue(1.0f)
// 					.Value(NotifyEvent->TriggerWeightThreshold)
// 					.AllowSpin(false)
// 					.OnValueCommitted_Lambda([this, NotifyIndex](float InValue, ETextCommit::Type InCommitType)
// 						{
// 							if (InCommitType == ETextCommit::OnEnter && AnimNotifies.IsValidIndex(NotifyIndex))
// 							{
// 								float NewWeight = FMath::Max(InValue, ZERO_ANIMWEIGHT_THRESH);
// 								AnimNotifies[NotifyIndex]->TriggerWeightThreshold = NewWeight;
// 
// 								FSlateApplication::Get().DismissAllMenus();
// 							}
// 						})
// 					]
// 					];
// 
// 				MenuBuilder.AddWidget(ThresholdWeightWidget, LOCTEXT("MinTriggerWeight", "Min Trigger Weight"));

				// Add menu for changing duration if this is a Notify State
				if (NotifyNodes[NodeIndex]->GetDuration() > 0.0f)
				{
					TSharedRef<SWidget> NotifyStateDurationWidget =
						SNew(SBox)
						.HAlign(HAlign_Right)
						.ToolTipText(LOCTEXT("SetAnimStateDuration_ToolTip", "The duration of this Anim Notify State"))
						[
							SNew(SBox)
							.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
							.WidthOverride(100.0f)
							[
								SNew(SNumericEntryBox<float>)
								.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
								.MinValue(UPaperZDAnimNotifyState::MinimumStateDuration)
								.MinSliderValue(UPaperZDAnimNotifyState::MinimumStateDuration)
								.MaxSliderValue(100.0f)
								.Value(NotifyNodes[NodeIndex]->GetDuration())
								.AllowSpin(false)
								.OnValueCommitted_Lambda([this, NodeIndex](float InValue, ETextCommit::Type InCommitType)
								{
									if (InCommitType == ETextCommit::OnEnter && NotifyNodes.IsValidIndex(NodeIndex))
									{
										const FScopedTransaction Transaction(LOCTEXT("SetNotifyDurationSecondsTransaction", "Set Anim Notify State duration in seconds"));
										AnimSequence->Modify();
										NotifyNodes[NodeIndex]->GetNotifyObject()->Modify();

										float NewDuration = FMath::Max(InValue, UPaperZDAnimNotifyState::MinimumStateDuration);
										float MaxDuration = AnimSequence->GetTotalDuration() - NotifyNodes[NodeIndex]->GetTime();
										NewDuration = FMath::Min(NewDuration, MaxDuration);
										NotifyNodes[NodeIndex]->SetDuration(NewDuration);

										// If we have a delegate bound to refresh the offsets, call it.
										// This is used by the montage editor to keep the offsets up to date.
										//OnRequestRefreshOffsets.ExecuteIfBound();

										FSlateApplication::Get().DismissAllMenus();
									}
								})
							]
						];

					MenuBuilder.AddWidget(NotifyStateDurationWidget, LOCTEXT("SetAnimStateDuration", "Anim Notify State Duration"));

					//Same as before, but in frames
					TSharedRef<SWidget> NotifyStateDurationFramesWidget =
						SNew(SBox)
						.HAlign(HAlign_Right)
						.ToolTipText(LOCTEXT("SetAnimStateDurationFrames_ToolTip", "The duration of this Anim Notify State in frames"))
						[
							SNew(SBox)
							.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
							.WidthOverride(100.0f)
							[
								SNew(SNumericEntryBox<int32>)
								.Font(FAppStyle::GetFontStyle(TEXT("MenuItem.Font")))
								.MinValue(1)
								.MinSliderValue(1)
								.MaxSliderValue(AnimSequence->GetNumberOfFrames())
								.Value(AnimSequence->GetFrameAtTime(NotifyNodes[NodeIndex]->GetDuration()))
								.AllowSpin(false)
								.OnValueCommitted_Lambda([this, NodeIndex](int32 InValue, ETextCommit::Type InCommitType)
								{
									if (InCommitType == ETextCommit::OnEnter && NotifyNodes.IsValidIndex(NodeIndex))
									{
										const FScopedTransaction Transaction(LOCTEXT("SetNotifyDurationFramesTransaction", "Set Anim Notify State duration in frames"));
										AnimSequence->Modify();
										NotifyNodes[NodeIndex]->GetNotifyObject()->Modify();

										float NewDuration = FMath::Max(AnimSequence->GetTimeAtFrame(InValue), UPaperZDAnimNotifyState::MinimumStateDuration);
										float MaxDuration = AnimSequence->GetTotalDuration() - NotifyNodes[NodeIndex]->GetTime();
										NewDuration = FMath::Min(NewDuration, MaxDuration);
										NotifyNodes[NodeIndex]->SetDuration(NewDuration);

										// If we have a delegate bound to refresh the offsets, call it.
										// This is used by the montage editor to keep the offsets up to date.
										//OnRequestRefreshOffsets.ExecuteIfBound();

										FSlateApplication::Get().DismissAllMenus();
									}
								})
							]
						];

					MenuBuilder.AddWidget(NotifyStateDurationFramesWidget, LOCTEXT("SetAnimStateDurationFrames", "Anim Notify State Frames"));
				}
			}
		}
		else
		{
			//The track itself was selected, we can show the context menu
			MenuBuilder.AddSubMenu(
				NSLOCTEXT("NewNotifySubMenu", "NewNotifySubMenuAddNotify", "Add Notify..."),
				NSLOCTEXT("NewNotifySubMenu", "NewNotifySubMenuAddNotifyToolTip", "Add AnimNotifyEvent"),
				FNewMenuDelegate::CreateRaw(this, &SPaperZDAnimNotifyTrack::FillNewNotifyMenu, false));

			MenuBuilder.AddSubMenu(
				NSLOCTEXT("NewNotifySubMenu", "NewNotifySubMenuAddNotifyState", "Add Notify State..."),
				NSLOCTEXT("NewNotifySubMenu", "NewNotifySubMenuAddNotifyStateToolTip", "Add AnimNotifyState"),
				FNewMenuDelegate::CreateRaw(this, &SPaperZDAnimNotifyTrack::FillNewNotifyStateMenu, false));

			MenuBuilder.AddMenuEntry(
				NSLOCTEXT("NewNotifySubMenu", "ManageNotifies", "Manage Notifies..."),
				NSLOCTEXT("NewNotifySubMenu", "ManageNotifiesToolTip", "Opens the Manage Notifies window"),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateSP(this, &SPaperZDAnimNotifyTrack::OnManageNotifies)));
		}
	}
	MenuBuilder.EndSection();

	//Copy & Paste
	MenuBuilder.BeginSection("AnimEdit", LOCTEXT("NotifyEditHeading", "Edit") );
	{
		if ( NodeIndex != INDEX_NONE )
		{
			// copy notify menu item
			MenuBuilder.AddMenuEntry(FPaperZDAnimNotifyPanelCommands::Get().CopyNotifies);

			// allow it to delete
			MenuBuilder.AddMenuEntry(FPaperZDAnimNotifyPanelCommands::Get().DeleteNotify);

			// For the "Replace With..." menu, make sure the current AnimNotify selection is valid for replacement
// 			if (OnGetIsAnimNotifySelectionValidforReplacement.IsBound() && OnGetIsAnimNotifySelectionValidforReplacement.Execute())
// 			{
// 				// If this is an AnimNotifyState (has duration) allow it to be replaced with other AnimNotifyStates
// 				if (NotifyEvent->NotifyStateClass)
// 				{
// 					MenuBuilder.AddSubMenu(
// 						NSLOCTEXT("NewNotifySubMenu", "NewNotifySubMenuReplaceWithNotifyState", "Replace with Notify State..."),
// 						NSLOCTEXT("NewNotifySubMenu", "NewNotifySubMenuReplaceWithNotifyStateToolTip", "Replace with AnimNotifyState"),
// 						FNewMenuDelegate::CreateRaw(this, &SAnimNotifyTrack::FillNewNotifyStateMenu, true));
// 				}
// 				// If this is a regular AnimNotify (no duration) allow it to be replaced with other AnimNotifies
// 				else
// 				{
// 					MenuBuilder.AddSubMenu(
// 						NSLOCTEXT("NewNotifySubMenu", "NewNotifySubMenuReplaceWithNotify", "Replace with Notify..."),
// 						NSLOCTEXT("NewNotifySubMenu", "NewNotifySubMenuReplaceWithNotifyToolTip", "Replace with AnimNotifyEvent"),
// 						FNewMenuDelegate::CreateRaw(this, &SAnimNotifyTrack::FillNewNotifyMenu, true));
// 				}
// 			}
		}
		else
		{
			FString PropertyString;
			const TCHAR* Buffer;
			float OriginalTime;
			float OriginalLength;
			int32 FirstTrackIndex;
			int32 TrackSpan;
			int32 Num;

			//Check whether can we show menu item to paste anim notify event
			if( ReadNotifyPasteHeader(PropertyString, Buffer, OriginalTime, OriginalLength, FirstTrackIndex, TrackSpan, Num) )
			{
				//Paste notify menu item
				if (Num == 1)
				{
					MenuBuilder.AddMenuEntry(FPaperZDAnimNotifyPanelCommands::Get().PasteNotifies);
				}
				else
				{
					NewAction.ExecuteAction.BindRaw(
						this, &SPaperZDAnimNotifyTrack::OnPasteNotifyClicked, EPaperZDNotifyPasteMode::MousePosition, EPaperZDNotifyPasteMultipleMode::Relative);

					MenuBuilder.AddMenuEntry(LOCTEXT("PasteMultRel", "Paste Multiple Relative"), LOCTEXT("PasteMultRelToolTip", "Paste multiple notifies beginning at the mouse cursor, maintaining the same relative spacing as the source."), FSlateIcon(), NewAction);
					MenuBuilder.AddMenuEntry(FPaperZDAnimNotifyPanelCommands::Get().PasteNotifies, NAME_None, LOCTEXT("PasteMultAbs", "Paste Multiple Absolute"), LOCTEXT("PasteMultAbsToolTip", "Paste multiple notifies beginning at the mouse cursor, maintaining absolute spacing."));
				}

				if(OriginalTime < AnimSequence->GetTotalDuration())
				{
					NewAction.ExecuteAction.BindRaw(
						this, &SPaperZDAnimNotifyTrack::OnPasteNotifyClicked, EPaperZDNotifyPasteMode::OriginalTime, EPaperZDNotifyPasteMultipleMode::Absolute);

					FText DisplayText = FText::Format( LOCTEXT("PasteAtOriginalTime", "Paste at original time ({0})"), FText::AsNumber( OriginalTime) );
					MenuBuilder.AddMenuEntry(DisplayText, LOCTEXT("PasteAtOriginalTimeToolTip", "Paste animation notify event at the time it was set to when it was copied"), FSlateIcon(), NewAction);
				}
				
			}
		}
	}
	MenuBuilder.EndSection(); //AnimEdit

	//Utils
	if (NodeIndex != INDEX_NONE && IsSingleNodeSelected())
	{
		//We can only use the notify under the mouse
		UPaperZDAnimNotify_Base* Notify = NotifyNodes[NodeIndex]->GetNotifyObject();

		MenuBuilder.BeginSection("ViewSource", LOCTEXT("NotifyViewHeading", "View"));
		if (UBlueprint* Blueprint = Cast<UBlueprint>(Notify->GetClass()->ClassGeneratedBy))
		{
			NewAction.ExecuteAction.BindRaw(
				this, &SPaperZDAnimNotifyTrack::OnOpenNotifySource, Blueprint);
				MenuBuilder.AddMenuEntry(LOCTEXT("OpenNotifyBlueprint", "Open Notify Blueprint"), LOCTEXT("OpenNotifyBlueprintTooltip", "Opens the source blueprint for this notify"), FSlateIcon(), NewAction);
		}
// 		else if (UPaperZDAnimNotifyCustom* CustomNotify = Cast<UPaperZDAnimNotifyCustom>(Notify))
// 		{
// 			NewAction.ExecuteAction.BindRaw(
// 				this, &SPaperZDAnimNotifyTrack::OnFilterSourceNotify, CustomNotify->Name);
// 				MenuBuilder.AddMenuEntry(LOCTEXT("FindNotifyReferences", "Find References"), LOCTEXT("FindNotifyReferencesTooltip", "Find all references to this custom notify in the asset browser"), FSlateIcon(), NewAction);
// 		}
		MenuBuilder.EndSection(); //ViewSource
	}	

	FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();

	// Display the newly built menu
	FSlateApplication::Get().PushMenu(SharedThis(this), WidgetPath, MenuBuilder.MakeWidget(), CursorPos, FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));
	return TSharedPtr<SWidget>();
}

void SPaperZDAnimNotifyTrack::SelectNodesByUniqueIDs(const TSet<uint32>& InUniqueIds, bool bUpdateSelectionSet)
{
	SelectedNodeIndices.Empty();

	for (int32 NodeIndex = 0; NodeIndex < NotifyNodes.Num(); NodeIndex++)
	{
		TSharedPtr<SPaperZDAnimNotifyNode> Node = NotifyNodes[NodeIndex];
		Node->bSelected = InUniqueIds.Contains(Node->GetNotifyObject()->GetUniqueID());
		if (Node->bSelected)
		{
			SelectedNodeIndices.Add(NodeIndex);
		}
	}

	if (bUpdateSelectionSet)
	{
		SendSelectionChanged();
	}
}

template<typename NotifyTypeClass>
void SPaperZDAnimNotifyTrack::MakeNewNotifyPicker(FMenuBuilder& MenuBuilder, bool bIsReplaceWithMenu /* = false */)
{
	FText TypeName = NotifyTypeClass::StaticClass() == UPaperZDAnimNotify::StaticClass() ? LOCTEXT("AnimNotifyName", "anim notify") : LOCTEXT("AnimNotifyStateName", "anim notify state");
	FText SectionHeaderFormat = bIsReplaceWithMenu ? LOCTEXT("ReplaceWithAnExistingAnimNotify", "Replace with an existing {0}") : LOCTEXT("AddsAnExistingAnimNotify", "Add an existing {0}");

	class FNotifyStateClassFilter : public IClassViewerFilter
	{
	public:
		FNotifyStateClassFilter(UPaperZDAnimSequence* InSequence)
			: Sequence(InSequence)
		{}

		bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			const bool bChildOfObjectClass = InClass->IsChildOf(NotifyTypeClass::StaticClass());
			const bool bMatchesFlags = !InClass->HasAnyClassFlags(CLASS_Hidden | CLASS_HideDropDown | CLASS_Deprecated | CLASS_Abstract);
			return bChildOfObjectClass && bMatchesFlags && CastChecked<NotifyTypeClass>(InClass->ClassDefaultObject)->CanBePlaced(Sequence);
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			const bool bChildOfObjectClass = InUnloadedClassData->IsChildOf(NotifyTypeClass::StaticClass());
			const bool bMatchesFlags = !InUnloadedClassData->HasAnyClassFlags(CLASS_Hidden | CLASS_HideDropDown | CLASS_Deprecated | CLASS_Abstract);
			bool bValidToPlace = false;
			if (bChildOfObjectClass)
			{
				if (const UClass* NativeBaseClass = InUnloadedClassData->GetNativeParent())
				{
					bValidToPlace = CastChecked<NotifyTypeClass>(NativeBaseClass->ClassDefaultObject)->CanBePlaced(Sequence);
				}
			}

			return bChildOfObjectClass && bMatchesFlags && bValidToPlace;
		}

		/** Sequence referenced by outer panel */
		UPaperZDAnimSequence* Sequence;
	};

	// MenuBuilder always has a search widget added to it by default, hence if larger then 1 then something else has been added to it
	if (MenuBuilder.GetMultiBox()->GetBlocks().Num() > 1)
	{
		MenuBuilder.AddMenuSeparator();
	}

	FClassViewerInitializationOptions InitOptions;
	InitOptions.Mode = EClassViewerMode::ClassPicker;
	InitOptions.bShowObjectRootClass = false;
	InitOptions.bShowUnloadedBlueprints = true;
	InitOptions.bShowNoneOption = false;
	InitOptions.bEnableClassDynamicLoading = true;
	InitOptions.bExpandRootNodes = true;
	InitOptions.NameTypeToDisplay = EClassViewerNameTypeToDisplay::DisplayName;
	InitOptions.ClassFilters.Add(MakeShared<FNotifyStateClassFilter>(AnimSequence));
	InitOptions.bShowBackgroundBorder = false;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	MenuBuilder.AddWidget(
		SNew(SBox)
		.MinDesiredWidth(300.0f)
		.MaxDesiredHeight(400.0f)
		[
			ClassViewerModule.CreateClassViewer(InitOptions,
			FOnClassPicked::CreateLambda([this, bIsReplaceWithMenu](UClass* InClass)
				{
					FSlateApplication::Get().DismissAllMenus();
					if (bIsReplaceWithMenu)
					{
						//ReplaceSelectedWithNotify(MakeBlueprintNotifyName(InClass->GetName()), InClass);
					}
					else
					{
						CreateNewNotifyAtCursor(MakeBlueprintNotifyName(InClass->GetName()), InClass);
					}
				}
			))
		],
		FText(), true, false);
}

void SPaperZDAnimNotifyTrack::FillNewNotifyMenu(FMenuBuilder& MenuBuilder, bool bIsReplaceWithMenu /* = false */)
{
	//Add source notifies
	UPaperZDAnimationSource* AnimSource = AnimSequence->GetAnimSource();
	if (AnimSource)
	{
		MenuBuilder.BeginSection("AnimNotifySourceSubMenu", LOCTEXT("NewNotifySubMenu_Source", "Source Notifies"));
		{
			if (!bIsReplaceWithMenu)
			{
				FUIAction UIAction;
				UIAction.ExecuteAction.BindSP(
					this, &SPaperZDAnimNotifyTrack::OnNewNotifyClicked);
				MenuBuilder.AddMenuEntry(LOCTEXT("NewNotify", "New Notify..."), LOCTEXT("NewNotifyToolTip", "Create a new animation notify on the owning source"), FSlateIcon(), UIAction);
			}

			MenuBuilder.AddSubMenu(
				LOCTEXT("NewNotifySubMenu_Source", "Source Notifies"),
				LOCTEXT("NewNotifySubMenu_Source_Tooltip", "Choose from custom notifies on the animation source"),
				FNewMenuDelegate::CreateLambda([this, AnimSource, bIsReplaceWithMenu](FMenuBuilder& InSubMenuBuilder)
					{
						InSubMenuBuilder.AddWidget(
							SNew(SBox)
							.MinDesiredWidth(300.0f)
							.MaxDesiredHeight(400.0f)
							[
								SNew(SPaperZDAnimationSourceNotifies, AnimSource)
								.IsPicker(true)
								.OnItemSelected_Lambda([this, bIsReplaceWithMenu](const FName& InNotifyName)
									{
										FSlateApplication::Get().DismissAllMenus();
										if (!bIsReplaceWithMenu)
										{
											CreateNewNotifyAtCursor(InNotifyName, UPaperZDAnimNotifyCustom::StaticClass());
										}
// 										else
// 										{
// 											ReplaceSelectedWithNotify(InNotifyName.ToString(), nullptr);
// 										}
									}
								)
							]
							, FText()
							, true
							, false
						);
					}));
		}
		MenuBuilder.EndSection();
	}

	MakeNewNotifyPicker<UPaperZDAnimNotify>(MenuBuilder, bIsReplaceWithMenu);
}

void SPaperZDAnimNotifyTrack::FillNewNotifyStateMenu(FMenuBuilder& MenuBuilder, bool bIsReplaceWithMenu /* = false */)
{
	MakeNewNotifyPicker<UPaperZDAnimNotifyState>(MenuBuilder, bIsReplaceWithMenu);
}

void SPaperZDAnimNotifyTrack::OnNewNotifyClicked()
{
	// Show dialog to enter new track name
	TSharedRef<STextEntryPopup> TextEntry =
		SNew(STextEntryPopup)
		.Label(LOCTEXT("NewNotifyLabel", "Notify Name"))
		.OnTextCommitted(this, &SPaperZDAnimNotifyTrack::AddNewNotify);

	// Show dialog to enter new event name
	FSlateApplication::Get().PushMenu(
		AsShared(), // Menu being summoned from a menu that is closing: Parent widget should be k2 not the menu thats open or it will be closed when the menu is dismissed
		FWidgetPath(),
		TextEntry,
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect(FPopupTransitionEffect::TypeInPopup)
	);
}

void SPaperZDAnimNotifyTrack::AddNewNotify(const FText& NewNotifyName, ETextCommit::Type CommitInfo)
{
	UPaperZDAnimationSource* AnimSource = AnimSequence->GetAnimSource();
	if (AnimSource && CommitInfo == ETextCommit::OnEnter)
	{
		const FScopedTransaction Transaction(LOCTEXT("AddNewNotifyEvent", "Add New Anim Notify"));
		FName NewName = FName(*NewNotifyName.ToString());

		AnimSource->Modify();
		AnimSource->RegisterCustomNotify(NewName);

		//In general the registration shouldn't ever fail, unless the given name was already taken, in which case we just continue with said notify
		CreateNewNotifyAtCursor(NewName, UPaperZDAnimNotifyCustom::StaticClass());
	}

	FSlateApplication::Get().DismissAllMenus();
}

bool SPaperZDAnimNotifyTrack::IsSingleNodeSelected()
{
	return SelectedNodeIndices.Num() == 1;
}

void SPaperZDAnimNotifyTrack::CreateNewNotifyAtCursor(FName NewNotifyName, UClass* NotifyClass)
{
	const FScopedTransaction Transaction(LOCTEXT("AddNotifyEvent", "Add Anim Notify"));
	AnimSequence->Modify();
	AnimSequence->AddNotifyToTrack(NotifyClass, TrackIndex, NewNotifyName, LastClickedTime);
	OnUpdatePanel.ExecuteIfBound();
}

FName SPaperZDAnimNotifyTrack::MakeBlueprintNotifyName(const FString& InNotifyClassName)
{
	FString DefaultNotifyName = InNotifyClassName;
	DefaultNotifyName = DefaultNotifyName.Replace(TEXT("AnimNotify_"), TEXT(""), ESearchCase::CaseSensitive);
	DefaultNotifyName = DefaultNotifyName.Replace(TEXT("AnimNotifyState_"), TEXT(""), ESearchCase::CaseSensitive);
	DefaultNotifyName = DefaultNotifyName.Replace(TEXT("PaperZD"), TEXT(""), ESearchCase::CaseSensitive);

	return *DefaultNotifyName;
}

void SPaperZDAnimNotifyTrack::OnManageNotifies()
{
	OnInvokeTab.ExecuteIfBound(PaperZDAnimationSourceTabs::SourceNotifiesTab);
}

void SPaperZDAnimNotifyTrack::OnOpenNotifySource(UBlueprint* InSourceBlueprint) const
{
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(InSourceBlueprint);
}

void SPaperZDAnimNotifyTrack::OnFilterSourceNotify(FName InName)
{
	// Open asset browser first
	OnInvokeTab.ExecuteIfBound(PaperZDAnimationSourceTabs::AssetBrowserTab);

	//@TODO
}

//////////////////////////////////////////////////////////////////////////
// FPaperZDAnimNotifyPanelCommands
FPaperZDAnimNotifyPanelCommands::FPaperZDAnimNotifyPanelCommands() 
	: TCommands<FPaperZDAnimNotifyPanelCommands>("AnimNotifyPanel", NSLOCTEXT("Contexts", "AnimNotifyPanel", "Anim Notify Panel"), NAME_None, FAppStyle::GetAppStyleSetName())
{}

void FPaperZDAnimNotifyPanelCommands::RegisterCommands()
{
	UI_COMMAND(DeleteNotify, "Delete", "Deletes the selected notifies.", EUserInterfaceActionType::Button, FInputChord(EKeys::Platform_Delete));
	UI_COMMAND(CopyNotifies, "Copy", "Copy animation notify events.", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control, EKeys::C));
	UI_COMMAND(PasteNotifies, "Paste", "Paste animation notify event here.", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control, EKeys::V));
}

//////////////////////////////////////////////////////////////////////////
// SPaperZDAnimNotifyPanel
void SPaperZDAnimNotifyPanel::Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimModel>& InAnimModel)
{
	WeakModel = InAnimModel;
	AnimSequence = InAnimModel->GetAnimAssetBase();
	bSkipNotifyTracksRefresh = false;
	check(AnimSequence);
	
	//Setup slate attributes
	CurrentPosition = InArgs._CurrentPosition;
	ViewInputMin = InArgs._ViewInputMin;
	ViewInputMax = InArgs._ViewInputMax;
	OnSetViewRange = InArgs._OnSetViewRange;
	OnSelectionChanged = InArgs._OnSelectionChanged;
	OnInvokeTab = InArgs._OnInvokeTab;
	OnNotifiesChanged = InArgs._OnNotifiesChanged;
	OnSnapPosition = InArgs._OnSnapPosition;
	OnRefreshSnapTimes = InArgs._OnRefreshSnapTimes;

	//Bind commands
	FPaperZDAnimNotifyPanelCommands::Register();
	BindCommands();

	//Bind UNDO to the AnimSequence, that way if the user does an UNDO/REDO that changes the notify count/position, we can act accordingly
	AnimSequence->OnPostEditUndo.BindSP(SharedThis(this), &SPaperZDAnimNotifyPanel::Update);

	//Bind the post edit change on notifies, in case a user changes its time values manually
	OnPropertyChangedHandleDelegateHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.Add(FOnPropertyChangedSignature::CreateSP(this, &SPaperZDAnimNotifyPanel::OnPropertyChanged));	

	AnimSequence->GetOnNotifyChange().AddSP(this, &SPaperZDAnimNotifyPanel::RefreshNotifyTracks);
	AnimSequence->GetAnimSource()->GetOnCustomNotifyChange().AddSP(this, &SPaperZDAnimNotifyPanel::RefreshNotifyTracks);
	InAnimModel->OnTracksChanged().Add(FSimpleDelegate::CreateSP(this, &SPaperZDAnimNotifyPanel::RefreshNotifyTracks));

	//Setup the ChildSlot before calling for an update, so all the inner widgets are already created
	ChildSlot
	[
		SAssignNew(PanelArea, SBorder)
		.Visibility(EVisibility::SelfHitTestInvisible)
		.AddMetaData<FTagMetaData>(TEXT("PaperZDAnimNotify.Notify"))
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.Padding(0.0f)
		.ColorAndOpacity(FLinearColor::White)
	];

	//Update the View
	Update();
}

SPaperZDAnimNotifyPanel::~SPaperZDAnimNotifyPanel()
{
	AnimSequence->OnPostEditUndo.Unbind();
	AnimSequence->GetOnNotifyChange().RemoveAll(this);
	FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(OnPropertyChangedHandleDelegateHandle);
	AnimSequence->GetAnimSource()->GetOnCustomNotifyChange().RemoveAll(this);
}

void SPaperZDAnimNotifyPanel::Update()
{
	//Make sure the tracks have valid data... its possible to have NULL notifies if the user force-deleted them
	AnimSequence->InitTracks();

	//Chance to update any snap times that are managed outside of the panel.
	OnRefreshSnapTimes.ExecuteIfBound();

	//Now re-build the tracks
	RefreshNotifyTracks();
}

// Helper to save/restore selection state when widgets are recreated
struct FScopedSavedNotifySelection
{
	FScopedSavedNotifySelection(SPaperZDAnimNotifyPanel& InPanel)
		: Panel(InPanel)
	{
		for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : InPanel.NotifyTracks)
		{
			for (TSharedPtr<SPaperZDAnimNotifyNode> Node : Track->GetNotifyNodes())
			{
				if (Node->IsValid() && Node->bSelected)
				{
					SelectedNodeIds.Add(Node->GetNotifyObject()->GetUniqueID());
				}
			}
		}
	}

	~FScopedSavedNotifySelection()
	{
		// Re-apply selection state
		for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : Panel.NotifyTracks)
		{
			Track->SelectNodesByUniqueIDs(SelectedNodeIds, false);
		}
	}

	SPaperZDAnimNotifyPanel& Panel;
	TSet<uint32> SelectedNodeIds;
};

void SPaperZDAnimNotifyPanel::RefreshNotifyTracks()
{
	//Ignore if we're just skipping refreshes
	if (bSkipNotifyTracksRefresh)
	{
		return;
	}

	check(AnimSequence);
	{
		FScopedSavedNotifySelection ScopedSelection(*this);

		TSharedPtr<SVerticalBox> NotifySlots;
		PanelArea->SetContent(
			SAssignNew(NotifySlots, SVerticalBox)
		);

		//Re create every track
		NotifyTracks.Empty();
		for (int32 i = 0; i < AnimSequence->GetNumTracks(); i++)
		{
			//Create an editor track
			TSharedPtr<SPaperZDAnimNotifyTrack> AnimNotifyTrack;
			NotifySlots->AddSlot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				[
					SAssignNew(AnimNotifyTrack, SPaperZDAnimNotifyTrack)
					.CommandList(WeakModel.Pin()->GetCommandList())
					.TrackIndex(i)
					.AnimSequence(AnimSequence)
	 				.ViewInputMin(ViewInputMin)
	 				.ViewInputMax(ViewInputMax)
					.OnGetDraggedNodePos(this, &SPaperZDAnimNotifyPanel::CalculateDraggedNodePos)
	 				.OnUpdatePanel(this, &SPaperZDAnimNotifyPanel::Update)
					.OnRequestTrackPan(FPanTrackRequest::CreateSP(this, &SPaperZDAnimNotifyPanel::PanInputViewRange))
					.OnSelectionChanged(this, &SPaperZDAnimNotifyPanel::OnTrackSelectionChanged)
					.OnNodesDragStarted(this, &SPaperZDAnimNotifyPanel::OnNotifyNodesDragStarted)
					.OnSnapPosition(OnSnapPosition)
					.OnDeleteSelectedNotifies(this, &SPaperZDAnimNotifyPanel::DeleteSelectedNodes)
					.OnDeselectAllNotifies(this, &SPaperZDAnimNotifyPanel::DeselectAllNotifies)
					.OnInvokeTab(OnInvokeTab)
					.OnPasteNodes(this, &SPaperZDAnimNotifyPanel::OnPasteNodes)
				];

			//Add the notify track that resides inside the editor track
			NotifyTracks.Add(AnimNotifyTrack);
		}
	}

	// Signal selection change to refresh details panel
	OnTrackSelectionChanged();
}

void SPaperZDAnimNotifyPanel::OnTrackSelectionChanged()
{
	OnSelectionChanged.ExecuteIfBound(GetAllSelectedNotifyObjects<UObject>());
}

void SPaperZDAnimNotifyPanel::AddTrack()
{
	//Create the track and set it up
	const int32 TrackIdx = AnimSequence->CreateTrack();
	FPaperZDAnimTrackMetadata& TrackData = AnimSequence->GetMutableTrackMetadataChecked(TrackIdx);
	TrackData.DisplayName = GetNewTrackName();
	TrackData.Color = FLinearColor::White;
}

FName SPaperZDAnimNotifyPanel::GetNewTrackName()
{
	TArray<FName> TrackNames;
	TrackNames.Reserve(50);

	for (int32 i = 0; i < AnimSequence->GetNumTracks(); i++)
	{
		TrackNames.Add(AnimSequence->GetTrackMetadataChecked(i).DisplayName);
	}

	//Do a name test to get the first available
	FName NameToTest;
	int32 TrackIndex = 1;
	do
	{
		NameToTest = *FString::FromInt(TrackIndex++);
	} while (TrackNames.Contains(NameToTest));

	return NameToTest;
}

void SPaperZDAnimNotifyPanel::OnPropertyChanged(UObject* ChangedObject, FPropertyChangedEvent& PropertyEvent)
{
	// Bail if it isn't a notify
	if (!ChangedObject->GetClass()->IsChildOf(UPaperZDAnimNotify_Base::StaticClass()))
	{
		return;
	}

	// Don't process if it's an interactive change; wait till we receive the final event.
	if (PropertyEvent.ChangeType != EPropertyChangeType::Interactive)
	{
		for (UPaperZDAnimNotify_Base* Notify : AnimSequence->GetAnimNotifies())
		{
			if (Notify == ChangedObject)
			{
				//We changed a notify present in the sequence
				Update();
			}
		}
	}
}

void SPaperZDAnimNotifyPanel::BindCommands()
{
	TSharedRef<FUICommandList> CommandList = WeakModel.Pin()->GetCommandList();
	const FPaperZDAnimNotifyPanelCommands& Commands = FPaperZDAnimNotifyPanelCommands::Get();

	CommandList->MapAction(
		Commands.DeleteNotify,
		FExecuteAction::CreateSP(this, &SPaperZDAnimNotifyPanel::OnDeletePressed));

	CommandList->MapAction(
		Commands.CopyNotifies,
		FExecuteAction::CreateSP(this, &SPaperZDAnimNotifyPanel::CopySelectedNodesToClipboard));

	CommandList->MapAction(
		Commands.PasteNotifies,
		FExecuteAction::CreateSP(this, &SPaperZDAnimNotifyPanel::OnPasteNodes, (SPaperZDAnimNotifyTrack*)nullptr, -1.0f, EPaperZDNotifyPasteMode::MousePosition, EPaperZDNotifyPasteMultipleMode::Absolute));
}

FReply SPaperZDAnimNotifyPanel::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	TSharedRef<FUICommandList> CommandList = WeakModel.Pin()->GetCommandList();
	if (CommandList->ProcessCommandBindings(InKeyEvent))
	{
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply SPaperZDAnimNotifyPanel::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool bLeftButton = MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);

	if (bLeftButton)
	{
		TArray<TSharedPtr<SPaperZDAnimNotifyNode>> SelectedNodes;
		for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
		{
			Track->AppendSelectedNodeWidgetsToArray(SelectedNodes);
		}

		Marquee.Start(MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()), Marquee.OperationTypeFromMouseEvent(MouseEvent), SelectedNodes);
		if (Marquee.Operation == FPaperZDNotifyMarqueeOperation::Replace)
		{
			// Remove and Add operations preserve selections, replace starts afresh
			DeselectAllNotifies();
		}

		return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
	}

	return FReply::Unhandled();
}

FReply SPaperZDAnimNotifyPanel::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (Marquee.bActive)
	{
		OnTrackSelectionChanged();
		Marquee = FPaperZDNotifyMarqueeOperation();
		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

FReply SPaperZDAnimNotifyPanel::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool bLeftButton = MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	if (bLeftButton && Marquee.bActive)
	{
		Marquee.Rect.UpdateEndPoint(MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition()));
		RefreshMarqueeSelectedNodes(MyGeometry);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SPaperZDAnimNotifyPanel::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Marquee.bActive = true;
	return FReply::Handled().CaptureMouse(SharedThis(this));
}

void SPaperZDAnimNotifyPanel::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (Marquee.bActive)
	{
		OnTrackSelectionChanged();
	}
	Marquee = FPaperZDNotifyMarqueeOperation();
}

int32 SPaperZDAnimNotifyPanel::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	//Needs to call parent, otherwise right click doesn't pass through the tracks, for some reason
	LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	FVector2D Origin = AllottedGeometry.AbsoluteToLocal(Marquee.Rect.GetUpperLeft());
	FVector2D Extents = AllottedGeometry.AbsoluteToLocal(Marquee.Rect.GetSize());

	if (Marquee.IsValid())
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId++,
			AllottedGeometry.ToPaintGeometry(Marquee.Rect.GetSize(), FSlateLayoutTransform(Marquee.Rect.GetUpperLeft())),
			FAppStyle::GetBrush(TEXT("MarqueeSelection"))
		);
	}

	return LayerId;
}

void SPaperZDAnimNotifyPanel::RefreshMarqueeSelectedNodes(const FGeometry& PanelGeo)
{
	if (Marquee.IsValid())
	{
		FSlateRect MarqueeRect = Marquee.Rect.ToSlateRect();
		for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
		{
			if (Marquee.Operation == FPaperZDNotifyMarqueeOperation::Replace || Marquee.OriginalSelection.Num() == 0)
			{
				Track->DeselectAllNotifyNodes(false);
			}

			const FGeometry& TrackGeo = Track->GetCachedGeometry();

			FSlateRect TrackClip = TrackGeo.GetLayoutBoundingRect();
			FSlateRect PanelClip = PanelGeo.GetLayoutBoundingRect();
			FVector2D PanelSpaceOrigin = TrackClip.GetTopLeft() - PanelClip.GetTopLeft();
			FVector2D TrackSpaceOrigin = MarqueeRect.GetTopLeft() - PanelSpaceOrigin;
			FSlateRect MarqueeTrackSpace(TrackSpaceOrigin, TrackSpaceOrigin + MarqueeRect.GetSize());

			Track->RefreshMarqueeSelectedNodes(MarqueeTrackSpace, Marquee);
		}
	}
}

void SPaperZDAnimNotifyPanel::DeselectAllNotifies()
{
	for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
	{
		Track->DeselectAllNotifyNodes();
	}
}

TArray<UPaperZDAnimNotify_Base*> SPaperZDAnimNotifyPanel::GetAllSelectedNotifyObjects()
{
	TArray<UPaperZDAnimNotify_Base*>SelectedNotifies;
	for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
	{
		SelectedNotifies.Append(Track->GetAllSelectedNotifyObjects());
	}

	return SelectedNotifies;
}

template <class T>
TArray<T*> SPaperZDAnimNotifyPanel::GetAllSelectedNotifyObjects()
{
	TArray<T*>SelectedNotifies;
	for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
	{
		SelectedNotifies.Append(Track->GetAllSelectedNotifyObjects<T>());
	}

	return SelectedNotifies;
}

void SPaperZDAnimNotifyPanel::OnDeletePressed()
{
	// If there's no focus on the panel it's likely the user is not editing notifies
	// so don't delete anything when the key is pressed.
	if (HasKeyboardFocus() || HasFocusedDescendants())
	{
		DeleteSelectedNodes();
	}
}

void SPaperZDAnimNotifyPanel::DeleteSelectedNodes()
{
	const FScopedTransaction Transaction(LOCTEXT("DeleteNotify", "Deleted Notifies"));
	AnimSequence->Modify();

	//When deleting multiple nodes, the "RefreshNotifyTracks" method gets called for each single node deleted (as we don't have multi notify deletion)
	//This breaks the loop, as the internal tracks that are being iterated are no longer valid, for this we deactivate notify track updates until the
	//nodes have been completely deleted.
	bSkipNotifyTracksRefresh = true;

	for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
	{
		for (UPaperZDAnimNotify_Base* Notify : Track->GetAllSelectedNotifyObjects())
		{
			AnimSequence->RemoveNotify(Notify);
		}
	}

	//Reactivate
	bSkipNotifyTracksRefresh = false;

	Update();
}

void SPaperZDAnimNotifyPanel::CopySelectedNodesToClipboard() const
{
	TArray<UPaperZDAnimNotify_Base*> SelectedNodes;
	for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
	{
		Track->AppendSelectionToArray(SelectedNodes);
	}

	const FString HeaderString(TEXT("ZD_COPY_ANIMNOTIFY"));

	if (SelectedNodes.Num() > 0)
	{
		FString StrValue(HeaderString);

		// Sort by track
		SelectedNodes.Sort([](const UPaperZDAnimNotify_Base& A, const UPaperZDAnimNotify_Base& B)
		{
			return (A.TrackIndex < B.TrackIndex) || (A.TrackIndex == B.TrackIndex && A.Time < B.Time);
		});

		// Need to find how many tracks this selection spans and the minimum time to use as the beginning of the selection
		int32 MinTrack = MAX_int32;
		int32 MaxTrack = MIN_int32;
		float MinTime = MAX_flt;
		for (const UPaperZDAnimNotify_Base* NodeObject : SelectedNodes)
		{
			MinTrack = FMath::Min(MinTrack, NodeObject->TrackIndex);
			MaxTrack = FMath::Max(MaxTrack, NodeObject->TrackIndex);
			MinTime = FMath::Min(MinTime, NodeObject->Time);
		}

		int32 TrackSpan = MaxTrack - MinTrack + 1;

		//Add metadata
		StrValue += FString::Printf(TEXT("OriginalTime=%f,"), MinTime);
		StrValue += FString::Printf(TEXT("OriginalLength=%f,"), AnimSequence->GetTotalDuration());
		StrValue += FString::Printf(TEXT("FirstTrackIndex=%d,"), MinTrack);
		StrValue += FString::Printf(TEXT("TrackSpan=%d,"), TrackSpan);
		StrValue += FString::Printf(TEXT("Num=%d"), SelectedNodes.Num());
		StrValue += "\n";

		//Begin a text export with the original objects
		FStringOutputDevice Archive;
		const FExportObjectInnerContext Context;
		for (UPaperZDAnimNotify_Base* NotifyToExport : SelectedNodes)
		{
			UExporter::ExportToOutputDevice(&Context, NotifyToExport, NULL, Archive, TEXT("copy"), 0, PPF_ExportsNotFullyQualified | PPF_Copy | PPF_Delimited, false, AnimSequence);
		}

		//Append to the Str value and send to the clipboard
		StrValue += Archive;
		FPlatformApplicationMisc::ClipboardCopy(*StrValue);
	}
}

void SPaperZDAnimNotifyPanel::OnPasteNodes(SPaperZDAnimNotifyTrack* RequestTrack, float ClickTime, EPaperZDNotifyPasteMode PasteMode, EPaperZDNotifyPasteMultipleMode MultiplePasteType)
{
	if (RequestTrack == nullptr)
	{
		for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
		{
			if (Track->HasKeyboardFocus())
			{
				RequestTrack = Track.Get();
				if (ClickTime == -1.0f)
				{
					ClickTime = RequestTrack->GetLastClickedTime();
				}
				break;
			}
		}
	}

	int32 PasteIdx = RequestTrack != nullptr ? RequestTrack->GetTrackIndex() : 0;
	int32 NumTracks = NotifyTracks.Num();
	FString PropString;
	const TCHAR* Buffer;
	float OrigBeginTime;
	float OrigLength;
	int32 TrackSpan;
	int32 Num;
	int32 FirstTrack = -1;
	float ScaleMultiplier = 1.0f;

	if (ReadNotifyPasteHeader(PropString, Buffer, OrigBeginTime, OrigLength, FirstTrack, TrackSpan, Num))
	{
		DeselectAllNotifies();

		FScopedTransaction Transaction(LOCTEXT("PasteNotifyEvent", "Paste Anim Notifies"));
		AnimSequence->Modify();

		if (ClickTime == -1.0f)
		{
			if (PasteMode == EPaperZDNotifyPasteMode::OriginalTime)
			{
				// We want to place the notifies exactly where they were
				ClickTime = OrigBeginTime;
			}
			else
			{
				ClickTime = WeakModel.Pin()->GetScrubTime();
			}
		}

		// Expand the number of tracks if we don't have enough.
		check(TrackSpan > 0);
		if (PasteIdx + TrackSpan > NumTracks)
		{
			int32 TracksToAdd = (PasteIdx + TrackSpan) - NumTracks;
			while (TracksToAdd)
			{
				AddTrack();
				--TracksToAdd;
			}
			NumTracks = NotifyTracks.Num();
		}

		// Scaling for relative paste
		if (MultiplePasteType == EPaperZDNotifyPasteMultipleMode::Relative)
		{
			ScaleMultiplier = AnimSequence->GetTotalDuration() / OrigLength;
		}

		//Import the text into our factory
		FTextAnimNotifyFactory NotifyFactory(AnimSequence, PasteIdx - FirstTrack, ClickTime - OrigBeginTime, ScaleMultiplier);
		NotifyFactory.ProcessBuffer(AnimSequence, RF_Transactional, Buffer);
		AnimSequence->ImportNotifies(NotifyFactory.GetAnimNotifies());
	}
}

float SPaperZDAnimNotifyPanel::CalculateDraggedNodePos() const
{
	return CurrentDragXPosition;
}

FReply SPaperZDAnimNotifyPanel::OnNotifyNodesDragStarted(TArray<TSharedPtr<SPaperZDAnimNotifyNode>> NotifyNodes, TSharedRef<SWidget> Decorator, const FVector2D& ScreenCursorPos, const FVector2D& ScreenNodePosition, const bool bDragOnMarker)
{
	TSharedRef<SOverlay> NodeDragDecoratorOverlay = SNew(SOverlay);
	TSharedRef<SBorder> NodeDragDecorator = SNew(SBorder)
	.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
	[
		NodeDragDecoratorOverlay
	];

	TArray<TSharedPtr<SPaperZDAnimNotifyNode>> Nodes;
	for (TSharedPtr<SPaperZDAnimNotifyTrack> Track : NotifyTracks)
	{
		Track->DisconnectSelectedNodesForDrag(Nodes);
	}

	FBox2D OverlayBounds(Nodes[0]->GetScreenPosition(), Nodes[0]->GetScreenPosition() + FVector2D(Nodes[0]->GetDurationSize(), 0.0f));
	for (int32 Idx = 1; Idx < Nodes.Num(); ++Idx)
	{
		TSharedPtr<SPaperZDAnimNotifyNode> Node = Nodes[Idx];
		FVector2D NodePosition = Node->GetScreenPosition();
		float NodeDuration = Node->GetDurationSize();

		OverlayBounds += FBox2D(NodePosition, NodePosition + FVector2D(NodeDuration, 0.0f));
	}

	FVector2D OverlayOrigin = OverlayBounds.Min;
	FVector2D OverlayExtents = OverlayBounds.GetSize();
	
	for (TSharedPtr<SPaperZDAnimNotifyNode> Node : Nodes)
	{
		FVector2D OffsetFromFirst(Node->GetScreenPosition() - OverlayOrigin);

		NodeDragDecoratorOverlay->AddSlot()
			.Padding(FMargin(OffsetFromFirst.X, OffsetFromFirst.Y, 0.0f, 0.0f))
			[
				Node->AsShared()
			];
	}

	FPanTrackRequest PanRequestDelegate = FPanTrackRequest::CreateSP(this, &SPaperZDAnimNotifyPanel::PanInputViewRange);
	FOnUpdatePanel UpdateDelegate = FOnUpdatePanel::CreateSP(this, &SPaperZDAnimNotifyPanel::Update);

	return FReply::Handled().BeginDragDrop(FNotifyDragDropOp::New(Nodes, NodeDragDecorator, NotifyTracks, AnimSequence, ScreenCursorPos, OverlayOrigin, OverlayExtents, CurrentDragXPosition, PanRequestDelegate, OnSnapPosition, UpdateDelegate));
}

void SPaperZDAnimNotifyPanel::PanInputViewRange(int32 ScreenDelta, FVector2D ScreenViewSize)
{
	FTrackScaleInfo ScaleInfo(ViewInputMin.Get(), ViewInputMax.Get(), 0.f, 0.f, ScreenViewSize);

	float InputDeltaX = ScaleInfo.PixelsPerInput > 0.0f ? ScreenDelta / ScaleInfo.PixelsPerInput : 0.0f;

	float NewViewInputMin = ViewInputMin.Get() + InputDeltaX;
	float NewViewInputMax = ViewInputMax.Get() + InputDeltaX;

	InputViewRangeChanged(NewViewInputMin, NewViewInputMax);
}

void SPaperZDAnimNotifyPanel::InputViewRangeChanged(float ViewMin, float ViewMax)
{
	OnSetViewRange.ExecuteIfBound(ViewMin, ViewMax);
}

#undef LOCTEXT_NAMESPACE
