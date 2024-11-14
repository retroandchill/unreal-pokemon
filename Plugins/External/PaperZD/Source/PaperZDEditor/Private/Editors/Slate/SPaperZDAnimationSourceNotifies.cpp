// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDAnimationSourceNotifies.h"
#include "Editors/Slate/SPaperZDConfirmDialog.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Notifies/PaperZDAnimNotifyCustom.h"
#include "PaperZDAnimBP.h"

#include "Subsystems/AssetEditorSubsystem.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Widgets/Input/SSearchBox.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/ARFilter.h"

#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "ScopedTransaction.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "PaperZD_AnimationSourceNotifies"

void SPaperZDAnimationSourceNotifies::Construct(const FArguments& InArgs, UPaperZDAnimationSource* InAnimSource, const TSharedPtr<FAssetEditorToolkit>& InHostingApp /* = nullptr */)
{
	OnItemSelected = InArgs._OnItemSelected;
	bIsPicker = InArgs._IsPicker;

	AnimationSource = InAnimSource;
	WeakHostingApp = InHostingApp;
	check(AnimationSource);

	//Need to be able to update ourselves if the notifies change by some reason
	AnimationSource->GetOnCustomNotifyChange().AddSP(this, &SPaperZDAnimationSourceNotifies::OnNotifiesChanged);

	if (GEditor)
	{
		GEditor->RegisterForUndo(this);
	}

	FOnContextMenuOpening OnContextMenuOpening = !bIsPicker ? FOnContextMenuOpening::CreateSP(this, &SPaperZDAnimationSourceNotifies::OnGetContextMenuContent) : FOnContextMenuOpening();
	
	ChildSlot
	[
		SNew( SVerticalBox )

		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding( FMargin( 0.0f, 0.0f, 0.0f, 4.0f ) )
		[
			SAssignNew( NameFilterBox, SSearchBox )
			.SelectAllTextWhenFocused( true )
			.OnTextChanged( this, &SPaperZDAnimationSourceNotifies::OnFilterTextChanged )
			.OnTextCommitted( this, &SPaperZDAnimationSourceNotifies::OnFilterTextCommitted )
			.HintText( LOCTEXT( "NotifiesSearchBoxHint", "Search Animation Notifies...") )
		]

		+ SVerticalBox::Slot()
		.FillHeight( 1.0f )		// This is required to make the scrollbar work, as content overflows Slate containers by default
		[
			SAssignNew( NotifiesListView, SPaperZDAnimNotifyListType)
			.ListItemsSource( &NotifyList )
			.OnGenerateRow( this, &SPaperZDAnimationSourceNotifies::GenerateNotifyRow )
			.OnContextMenuOpening( OnContextMenuOpening )
			.OnSelectionChanged( this, &SPaperZDAnimationSourceNotifies::OnNotifySelectionChanged )
			.ItemHeight( 22.0f )
			.OnItemScrolledIntoView( this, &SPaperZDAnimationSourceNotifies::OnItemScrolledIntoView )
		]
	];

	CreateNotifiesList();
}

SPaperZDAnimationSourceNotifies::~SPaperZDAnimationSourceNotifies()
{
	AnimationSource->GetOnCustomNotifyChange().RemoveAll(this);

	if (GEditor)
	{
		GEditor->UnregisterForUndo(this);
	}
}

void SPaperZDAnimationSourceNotifies::PostUndo(bool bSuccess)
{
	RefreshNotifiesListWithFilter();
}

void SPaperZDAnimationSourceNotifies::PostRedo(bool bSuccess)
{
	RefreshNotifiesListWithFilter();
}

void SPaperZDAnimationSourceNotifies::CreateNotifiesList(const FString& SearchText /* = FString("") */)
{
	NotifyList.Empty();

	for (const FName& NotifyName : AnimationSource->GetRegisteredNotifyNames())
	{
		if (!SearchText.IsEmpty())
		{
			if (NotifyName.ToString().Contains(SearchText))
			{
				NotifyList.Add(FPaperZDAnimNotifyDisplayInfo::Make(NotifyName));
			}
		}
		else
		{
			NotifyList.Add(FPaperZDAnimNotifyDisplayInfo::Make(NotifyName));
		}
	}

	NotifiesListView->RequestListRefresh();
}

void SPaperZDAnimationSourceNotifies::NotifyUser(FNotificationInfo& NotificationInfo)
{
	TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(NotificationInfo);
	if (Notification.IsValid())
	{
		Notification->SetCompletionState(SNotificationItem::CS_Fail);
	}
}

