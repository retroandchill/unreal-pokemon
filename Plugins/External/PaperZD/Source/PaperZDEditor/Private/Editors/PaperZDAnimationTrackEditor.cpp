// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/PaperZDAnimationTrackEditor.h"
#include "Sequencer/PaperZDMovieSceneAnimationTrack.h"
#include "Sequencer/PaperZDMovieSceneAnimationSection.h"
#include "Sequencer/IPaperZDSequencerSource.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"
#include "ISequencerTrackEditor.h"
#include "ISequencer.h"
#include "SequencerSectionPainter.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "PaperZDAnimBPGeneratedClass.h"
#include "PaperZDAnimInstance.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Layout/SBorder.h"
#include "TimeToPixel.h"


namespace PaperZDAnimationEditorConstants
{
	// @todo Sequencer Allow this to be customizable
	const uint32 AnimationTrackHeight = 20;
}

#define LOCTEXT_NAMESPACE "FPaperZDAnimationTrackEditor"

//////////////////////////////////////////////////////////////////////////
//// PaperZDAnimationSection
//////////////////////////////////////////////////////////////////////////

FPaperZDAnimationSection::FPaperZDAnimationSection(UMovieSceneSection& InSection, FPaperZDAnimationTrackEditor *InTrackEditor)
	: Section(*CastChecked<UPaperZDMovieSceneAnimationSection>(&InSection))
	, TrackEditor(InTrackEditor)
{
}

UMovieSceneSection* FPaperZDAnimationSection::GetSectionObject()
{
	return &Section;
}

FText FPaperZDAnimationSection::GetSectionTitle() const
{
	if (Section.Params.Animation->IsValidLowLevel())
	{
		return FText::FromName(Section.Params.Animation->GetSequenceName());
	}
	return LOCTEXT("InvalidAnimationSection", "Invalid Sequence");
}

float FPaperZDAnimationSection::GetSectionHeight() const
{
	return PaperZDAnimationEditorConstants::AnimationTrackHeight;
}

int32 FPaperZDAnimationSection::OnPaintSection(FSequencerSectionPainter& Painter) const
{
	const ESlateDrawEffect DrawEffects = Painter.bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	const FTimeToPixel& TimeToPixelConverter = Painter.GetTimeConverter();
	static const FSlateBrush* GenericDivider = FAppStyle::GetBrush("Sequencer.GenericDivider");

	int32 LayerId = Painter.PaintSectionBackground();

	// Add lines where the animation starts and ends/loops
	float AnimPlayRate = FMath::IsNearlyZero(Section.Params.PlayRate) ? 1.0f : Section.Params.PlayRate;
	float SeqLength = (Section.Params.GetSequenceLength() - (Section.Params.StartOffset + Section.Params.EndOffset)) / AnimPlayRate;

	FFrameRate TickResolution = TimeToPixelConverter.GetTickResolution();
	if (!FMath::IsNearlyZero(SeqLength, KINDA_SMALL_NUMBER) && SeqLength > 0)
	{
		float MaxOffset = Section.GetRange().Size<FFrameTime>() / TickResolution;
		float OffsetTime = SeqLength;
		float StartTime = Section.GetInclusiveStartFrame() / TickResolution;

		while (OffsetTime < MaxOffset)
		{
			float OffsetPixel = TimeToPixelConverter.SecondsToPixel(StartTime + OffsetTime) - TimeToPixelConverter.SecondsToPixel(StartTime);

			FSlateDrawElement::MakeBox(
				Painter.DrawElements,
				LayerId,
				Painter.SectionGeometry.MakeChild(
					FVector2D(2.f, Painter.SectionGeometry.Size.Y - 2.f),
					FSlateLayoutTransform(FVector2D(OffsetPixel, 1.f))
				).ToPaintGeometry(),
				GenericDivider,
				DrawEffects
			);

			OffsetTime += SeqLength;
		}
	}

	return LayerId;
}

void FPaperZDAnimationSection::BeginResizeSection()
{
	InitialStartOffsetDuringResize = Section.Params.StartOffset;
	InitialStartTimeDuringResize = Section.HasStartFrame() ? Section.GetInclusiveStartFrame() : 0;
}

void FPaperZDAnimationSection::ResizeSection(ESequencerSectionResizeMode ResizeMode, FFrameNumber ResizeTime)
{
	// Adjust the start offset when resizing from the beginning
	if (ResizeMode == SSRM_LeadingEdge)
	{
		FFrameRate FrameRate = Section.GetTypedOuter<UMovieScene>()->GetTickResolution();
		float StartOffset = (ResizeTime - InitialStartTimeDuringResize) * Section.Params.PlayRate / FrameRate;
		StartOffset += InitialStartOffsetDuringResize;

		// Ensure start offset is not less than 0 and adjust ResizeTime
		if (StartOffset < 0)
		{
			ResizeTime = ResizeTime - ((StartOffset / Section.Params.PlayRate) * FrameRate).RoundToFrame();

			StartOffset = 0.f;
		}

		Section.Params.StartOffset = StartOffset;
	}

	ISequencerSection::ResizeSection(ResizeMode, ResizeTime);
}

