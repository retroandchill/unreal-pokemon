// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Editors/Util/PaperZDEditorDelegates.h"
#include "Framework/Commands/Commands.h"
#include "Framework/MarqueeRect.h"
#include "AssetRegistry/AssetData.h"

enum class EPaperZDNotifyPasteMode : uint8
{
	MousePosition,
	OriginalTime
};

enum class EPaperZDNotifyPasteMultipleMode : uint8
{
	Relative,
	Absolute
};


class SBorder;
class SScrollBar;
class FUICommandList;
class SPaperZDAnimNotifyTrack;
class SPaperZDAnimNotifyNode;
class UPaperZDAnimNotify_Base;
class FPaperZDAnimModel;
class UPaperZDAnimSequence;

//delegate declaration
DECLARE_DELEGATE_OneParam(FOnSelectionChanged, const TArray<UObject*>&)
DECLARE_DELEGATE(FOnTrackSelectionChanged)
DECLARE_DELEGATE_TwoParams(FOnSetViewRange, float, float)
DECLARE_DELEGATE_RetVal_ThreeParams(bool, FOnSnapPosition, float& /*InOutTimeToSnap*/, float /*InSnapMargin*/, TArrayView<const FName> /*InSkippedSnapTypes*/)
DECLARE_DELEGATE(FOnRefreshSnapTimes)

/* Marquee operation used for notify multi-selection. */
struct FPaperZDNotifyMarqueeOperation
{
	FPaperZDNotifyMarqueeOperation()
		: Operation(Add)
		, bActive(false)
	{}

	enum Type
	{
		/** Holding down Ctrl removes nodes */
		Remove,
		/** Holding down Shift adds to the selection */
		Add,
		/** When nothing is pressed, marquee replaces selection */
		Replace
	} Operation;

	bool IsValid() const
	{
		return Rect.IsValid() && bActive;
	}

	void Start(const FVector2D& InStartLocation, FPaperZDNotifyMarqueeOperation::Type InOperationType, TArray<TSharedPtr<SPaperZDAnimNotifyNode>>& InOriginalSelection)
	{
		Rect = FMarqueeRect(InStartLocation);
		Operation = InOperationType;
		OriginalSelection = InOriginalSelection;
	}

	void End()
	{
		Rect = FMarqueeRect();
	}


	/** Given a mouse event, figure out what the marquee selection should do based on the state of Shift and Ctrl keys */
	static FPaperZDNotifyMarqueeOperation::Type OperationTypeFromMouseEvent(const FPointerEvent& MouseEvent)
	{
		if (MouseEvent.IsControlDown())
		{
			return FPaperZDNotifyMarqueeOperation::Remove;
		}
		else if (MouseEvent.IsShiftDown())
		{
			return FPaperZDNotifyMarqueeOperation::Add;
		}
		else
		{
			return FPaperZDNotifyMarqueeOperation::Replace;
		}
	}

public:
	/** The marquee rectangle being dragged by the user */
	FMarqueeRect Rect;

	/** Whether the marquee has been activated, usually by a drag */
	bool bActive;

	/** The original selection state before the marquee selection */
	TArray<TSharedPtr<SPaperZDAnimNotifyNode>> OriginalSelection;
};

/**
 * Commands to be used by the notify panel.
 */
class FPaperZDAnimNotifyPanelCommands : public TCommands<FPaperZDAnimNotifyPanelCommands>
{
public:
	FPaperZDAnimNotifyPanelCommands();	

	TSharedPtr<FUICommandInfo> DeleteNotify;
	TSharedPtr<FUICommandInfo> CopyNotifies;
	TSharedPtr<FUICommandInfo> PasteNotifies;

	virtual void RegisterCommands() override;
};

/**
 * Legacy class that displays the notify panel, used now in conjunction with the anim tracks to be displayed on top of them.
 */