bool SPaperZDAnimationSourceNotifies::OnVerifyNotifyNameCommit(const FText& NewName, FText& OutErrorMessage, TSharedPtr<FPaperZDAnimNotifyDisplayInfo> Item)
{
	bool bValid(true);

	if (NewName.IsEmpty())
	{
		OutErrorMessage = LOCTEXT("NameMissing_Error", "You must provide a name.");
		bValid = false;
	}

	FName NotifyName(*NewName.ToString());
	if (NotifyName != Item->Name || Item->bIsNew)
	{
		if (AnimationSource->GetRegisteredNotifyNames().Contains(NotifyName))
		{
			OutErrorMessage = FText::Format(LOCTEXT("AlreadyInUseMessage", "'{0}' is already in use."), NewName);
			bValid = false;
		}
	}

	return bValid;
}

bool SPaperZDAnimationSourceNotifies::CanPerformDelete() const
{
	return NotifiesListView->GetNumItemsSelected() > 0;
}

void SPaperZDAnimationSourceNotifies::OnDeleteAnimNotify()
{
	TArray<TSharedPtr<FPaperZDAnimNotifyDisplayInfo>> SelectedRows = NotifiesListView->GetSelectedItems();

	// this one deletes all notifies with same name. 
	TArray<FName> SelectedNotifyNames;

	for(int Selection = 0; Selection < SelectedRows.Num(); ++Selection)
	{
		SelectedNotifyNames.Add(SelectedRows[Selection]->Name);
	}

	//Delete the notifies
	const int32 NumAnimationsModified = DeleteExistingNotifies(SelectedNotifyNames);
	if(NumAnimationsModified > 0)
	{
		// Tell the user that the socket is a duplicate
		FFormatNamedArguments Args;
		Args.Add( TEXT("NumAnimationsModified"), NumAnimationsModified );
		FNotificationInfo Info( FText::Format( LOCTEXT( "AnimNotifiesDeleted", "{NumAnimationsModified} animation(s) modified to delete notifications" ), Args ) );

		Info.bUseLargeFont = false;
		Info.ExpireDuration = 5.0f;

		NotifyUser( Info );
	}

	CreateNotifiesList( NameFilterBox->GetText().ToString() );
}

int32 SPaperZDAnimationSourceNotifies::DeleteExistingNotifies(TArray<FName> NotifyNames)
{
	//Name can be used, begin a transaction
	const FScopedTransaction Transaction(LOCTEXT("DeleteAnimNotifies", "Deleted Anim Notifies"));
	AnimationSource->Modify();

	//Now go through every sequence and remove any instance of the notifies we just deleted
	int32 NumModifiedSequences = 0;
	TArray<FAssetData> AnimSequenceData;
	GetCompatibleAnimSequences(AnimSequenceData);
	for (const FAssetData& AssetData : AnimSequenceData)
	{
		//Load the asset
		UPaperZDAnimSequence* LoadedSequence = Cast<UPaperZDAnimSequence>(AssetData.GetAsset());
		bool bModifiedSequence = false;
		for (int32 i = LoadedSequence->GetAnimNotifies().Num() - 1; i >= 0; i--)
		{
			UPaperZDAnimNotifyCustom* CustomNotify = Cast<UPaperZDAnimNotifyCustom>(LoadedSequence->GetAnimNotifies()[i]);
			if (CustomNotify && NotifyNames.Contains(CustomNotify->Name))
			{
				//Check if this sequence was already modified, as we're removing a notify on it
				if (!bModifiedSequence)
				{
					LoadedSequence->Modify();
					NumModifiedSequences++;
					bModifiedSequence = true;
				}

				//Remove the notify
				LoadedSequence->RemoveNotify(CustomNotify);
			}
		}
	}

	//Finally iterate on every name and start removing the notifies
	for (FName NameToDelete : NotifyNames)
	{
		AnimationSource->UnregisterCustomNotify(NameToDelete);
	}

	return NumModifiedSequences;
}

bool SPaperZDAnimationSourceNotifies::CanPerformRename() const
{
	return NotifiesListView->GetNumItemsSelected() == 1;
}

void SPaperZDAnimationSourceNotifies::OnRenameAnimNotify()
{
	TArray<TSharedPtr<FPaperZDAnimNotifyDisplayInfo>> SelectedRows = NotifiesListView->GetSelectedItems();

	check(SelectedRows.Num() == 1); // Should be guaranteed by CanPerformRename

	SelectedRows[0]->InlineEditableText->EnterEditingMode();
}

