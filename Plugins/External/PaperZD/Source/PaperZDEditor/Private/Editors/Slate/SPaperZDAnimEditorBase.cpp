// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDAnimEditorBase.h"
#include "Editors/AnimTimeline/SPaperZDAnimTimeline.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"

#define LOCTEXT_NAMESPACE "PaperZDEditor_AnimEditorBase"

void SPaperZDAnimEditorBase::Construct(const FArguments& InArgs, TSharedPtr<FPaperZDAnimationSourceEditor> InSourceEditor)
{
	SourceEditorPtr = InSourceEditor;
	OnObjectsSelected = InArgs._OnObjectsSelected;
	check(InArgs._AnimModel);
	
	SetInputViewRange(0, GetSequenceLength());

	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.FillHeight(1)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			[
				SAssignNew(NonScrollEditorPanels, SVerticalBox)
			]
			+SOverlay::Slot()
			[
				SNew(SPaperZDAnimTimeline, InArgs._AnimModel.ToSharedRef())
			]
		]
	];
}

// void SPaperZDAnimEditorBase::AddReferencedObjects(FReferenceCollector& Collector)
// {
// 	EditorObjectTracker.AddReferencedObjects(Collector);
// }
// 
// UObject* SPaperZDAnimEditorBase::ShowInDetailsView(UClass* EdClass)
// {
// 	check(GetEditorObject() != NULL);
// 
// 	UObject* Obj = EditorObjectTracker.GetEditorObjectForClass(EdClass);
// 
// 	if (Obj != NULL)
// 	{
// 		if (Obj->IsA(UEditorAnimBaseObj::StaticClass()))
// 		{
// 			UEditorAnimBaseObj* EdObj = Cast<UEditorAnimBaseObj>(Obj);
// 			InitDetailsViewEditorObject(EdObj);
// 
// 			TArray<UObject*> Objects;
// 			Objects.Add(EdObj);
// 			OnObjectsSelected.ExecuteIfBound(Objects);
// 		}
// 	}
// 	return Obj;
// }
// 
// void SPaperZDAnimEditorBase::ClearDetailsView()
// {
// 	TArray<UObject*> Objects;
// 	OnObjectsSelected.ExecuteIfBound(Objects);
// }
// 
// FText SPaperZDAnimEditorBase::GetEditorObjectName() const
// {
// 	if (GetEditorObject() != NULL)
// 	{
// 		return FText::FromString(GetEditorObject()->GetName());
// 	}
// 	else
// 	{
// 		return LOCTEXT("NoEditorObject", "No Editor Object");
// 	}
// }
// 
// void SPaperZDAnimEditorBase::RecalculateSequenceLength()
// {
// 	// Remove Gaps and update Montage Sequence Length
// 	if (UAnimCompositeBase* Composite = Cast<UAnimCompositeBase>(GetEditorObject()))
// 	{
// 		Composite->InvalidateRecursiveAsset();
// 
// 		float NewSequenceLength = CalculateSequenceLengthOfEditorObject();
// 		if (NewSequenceLength != GetSequenceLength())
// 		{
// 			ClampToEndTime(NewSequenceLength);
// 
// 			Composite->SetSequenceLength(NewSequenceLength);
// 
// 			// Reset view if we changed length (note: has to be done after ->SetSequenceLength)!
// 			SetInputViewRange(0.f, NewSequenceLength);
// 
// 			UAnimSingleNodeInstance* PreviewInstance = GetPreviewInstance();
// 			if (PreviewInstance)
// 			{
// 				// Re-set the position, so instance is clamped properly
// 				PreviewInstance->SetPosition(PreviewInstance->GetCurrentTime(), false);
// 			}
// 		}
// 	}
// 
// 	if (UAnimSequenceBase* Sequence = Cast<UAnimSequenceBase>(GetEditorObject()))
// 	{
// 		Sequence->ClampNotifiesAtEndOfSequence();
// 	}
// }
// 
// bool SPaperZDAnimEditorBase::ClampToEndTime(float NewEndTime)
// {
// 	float SequenceLength = GetSequenceLength();
// 
// 	//if we had a valid sequence length before and our new end time is shorter
// 	//then we need to clamp.
// 	return (SequenceLength > 0.f && NewEndTime < SequenceLength);
// }