void FPaperZDAnimationSection::BuildSectionContextMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding)
{
	MenuBuilder.AddSubMenu(
		LOCTEXT("ReplaceAnimation", "Replace Animation"), NSLOCTEXT("ZDSequencer", "ReplaceAnimationTooltip", "Replace an Animation Sequence with another."),
		FNewMenuDelegate::CreateRaw(TrackEditor, &FPaperZDAnimationTrackEditor::AddAnimationSubMenu, ObjectBinding, &Section)
	);
}

//////////////////////////////////////////////////////////////////////////
//// PaperZDAnimationTrackEditor
//////////////////////////////////////////////////////////////////////////
FPaperZDAnimationTrackEditor::FPaperZDAnimationTrackEditor(TSharedRef<ISequencer> OwningSequencer) 
	: FMovieSceneTrackEditor(OwningSequencer)
{
}

TSharedRef<ISequencerTrackEditor> FPaperZDAnimationTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer)
{
	return MakeShareable(new FPaperZDAnimationTrackEditor(OwningSequencer));
}

bool FPaperZDAnimationTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return Type == UPaperZDMovieSceneAnimationTrack::StaticClass();
}


TSharedRef<ISequencerSection> FPaperZDAnimationTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	check(SupportsType(SectionObject.GetOuter()->GetClass()));
	return MakeShareable(new FPaperZDAnimationSection(SectionObject, this));
}

void FPaperZDAnimationTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass)
{
	if (ObjectClass->ImplementsInterface(UPaperZDSequencerSource::StaticClass()))
	{
		if (ObjectBindings.Num())
		{
			UMovieSceneTrack* Track = nullptr;
			MenuBuilder.AddSubMenu(
				LOCTEXT("AddAnimation", "Animation"), NSLOCTEXT("Sequencer", "AddAnimationTooltip", "Adds an animation track."),
				FNewMenuDelegate::CreateRaw(this, &FPaperZDAnimationTrackEditor::AddAnimationSubMenu, ObjectBindings[0], Track)
			);
		}
	}
}

void FPaperZDAnimationTrackEditor::AddAnimationSubMenu(FMenuBuilder& MenuBuilder, FGuid ObjectBinding, UMovieSceneTrack* Track)
{
	UObject* BoundObject = GetSequencer()->FindSpawnedObjectOrTemplate(ObjectBinding);
	IPaperZDSequencerSource* SequencerSource = Cast<IPaperZDSequencerSource>(BoundObject);

	//Request the creation of an animation list
	FOnAssetSelected OnAssetSelected = FOnAssetSelected::CreateSP(this, &FPaperZDAnimationTrackEditor::OnAnimationSequenceSelected, ObjectBinding, Track);
	TSharedRef<SWidget> AnimList = CreateAnimationListWidget(SequencerSource, OnAssetSelected);

	MenuBuilder.AddWidget(AnimList, FText::GetEmpty(), true);
}

void FPaperZDAnimationTrackEditor::AddAnimationSubMenu(FMenuBuilder& MenuBuilder, FGuid ObjectBinding, UPaperZDMovieSceneAnimationSection* Section)
{
	UObject* BoundObject = GetSequencer()->FindSpawnedObjectOrTemplate(ObjectBinding);
	IPaperZDSequencerSource* SequencerSource = Cast<IPaperZDSequencerSource>(BoundObject);

	//Request the creation of an animation list
	FOnAssetSelected OnAssetSelected = FOnAssetSelected::CreateSP(this, &FPaperZDAnimationTrackEditor::OnAnimationSequenceSelected, ObjectBinding, Section);
	TSharedRef<SWidget> AnimList = CreateAnimationListWidget(SequencerSource, OnAssetSelected);

	MenuBuilder.AddWidget(AnimList, FText::GetEmpty(), true);	
}