int32 SPaperZDAnimationSourceNotifies::RenameExistingNotify(FName NewNotifyName, FName ExistingNotify)
{
	//First of all, check that the new notify name is not being used currently
	if (AnimationSource->GetRegisteredNotifyNames().Contains(NewNotifyName))
	{
		// Tell the user that the socket is a duplicate
		FNotificationInfo Info(LOCTEXT("AnimNotify_DuplicateName", "Cannot rename, notify name already in use."));
		Info.bUseLargeFont = false;
		Info.ExpireDuration = 5.0f;

		NotifyUser(Info);
		return 0;
	}
	else
	{
		//First iterate over every AnimBP and see if there're any of them that require loading for the rename
		//Loading Animation blueprints can be costly, so we want to ask the user before actually renaming anything
		TArray<FAssetData> AnimBPData;
		GetCompatibleAnimBPs(AnimBPData);
		{
			int32 NumRequiredToLoad = 0;
			for (const FAssetData& Data : AnimBPData)
			{
				if (!Data.IsAssetLoaded())
				{
					NumRequiredToLoad++;
				}
			}

			if (NumRequiredToLoad > 0)
			{
				FFormatNamedArguments Args;
				Args.Add(TEXT("NumRequiredToLoad"), NumRequiredToLoad);

				TSharedRef<SPaperZDConfirmDialog> Dialog = SNew(SPaperZDConfirmDialog)
					.TitleText(FText::FromString(TEXT("Loading assets required")))
					.DetailText(FText::Format(LOCTEXT("RequiredLoadAsset", "Renaming this notify requires loading {NumRequiredToLoad} animation blueprints, continue?"), NumRequiredToLoad))
					.ShowCancelButton(true);

				if (!Dialog->Show())
				{
					return -1;
				}
			}
		}

		//Start the rename operation
		const FScopedTransaction Transaction(LOCTEXT("RenameAnimNotify", "Rename Anim Notify"));

		//Start by actually renaming the AnimBPs first, if any of the AnimBP loads right now, they will start the compilation process.
		//If we rename the source notify first, the compilation will create a new function graph for it and possibly destroy the old one, rendering the rename useless
		for (const FAssetData& Data : AnimBPData)
		{
			UPaperZDAnimBP* AnimBP = Cast<UPaperZDAnimBP>(Data.GetAsset());
			AnimBP->OnRenameCustomNotify(ExistingNotify, NewNotifyName);
		}

		//Now rename the animation source.
		AnimationSource->Modify();
		AnimationSource->RenameCustomNotify(ExistingNotify, NewNotifyName);
		
		//Finally we need to check every anim sequence that actually uses this notify
		int32 NumModifiedSequences = 0;
		TArray<FAssetData> AnimSequenceData;
		GetCompatibleAnimSequences(AnimSequenceData);
		for (const FAssetData& AssetData : AnimSequenceData)
		{
			//Load the asset
			UPaperZDAnimSequence* LoadedSequence = Cast<UPaperZDAnimSequence>(AssetData.GetAsset());
			bool bModifiedSequence = false;
			for (UPaperZDAnimNotify_Base* NotifyBase : LoadedSequence->GetAnimNotifies())
			{
				UPaperZDAnimNotifyCustom* CustomNotify = Cast<UPaperZDAnimNotifyCustom>(NotifyBase);
				if (CustomNotify && CustomNotify->Name == ExistingNotify)
				{
					//Found a notify to change
					CustomNotify->Modify();
					CustomNotify->Name = NewNotifyName;

					//Increase the number of the count if this is the first time we modify the animation
					if (!bModifiedSequence)
					{
						NumModifiedSequences++;
						bModifiedSequence = true;
					}
				}
			}
		}

		return NumModifiedSequences;
	}

}

