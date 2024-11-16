// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/PaperZDAnimModel.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack.h"
#include "Editors/Util/PaperZDEditorSettings.h"
#include "Editors/PaperZDAnimationSourceViewportClient.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"

#define LOCTEXT_NAMESPACE "FPaperZDAnimModel"

const FPaperZDAnimModel::FSnapType FPaperZDAnimModel::FSnapType::Frames("Frames", LOCTEXT("FramesSnapName", "Frames"), [](const FPaperZDAnimModel& InModel, double InTime)
	{
		// Round to nearest frame
		double FrameRate = InModel.GetFrameRate();
		if (FrameRate > 0)
		{
			return FMath::RoundToDouble(InTime * FrameRate) / FrameRate;
		}

		return InTime;
	});

const FPaperZDAnimModel::FSnapType FPaperZDAnimModel::FSnapType::Notifies("Notifies", LOCTEXT("NotifiesSnapName", "Notifies"));
//const FAnimModel::FSnapType FAnimModel::FSnapType::MontageSection("MontageSection", LOCTEXT("MontageSectionSnapName", "Montage Sections")); //@TODO: When montages are available

FPaperZDAnimModel::FPaperZDAnimModel(const TSharedRef<FUICommandList>& InCommandList, UPaperZDAnimationSource* InAnimSource, const TSharedPtr<FPaperZDAnimationSourceViewportClient>& InViewportClient)
	: WeakViewportClient(InViewportClient)
	, WeakAnimationSource(InAnimSource)
	, WeakCommandList(InCommandList)
	, bIsSelecting(false)
{}

float FPaperZDAnimModel::GetAnimAssetLength() const
{
	if (UPaperZDAnimSequence* AnimAsset = GetAnimAssetBase())
	{
		return AnimAsset->GetTotalDuration();
	}

	return 0.0f;
}

void FPaperZDAnimModel::SetEditableTime(int32 TimeIndex, double Time, bool bIsDragging)
{
	EditableTimes[TimeIndex] = FMath::Clamp(Time, 0.0, (double)GetAnimAssetLength());
	OnSetEditableTime(TimeIndex, EditableTimes[TimeIndex], bIsDragging);
}


bool FPaperZDAnimModel::Snap(float& InOutTime, float InSnapMargin, TArrayView<const FName> InSkippedSnapTypes) const
{
	double DoubleTime = (double)InOutTime;
	bool bResult = Snap(DoubleTime, (double)InSnapMargin, InSkippedSnapTypes);
	InOutTime = DoubleTime;
	return bResult;
}

bool FPaperZDAnimModel::Snap(double& InOutTime, double InSnapMargin, TArrayView<const FName> InSkippedSnapTypes) const
{
	InSnapMargin = FMath::Max(InSnapMargin, (double)KINDA_SMALL_NUMBER);

	double ClosestDelta = DBL_MAX;
	double ClosestSnapTime = DBL_MAX;

	// Check for enabled snap functions first
	for (const TPair<FName, FSnapType>& SnapTypePair : SnapTypes)
	{
		if (SnapTypePair.Value.SnapFunction != nullptr)
		{
			if (IsSnapChecked(SnapTypePair.Value.Type))
			{
				if (!InSkippedSnapTypes.Contains(SnapTypePair.Value.Type))
				{
					double SnappedTime = SnapTypePair.Value.SnapFunction(*this, InOutTime);
					if (SnappedTime != InOutTime)
					{
						double Delta = FMath::Abs(SnappedTime - InOutTime);
						if (Delta < InSnapMargin && Delta < ClosestDelta)
						{
							ClosestDelta = Delta;
							ClosestSnapTime = SnappedTime;
						}
					}
				}
			}
		}
	}

	// Find the closest in-range enabled snap time
	for (const FSnapTime& SnapTime : SnapTimes)
	{
		double Delta = FMath::Abs(SnapTime.Time - InOutTime);
		if (Delta < InSnapMargin && Delta < ClosestDelta)
		{
			if (!InSkippedSnapTypes.Contains(SnapTime.Type))
			{
				if (const FSnapType* SnapType = SnapTypes.Find(SnapTime.Type))
				{
					if (IsSnapChecked(SnapTime.Type))
					{
						ClosestDelta = Delta;
						ClosestSnapTime = SnapTime.Time;
					}
				}
			}
		}
	}

	if (ClosestDelta != DBL_MAX)
	{
		InOutTime = ClosestSnapTime;
		return true;
	}

	return false;
}