class SPaperZDAnimNotifyPanel: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPaperZDAnimNotifyPanel)
		: _CurrentPosition()
		, _ViewInputMin()
		, _ViewInputMax()
		, _OnSetViewRange()
		, _OnSelectionChanged()
		, _OnRefreshSnapTimes()
	{}
	SLATE_ATTRIBUTE(float, CurrentPosition)
	SLATE_ATTRIBUTE(float, ViewInputMin)
	SLATE_ATTRIBUTE(float, ViewInputMax)
	SLATE_EVENT(FOnSetViewRange, OnSetViewRange)
	SLATE_EVENT(FOnSelectionChanged, OnSelectionChanged)
	SLATE_EVENT(FPaperZDEditorDelegates::OnInvokeTabSignature, OnInvokeTab )
	SLATE_EVENT(FSimpleDelegate, OnNotifiesChanged )
	SLATE_EVENT(FOnSnapPosition, OnSnapPosition )
	SLATE_EVENT(FOnRefreshSnapTimes, OnRefreshSnapTimes)
	SLATE_END_ARGS()

	/* Constructs this widget. */
	void Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimModel>& InAnimModel);
	~SPaperZDAnimNotifyPanel();

	/* Binds the command list to their actions. */
	void BindCommands();

	/* Called when the notifies have been modified and thus, the panel needs to update them. */
	void RefreshNotifyTracks();

	/* Called to do a full update/refresh of the panel. */
	void Update();

	/** Returns the position of the notify node currently being dragged. Returns -1 if no node is being dragged */
	float CalculateDraggedNodePos() const;
	
	/* Called by the editor when a property on any object changes. */
	typedef FCoreUObjectDelegates::FOnObjectPropertyChanged::FDelegate FOnPropertyChangedSignature;
	void OnPropertyChanged(UObject* ChangedObject, FPropertyChangedEvent& PropertyEvent);

	/* Pans the input view when doing a drag and drop operation. */
	void PanInputViewRange(int32 ScreenDelta, FVector2D ScreenViewSize);

	/* Called when the panel forces the view range to change. */
	void InputViewRangeChanged(float ViewMin, float ViewMax);

	//~Begin SWidget Interface
	FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
 	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
 	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
 	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
 	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
  	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
 	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	//~End SWidget Interface

	/* Updates the nodes that are currently being selected by the marquee rect. */
	void RefreshMarqueeSelectedNodes(const FGeometry& PanelGeo);

	/* Handler for when a notify node drag has been initiated/ */
	FReply OnNotifyNodesDragStarted(TArray<TSharedPtr<SPaperZDAnimNotifyNode>> NotifyNodes, TSharedRef<SWidget> Decorator, const FVector2D& ScreenCursorPos, const FVector2D& ScreenNodePosition, const bool bDragOnMarker);

	/* Called when the node selection changes, to notify any observer and update the node's tint color. */
	void OnTrackSelectionChanged();

	/* Add a track on the sequence, usually due to a paste of notifies that require an expansion of the track span. */
	void AddTrack();

	/* Obtain a valid name for a new track. */
	FName GetNewTrackName();

	// Called to deselect all notifies across all tracks
	void DeselectAllNotifies();
	
	//Deletion related
	void OnDeletePressed();
	void DeleteSelectedNodes();

	//Copy/Paste related
	void CopySelectedNodesToClipboard() const;
	void OnPasteNodes(SPaperZDAnimNotifyTrack* RequestTrack, float ClickTime, EPaperZDNotifyPasteMode PasteMode, EPaperZDNotifyPasteMultipleMode MultiplePasteType);
	
	//For getting selections
	TArray<UPaperZDAnimNotify_Base*> GetAllSelectedNotifyObjects();

	template<class T>
	TArray<T*> GetAllSelectedNotifyObjects();

	/** We support keyboard focus to detect when we should process key commands like delete */
	virtual bool SupportsKeyboardFocus() const override
	{
		return true;
	}

private:
	friend struct FScopedSavedNotifySelection;

	/* Anim Model linked to the notify panel. */
	TWeakPtr<FPaperZDAnimModel> WeakModel;

	/* Pointer to the AnimSequence identified by the AnimModel. */
	TObjectPtr<UPaperZDAnimSequence> AnimSequence;

	//Slate arguments
	TAttribute<float>											CurrentPosition;
	TAttribute<float>											ViewInputMin;
	TAttribute<float>											ViewInputMax;
	float														WidgetWidth;
	FOnSetViewRange												OnSetViewRange;
	FOnSelectionChanged											OnSelectionChanged;
	FSimpleDelegate												OnNotifiesChanged;
	FOnSnapPosition												OnSnapPosition;
	FPaperZDEditorDelegates::OnInvokeTabSignature				OnInvokeTab;
	FOnRefreshSnapTimes											OnRefreshSnapTimes;

	/** Manager for mouse controlled marquee selection */
	FPaperZDNotifyMarqueeOperation Marquee;

	/** Store the position of a currently dragged node for display across tracks */
	float CurrentDragXPosition;
	
	/* Handle bound to the OnPropertyChange delegate. */
	FDelegateHandle OnPropertyChangedHandleDelegateHandle;

	/* The panel area holding the widgets to be constructed. */
	TSharedPtr<SBorder> PanelArea;

	/* Notify tracks being rendered. */
	TArray<TSharedPtr<class SPaperZDAnimNotifyTrack>> NotifyTracks;

	/* Flag used to ignore track refresh calls issued when the notifies change on the sequence being edited. */
	bool bSkipNotifyTracksRefresh;
};