void SPaperZDAnimationSourceNotifies::OnNotifyNameCommitted(const FText& NewName, ETextCommit::Type, TSharedPtr<FPaperZDAnimNotifyDisplayInfo> Item)
{
	FName NotifyName = FName(*NewName.ToString());
	if (Item->bIsNew)
	{
		//Start the rename operation
		const FScopedTransaction Transaction(LOCTEXT("NewAnimNotify", "New Source Anim Notify"));
		AnimationSource->Modify();
		AnimationSource->RegisterCustomNotify(NotifyName);
		Item->bIsNew = false;
	}
	else if(NotifyName != Item->Name)
	{
		const int32 NumAnimationsModified = RenameExistingNotify(NotifyName, Item->Name);

		if (NumAnimationsModified > 0)
		{
			// Notify the user about the amount of animations used
			FFormatNamedArguments Args;
			Args.Add(TEXT("NumAnimationsModified"), NumAnimationsModified);
			FNotificationInfo Info(FText::Format(LOCTEXT("AnimNotifiesRenamed", "{NumAnimationsModified} animation(s) modified to rename notification"), Args));
			Info.bUseLargeFont = false;
			Info.ExpireDuration = 5.0f;

			NotifyUser(Info);
		}

		RefreshNotifiesListWithFilter();
	}
}

void SPaperZDAnimationSourceNotifies::RefreshNotifiesListWithFilter()
{
	CreateNotifiesList(NameFilterBox->GetText().ToString());
}

void SPaperZDAnimationSourceNotifies::OnNotifiesChanged()
{
	RefreshNotifiesListWithFilter();
}

void SPaperZDAnimationSourceNotifies::OnFilterTextChanged( const FText& SearchText )
{
	FilterText = SearchText;

	RefreshNotifiesListWithFilter();
}

void SPaperZDAnimationSourceNotifies::OnFilterTextCommitted( const FText& SearchText, ETextCommit::Type CommitInfo )
{
	// Just do the same as if the user typed in the box
	OnFilterTextChanged( SearchText );
}

TSharedRef<ITableRow> SPaperZDAnimationSourceNotifies::GenerateNotifyRow(TSharedPtr<FPaperZDAnimNotifyDisplayInfo> InInfo, const TSharedRef<STableViewBase>& OwnerTable)
{
	check( InInfo.IsValid() );

	return
		SNew( STableRow<TSharedPtr<FPaperZDAnimNotifyDisplayInfo>>, OwnerTable )
		[
			SNew( SVerticalBox )
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding( 0.0f, 4.0f )
			.VAlign( VAlign_Center )
			[
				SAssignNew(InInfo->InlineEditableText, SInlineEditableTextBlock)
				.Text(FText::FromName(InInfo->Name))
				.OnVerifyTextChanged(this, &SPaperZDAnimationSourceNotifies::OnVerifyNotifyNameCommit, InInfo)
				.OnTextCommitted(this, &SPaperZDAnimationSourceNotifies::OnNotifyNameCommitted, InInfo)
				.IsSelected(this, &SPaperZDAnimationSourceNotifies::IsSelected)
				.HighlightText_Lambda([this](){ return FilterText; })
				.IsReadOnly(bIsPicker)
			]
		];
}