void FPaperZDAnimModel::ToggleSnap(FName InSnapName)
{
	if (IsSnapChecked(InSnapName))
	{
		GetMutableDefault<UPaperZDEditorSettings>()->TimelineEnabledSnaps.Remove(InSnapName);
	}
	else
	{
		GetMutableDefault<UPaperZDEditorSettings>()->TimelineEnabledSnaps.AddUnique(InSnapName);
	}
}

bool FPaperZDAnimModel::IsSnapChecked(FName InSnapName) const
{
	return GetDefault<UPaperZDEditorSettings>()->TimelineEnabledSnaps.Contains(InSnapName);
}

bool FPaperZDAnimModel::IsSnapAvailable(FName InSnapName) const
{
	return SnapTypes.Find(InSnapName) != nullptr;
}

FAnimatedRange FPaperZDAnimModel::GetViewRange() const
{
	return ViewRange;
}

FAnimatedRange FPaperZDAnimModel::GetWorkingRange() const
{
	return WorkingRange;
}

float FPaperZDAnimModel::GetFrameRate() const
{
	if (UPaperZDAnimSequence* AnimAsset = GetAnimAssetBase())
	{
		return AnimAsset->GetFramesPerSecond();
	}

	return 0.0f;
}

int32 FPaperZDAnimModel::GetTickResolution() const
{
	return FMath::RoundToInt((double)GetDefault<UPaperZDEditorSettings>()->TimelineScrubSnapValue * GetFrameRate());
}

TRange<FFrameNumber> FPaperZDAnimModel::GetPlaybackRange() const
{
	const int32 Resolution = GetTickResolution();
	return TRange<FFrameNumber>(FFrameNumber(FMath::RoundToInt32(PlaybackRange.GetLowerBoundValue() * (double)Resolution)), FFrameNumber(FMath::RoundToInt32(PlaybackRange.GetUpperBoundValue() * (double)Resolution)));
}

FFrameNumber FPaperZDAnimModel::GetScrubPosition() const
{
	//Obtain from the preview scene's viewport client
	if (WeakViewportClient.IsValid())
	{
		const float CurrentTime = WeakViewportClient.Pin()->GetPlaybackPosition();
		return FFrameNumber(FMath::RoundToInt32(CurrentTime * (double)GetTickResolution()));
	}

	return FFrameNumber(0);
}

float FPaperZDAnimModel::GetScrubTime() const
{
	//Obtain from the preview scene's viewport client
	if (WeakViewportClient.IsValid())
	{
		return WeakViewportClient.Pin()->GetPlaybackPosition();		
	}

	return 0.0f;
}

void FPaperZDAnimModel::SetScrubPosition(FFrameTime NewScrubPostion) const
{
	//Set the new position on the scene
	if (WeakViewportClient.IsValid())
	{
		WeakViewportClient.Pin()->SetPlaybackPosition(NewScrubPostion.AsDecimal() / (double) GetTickResolution());
	}
}

void FPaperZDAnimModel::HandleViewRangeChanged(TRange<double> InRange, EViewRangeInterpolation InInterpolation)
{
	SetViewRange(InRange);
}

void FPaperZDAnimModel::SetViewRange(TRange<double> InRange)
{
	ViewRange = InRange;

	if (WorkingRange.HasLowerBound() && WorkingRange.HasUpperBound())
	{
		WorkingRange = TRange<double>::Hull(WorkingRange, ViewRange);
	}
	else
	{
		WorkingRange = ViewRange;
	}
}