TSharedRef<SWidget> FPaperZDAnimationTrackEditor::CreateAnimationListWidget(IPaperZDSequencerSource* SequencerSource, FOnAssetSelected OnAssetSelected)
{
	//Obtain the AnimInstance class if one exists and obtain the animation source from it
	UPaperZDAnimBPGeneratedClass* AnimInstanceClass = Cast<UPaperZDAnimBPGeneratedClass>(SequencerSource->GetSequencerAnimInstanceClass());
	const UPaperZDAnimationSource* AnimationSource = AnimInstanceClass ? AnimInstanceClass->GetSupportedAnimationSource() : nullptr;

	//Shouldn't happen, but an animation source could not be setup (deleted animation source).
	TSharedPtr<SWidget> AnimationList;
	if (AnimationSource)
	{
		//Setup the asset picker
		FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

		//Setup filter
		FARFilter Filter;
		Filter.bRecursiveClasses = true;
		Filter.ClassPaths.Add(UPaperZDAnimSequence::StaticClass()->GetClassPathName());
		Filter.TagsAndValues.Add(UPaperZDAnimSequence::GetAnimSourceMemberName(), FAssetData(AnimationSource).GetExportTextName());

		//Setup config of the asset picker
		FAssetPickerConfig Config;
		Config.Filter = Filter;
		Config.InitialAssetViewType = EAssetViewType::Column;
		//Config.bAddFilterUI = true;
		Config.bShowPathInColumnView = true;
		Config.bSortByPathInColumnView = true;
		Config.HiddenColumnNames.Add("AnimSource");

		//Setup callbacks
		Config.OnAssetSelected = OnAssetSelected;				

		//Create the widget
		AnimationList = SNew(SBox)
						.WidthOverride(300.0f)
						.HeightOverride(300.0f)
						[
							SNew(SBorder)
							.Padding(FMargin(3))
							.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
							[
								ContentBrowserModule.Get().CreateAssetPicker(Config)
							]
						];
	}
	else
	{
		AnimationList = SNew(SBox)
			.WidthOverride(300.0f)
			.HeightOverride(300.f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Justification(ETextJustify::Center)
				.Text(LOCTEXT("NoAnimationSourceFound", "No valid animation source found.\nTry compiling the AnimationBlueprint associated with this object."))
			];
	}

	return AnimationList.ToSharedRef();
}

void FPaperZDAnimationTrackEditor::OnAnimationSequenceSelected(const FAssetData& AssetData, FGuid ObjectBinding, UMovieSceneTrack* Track)
{
	TSharedPtr<ISequencer> SequencerPtr = GetSequencer();
	UPaperZDAnimSequence* SelectedSequence = Cast<UPaperZDAnimSequence>(AssetData.GetAsset());	
	if (SelectedSequence && SequencerPtr.IsValid())
	{
		const FScopedTransaction Transaction(LOCTEXT("AddAnimation_Transaction", "Add Animation"));

		UObject* Object = SequencerPtr->FindSpawnedObjectOrTemplate(ObjectBinding);
		int32 RowIndex = INDEX_NONE;
		AnimatablePropertyChanged(FOnKeyProperty::CreateRaw(this, &FPaperZDAnimationTrackEditor::AddKeyInternal, Object, SelectedSequence, Track, RowIndex));
	}

	//Remove the menus
	FSlateApplication::Get().DismissAllMenus();
}

void FPaperZDAnimationTrackEditor::OnAnimationSequenceSelected(const FAssetData& AssetData, FGuid ObjectBinding, UPaperZDMovieSceneAnimationSection *Section)
{
	UPaperZDAnimSequence* SelectedSequence = Cast<UPaperZDAnimSequence>(AssetData.GetAsset());
	if (SelectedSequence)
	{
		Section->Params.Animation = SelectedSequence;
		Section->MarkAsChanged();
	}

	//Remove the menus
	FSlateApplication::Get().DismissAllMenus();
}

FKeyPropertyResult FPaperZDAnimationTrackEditor::AddKeyInternal(FFrameNumber KeyTime, UObject* Object, UPaperZDAnimSequence* AnimSequence, UMovieSceneTrack* Track, int32 RowIndex)
{
	FKeyPropertyResult KeyPropertyResult;

	FFindOrCreateHandleResult HandleResult = FindOrCreateHandleToObject(Object);
	FGuid ObjectHandle = HandleResult.Handle;
	KeyPropertyResult.bHandleCreated |= HandleResult.bWasCreated;
	if (ObjectHandle.IsValid())
	{
		if (!Track)
		{
			Track = AddTrack(GetSequencer()->GetFocusedMovieSceneSequence()->GetMovieScene(), ObjectHandle, UPaperZDMovieSceneAnimationTrack::StaticClass(), NAME_None);
			KeyPropertyResult.bTrackCreated = true;
		}

		if (ensure(Track))
		{
			Track->Modify();
			Cast<UPaperZDMovieSceneAnimationTrack>(Track)->AddNewAnimationOnRow(KeyTime, AnimSequence, RowIndex);
			KeyPropertyResult.bTrackModified = true;
		}
	}

	return KeyPropertyResult;
}

#undef LOCTEXT_NAMESPACE