TSharedPtr<SWidget> SPaperZDAnimationSourceNotifies::OnGetContextMenuContent() const
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder( bShouldCloseWindowAfterMenuSelection, NULL);

	MenuBuilder.BeginSection("AnimNotifyAction", LOCTEXT("AnimNotifyActions", "Notifies"));
	{
		FUIAction Action = FUIAction(FExecuteAction::CreateSP(const_cast<SPaperZDAnimationSourceNotifies*>(this), &SPaperZDAnimationSourceNotifies::OnAddAnimNotify));
		const FText Label = LOCTEXT("NewAnimNotifyButtonLabel", "New...");
		const FText ToolTipText = LOCTEXT("NewAnimNotifyButtonTooltip", "Creates a new anim notify.");
		MenuBuilder.AddMenuEntry(Label, ToolTipText, FSlateIcon(), Action);
	}
	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("AnimNotifyAction", LOCTEXT("SelectedAnimNotifyActions", "Selected Notify Actions"));
	{
		{
			FUIAction Action = FUIAction(FExecuteAction::CreateSP(const_cast<SPaperZDAnimationSourceNotifies*>(this), &SPaperZDAnimationSourceNotifies::OnRenameAnimNotify),
				FCanExecuteAction::CreateSP(this, &SPaperZDAnimationSourceNotifies::CanPerformRename));
			const FText Label = LOCTEXT("RenameAnimNotifyButtonLabel", "Rename");
			const FText ToolTipText = LOCTEXT("RenameAnimNotifyButtonTooltip", "Renames the selected anim notifies.");
			MenuBuilder.AddMenuEntry(Label, ToolTipText, FSlateIcon(), Action);
		}

		{
			FUIAction Action = FUIAction(FExecuteAction::CreateSP(const_cast<SPaperZDAnimationSourceNotifies*>(this), &SPaperZDAnimationSourceNotifies::OnDeleteAnimNotify),
				FCanExecuteAction::CreateSP(this, &SPaperZDAnimationSourceNotifies::CanPerformDelete));
			const FText Label = LOCTEXT("DeleteAnimNotifyButtonLabel", "Delete");
			const FText ToolTipText = LOCTEXT("DeleteAnimNotifyButtonTooltip", "Deletes the selected anim notifies.");
			MenuBuilder.AddMenuEntry(Label, ToolTipText, FSlateIcon(), Action);
		}
// 
// 		if (WeakHostingApp.IsValid() && NotifiesListView->GetNumItemsSelected() == 1)
// 		{
// 			FUIAction Action = FUIAction(FExecuteAction::CreateSP(const_cast<SPaperZDAnimationSourceNotifies*>(this), &SPaperZDAnimationSourceNotifies::OnFindReferences),
// 				FCanExecuteAction::CreateSP(this, &SPaperZDAnimationSourceNotifies::CanPerformDelete));
// 			const FText Label = LOCTEXT("FindNotifyReferences", "Find References");
// 			const FText ToolTipText = LOCTEXT("FindNotifyReferencesTooltip", "Find all references to this skeleton notify in the asset browser");
// 			MenuBuilder.AddMenuEntry(Label, ToolTipText, FSlateIcon(), Action);
// 		}
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void SPaperZDAnimationSourceNotifies::OnNotifySelectionChanged(TSharedPtr<FPaperZDAnimNotifyDisplayInfo> Selection, ESelectInfo::Type SelectInfo)
{
	if (Selection.IsValid())
	{
		OnItemSelected.ExecuteIfBound(Selection->Name);
	}
}

void SPaperZDAnimationSourceNotifies::OnAddAnimNotify()
{
	// Find a unique name for this notify
	const TCHAR* BaseNotifyString = TEXT("NewNotify");
	FString NewNotifyString = BaseNotifyString;
	int32 NumericExtension = 0;

	while (AnimationSource->GetRegisteredNotifyNames().ContainsByPredicate([&NewNotifyString](const FName& InNotifyName) { return InNotifyName.ToString() == NewNotifyString; }))
	{
		NewNotifyString = FString::Printf(TEXT("%s_%d"), BaseNotifyString, NumericExtension);
		NumericExtension++;
	}

	// Add an item. The subsequent rename will commit the item.
	TSharedPtr<FPaperZDAnimNotifyDisplayInfo> NewItem = FPaperZDAnimNotifyDisplayInfo::Make(*NewNotifyString);
	NewItem->bIsNew = true;
	NotifyList.Add(NewItem);

	NotifiesListView->ClearSelection();
	NotifiesListView->RequestListRefresh();
	NotifiesListView->RequestScrollIntoView(NewItem);
}

void SPaperZDAnimationSourceNotifies::OnItemScrolledIntoView(TSharedPtr<FPaperZDAnimNotifyDisplayInfo> InItem, const TSharedPtr<ITableRow>& InTableRow)
{
	if (InItem.IsValid() && InItem->InlineEditableText.IsValid() && InItem->bIsNew)
	{
		InItem->InlineEditableText->EnterEditingMode();
	}
}

void SPaperZDAnimationSourceNotifies::GetCompatibleAnimSequences(TArray<struct FAssetData>& OutAssets) const
{	
	//Setup filter
	FARFilter Filter;
	Filter.bRecursiveClasses = true;
	Filter.ClassPaths.Add(UPaperZDAnimSequence::StaticClass()->GetClassPathName());
	Filter.TagsAndValues.Add(UPaperZDAnimSequence::GetAnimSourceMemberName(), FAssetData(AnimationSource).GetExportTextName());

  	// Load the asset registry module
  	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().GetAssets(Filter, OutAssets);
}

void SPaperZDAnimationSourceNotifies::GetCompatibleAnimBPs(TArray<struct FAssetData>& OutAssets) const
{
	//Setup filter
	FARFilter Filter;
	Filter.bRecursiveClasses = true;
	Filter.ClassPaths.Add(UPaperZDAnimBP::StaticClass()->GetClassPathName());
	Filter.TagsAndValues.Add(UPaperZDAnimBP::GetAnimationSourceMemberName(), FAssetData(AnimationSource).GetExportTextName());

	// Load the asset registry module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().GetAssets(Filter, OutAssets);
}

#undef LOCTEXT_NAMESPACE