void FPaperZDAnimModel::HandleWorkingRangeChanged(TRange<double> InRange)
{
	WorkingRange = InRange;
}

bool FPaperZDAnimModel::IsTrackSelected(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack) const
{
	return SelectedTracks.Find(InTrack) != nullptr;
}

void FPaperZDAnimModel::ClearTrackSelection()
{
	SelectedTracks.Empty();
}

void FPaperZDAnimModel::SetTrackSelected(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack, bool bIsSelected)
{
	if (bIsSelected)
	{
		SelectedTracks.Add(InTrack);
	}
	else
	{
		SelectedTracks.Remove(InTrack);
	}
}

// void FAnimModel::AddReferencedObjects(FReferenceCollector& Collector)
// {
// 	EditorObjectTracker.AddReferencedObjects(Collector);
// }

void FPaperZDAnimModel::SelectObjects(const TArray<UObject*>& Objects)
{
	if (!bIsSelecting)
	{
		TGuardValue<bool> GuardValue(bIsSelecting, true);
		OnSelectObjects.ExecuteIfBound(Objects);

		OnHandleObjectsSelectedDelegate.Broadcast(Objects);
	}
}

// UObject* FAnimModel::ShowInDetailsView(UClass* EdClass)
// {
// 	UObject* Obj = EditorObjectTracker.GetEditorObjectForClass(EdClass);
// 	if (Obj != nullptr)
// 	{
// 		if (Obj->IsA(UEditorAnimBaseObj::StaticClass()))
// 		{
// 			if (!bIsSelecting)
// 			{
// 				TGuardValue<bool> GuardValue(bIsSelecting, true);
// 
// 				ClearTrackSelection();
// 
// 				UEditorAnimBaseObj* EdObj = Cast<UEditorAnimBaseObj>(Obj);
// 				InitDetailsViewEditorObject(EdObj);
// 
// 				TArray<UObject*> Objects;
// 				Objects.Add(EdObj);
// 				OnSelectObjects.ExecuteIfBound(Objects);
// 
// 				OnHandleObjectsSelectedDelegate.Broadcast(Objects);
// 			}
// 		}
// 	}
// 	return Obj;
// }

// void FAnimModel::ClearDetailsView()
// {
// 	if (!bIsSelecting)
// 	{
// 		TGuardValue<bool> GuardValue(bIsSelecting, true);
// 
// 		TArray<UObject*> Objects;
// 		OnSelectObjects.ExecuteIfBound(Objects);
// 		OnHandleObjectsSelectedDelegate.Broadcast(Objects);
// 	}
// }

// void FAnimModel::ToggleSnap(FName InSnapName)
// {
// 	if (IsSnapChecked(InSnapName))
// 	{
// 		GetMutableDefault<UPersonaOptions>()->TimelineEnabledSnaps.Remove(InSnapName);
// 	}
// 	else
// 	{
// 		GetMutableDefault<UPersonaOptions>()->TimelineEnabledSnaps.AddUnique(InSnapName);
// 	}
// }
// 
// bool FAnimModel::IsSnapChecked(FName InSnapName) const
// {
// 	return GetDefault<UPersonaOptions>()->TimelineEnabledSnaps.Contains(InSnapName);
// }
// 
// bool FAnimModel::IsSnapAvailable(FName InSnapName) const
// {
// 	return SnapTypes.Find(InSnapName) != nullptr;
// }

void FPaperZDAnimModel::BuildContextMenu(FMenuBuilder& InMenuBuilder)
{
	// Let each selected item contribute to the context menu
	TSet<FName> ExistingMenuTypes;
	for (const TSharedRef<FPaperZDAnimTimelineTrack>& SelectedItem : SelectedTracks)
	{
		SelectedItem->AddToContextMenu(InMenuBuilder, ExistingMenuTypes);
	}
}

#undef LOCTEXT_NAMESPACE