void SPaperZDAnimEditorBase::OnSelectionChanged(const TArray<UObject*>& SelectedItems)
{
	OnObjectsSelected.ExecuteIfBound(SelectedItems);
}
// 
// class UAnimSingleNodeInstance* SPaperZDAnimEditorBase::GetPreviewInstance() const
// {
// 	return (GetPreviewScene()->GetPreviewMeshComponent()) ? GetPreviewScene()->GetPreviewMeshComponent()->PreviewInstance : nullptr;
// }
// 
// float SPaperZDAnimEditorBase::GetScrubValue() const
// {
// 	UAnimSingleNodeInstance* PreviewInstance = GetPreviewInstance();
// 	if (PreviewInstance)
// 	{
// 		float CurTime = PreviewInstance->GetCurrentTime();
// 		return (CurTime);
// 	}
// 	else
// 	{
// 		return 0.f;
// 	}
// }

void SPaperZDAnimEditorBase::SetInputViewRange(float InViewMinInput, float InViewMaxInput)
{
	ViewMaxInput = FMath::Min<float>(InViewMaxInput, GetSequenceLength());
	ViewMinInput = FMath::Max<float>(InViewMinInput, 0.f);
}
// 
// FText SPaperZDAnimEditorBase::GetCurrentSequenceTime() const
// {
// 	UAnimSingleNodeInstance* PreviewInstance = GetPreviewInstance();
// 	float CurTime = 0.f;
// 	float TotalTime = GetSequenceLength();
// 
// 	if (PreviewInstance)
// 	{
// 		CurTime = PreviewInstance->GetCurrentTime();
// 	}
// 
// 	static const FNumberFormattingOptions FractionNumberFormat = FNumberFormattingOptions()
// 		.SetMinimumFractionalDigits(3)
// 		.SetMaximumFractionalDigits(3);
// 	return FText::Format(LOCTEXT("FractionSecondsFmt", "{0} / {1} (second(s))"), FText::AsNumber(CurTime, &FractionNumberFormat), FText::AsNumber(TotalTime, &FractionNumberFormat));
// }
// 
// float SPaperZDAnimEditorBase::GetPercentageInternal() const
// {
// 	UAnimSingleNodeInstance* PreviewInstance = GetPreviewInstance();
// 	float Percentage = 0.f;
// 	if (PreviewInstance)
// 	{
// 		float SequenceLength = GetSequenceLength();
// 		if (SequenceLength > 0.f)
// 		{
// 			Percentage = PreviewInstance->GetCurrentTime() / SequenceLength;
// 		}
// 		else
// 		{
// 			Percentage = 0.f;
// 		}
// 	}
// 
// 	return Percentage;
// }
// 
// FText SPaperZDAnimEditorBase::GetCurrentPercentage() const
// {
// 	float Percentage = GetPercentageInternal();
// 
// 	static const FNumberFormattingOptions PercentNumberFormat = FNumberFormattingOptions()
// 		.SetMinimumFractionalDigits(2)
// 		.SetMaximumFractionalDigits(2);
// 	return FText::AsPercent(Percentage, &PercentNumberFormat);
// }
// 
// FText SPaperZDAnimEditorBase::GetCurrentFrame() const
// {
// 	float Percentage = GetPercentageInternal();
// 	float LastFrame = 0;
// 
// 	if (UAnimSequenceBase* AnimSeqBase = Cast<UAnimSequenceBase>(GetEditorObject()))
// 	{
// 		LastFrame = FMath::Max(AnimSeqBase->GetNumberOfFrames() - 1, 0);
// 	}
// 
// 	static const FNumberFormattingOptions FractionNumberFormat = FNumberFormattingOptions()
// 		.SetMinimumFractionalDigits(2)
// 		.SetMaximumFractionalDigits(2);
// 	return FText::Format(LOCTEXT("FractionKeysFmt", "{0} / {1} Frame"), FText::AsNumber(LastFrame * Percentage, &FractionNumberFormat), FText::AsNumber((int32)LastFrame));
// }
// 
float SPaperZDAnimEditorBase::GetSequenceLength() const
{
	UPaperZDAnimSequence* EditedObject = GetEditorObject();
	if (EditedObject)
	{
		return EditedObject->GetTotalDuration();
	}

	return 0.0f;
}

#undef LOCTEXT_NAMESPACE
