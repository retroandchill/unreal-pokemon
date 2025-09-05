// Fill out your copyright notice in the Description page of Project Settings.


#include "Repositories/GameDataRepositoryEditor.h"

#include "DesktopPlatformModule.h"
#include "EditorDirectories.h"
#include "GameDataAccessToolsEditor.h"
#include "GameDataRepository.h"
#include "IStructureDetailsView.h"
#include "Repositories/GameDataRepositoryEntrySelector.h"
#include "Serialization/GameDataEntrySerializer.h"


class IDesktopPlatform;

void FGameDataRepositoryEditor::Initialize(const EToolkitMode::Type Mode,
                                           const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                           UGameDataRepository* Asset)
{
    PropertyAccessor = MakeUnique<FGameDataRepositoryDataAccessor>(Asset);
    const auto Layout = FTabManager::NewLayout("GameDataRepositoryEditor_Layout")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
                                         ->Split
                                         (
                                             FTabManager::NewStack()
                                             ->SetSizeCoefficient(0.3f)
                                             ->AddTab("EntrySelectionTab", ETabState::OpenedTab)
                                         )
                                         ->Split
                                         (
                                             FTabManager::NewStack()
                                             ->SetSizeCoefficient(0.7f)
                                             ->AddTab("EntryEditTab", ETabState::OpenedTab)
                                         )
        );

    Serializers = IGameDataAccessToolsEditorModule::Get().GetAvailableSerializers(Asset->GetClass());

    const auto ToolbarExtender = MakeShared<FExtender>();
    ToolbarExtender->AddToolBarExtension(
        "Asset",
        EExtensionHook::After,
        GetToolkitCommands(),
        FToolBarExtensionDelegate::CreateRaw(this, &FGameDataRepositoryEditor::FillToolbar)
    );
    AddToolbarExtender(ToolbarExtender);

    InitAssetEditor(Mode, InitToolkitHost, "GameDataRepositoryEditor", Layout, true, true, Asset);
}

void FGameDataRepositoryEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(
        NSLOCTEXT("GameDataRepository", "GameDataRepository", "Game Data Asset"));

    InTabManager->RegisterTabSpawner("EntrySelectionTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
                {
                    return SNew(SDockTab)
                        [
                            SAssignNew(EntrySelector, SGameDataRepositoryEntrySelector)
                            .OnEntrySelected(this, &FGameDataRepositoryEditor::OnEntrySelected)
                            .OnGetEntries(this, &FGameDataRepositoryEditor::OnGetEntries)
                        ];
                }))
                .SetDisplayName(NSLOCTEXT("GameDataRepository", "EntrySelectionTab", "Entries"))
                .SetGroup(WorkspaceMenuCategory.ToSharedRef());

    FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(
        "PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    FStructureDetailsViewArgs DetailsViewArgsStruct;

    auto CurrentEntry = PropertyAccessor->GetFirstEntry();
    DetailsView = PropertyEditorModule.CreateStructureDetailView(DetailsViewArgs, DetailsViewArgsStruct, CurrentEntry);
    DetailsView->GetOnFinishedChangingPropertiesDelegate().AddRaw(this, &FGameDataRepositoryEditor::OnPropertyChanged);
    InTabManager->RegisterTabSpawner("EntryEditTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs&)
                {
                    return SNew(SDockTab)
                        [
                            DetailsView->GetWidget().ToSharedRef()
                        ];
                }))
                .SetDisplayName(NSLOCTEXT("GameDataRepository", "Details", "Details"))
                .SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FGameDataRepositoryEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
    InTabManager->UnregisterTabSpawner("EntrySelectionTab");
    InTabManager->UnregisterTabSpawner("EntryEditTab");
}

FName FGameDataRepositoryEditor::GetToolkitFName() const
{
    return FName("GameDataRepositoryEditor");
}

FText FGameDataRepositoryEditor::GetBaseToolkitName() const
{
    return NSLOCTEXT("GameDataRepositoryEditor", "AppLabel", "Game Data Asset Editor");
}

FString FGameDataRepositoryEditor::GetWorldCentricTabPrefix() const
{
    return "GameDataRepositoryEditor";
}

FLinearColor FGameDataRepositoryEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor();
}

void FGameDataRepositoryEditor::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
    // Add import section
    ToolbarBuilder.BeginSection("Import");
    {
        // Add import button that will be populated with dynamic actions
        ToolbarBuilder.AddComboButton(
            FUIAction(),
            FOnGetContent::CreateSP(this, &FGameDataRepositoryEditor::ImportMenuEntries),
            NSLOCTEXT("GameDataRepository", "Import", "Import"),
            NSLOCTEXT("GameDataRepository", "ImportTooltip", "Import data from various sources"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Reimport")
        );
    }
    ToolbarBuilder.EndSection();

    // Add entry management section
    ToolbarBuilder.BeginSection("EntryManagement");
    {
        ToolbarBuilder.AddToolBarButton(
            FUIAction(
                FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::OnAddEntry),
                FCanExecuteAction::CreateRaw(this, &FGameDataRepositoryEditor::CanAddEntry)
            ),
            NAME_None,
            NSLOCTEXT("GameDataRepository", "AddEntry", "Add"),
            NSLOCTEXT("GameDataRepository", "AddEntryTooltip", "Add new entry"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "Plus")
        );

        ToolbarBuilder.AddToolBarButton(
            FUIAction(
                FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::OnDeleteEntry),
                FCanExecuteAction::CreateRaw(this, &FGameDataRepositoryEditor::CanDeleteEntry)
            ),
            NAME_None,
            NSLOCTEXT("GameDataRepository", "DeleteEntry", "Delete"),
            NSLOCTEXT("GameDataRepository", "DeleteEntryTooltip", "Delete selected entry"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "Cross")
        );

        ToolbarBuilder.AddToolBarButton(
            FUIAction(
                FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::OnMoveEntryUp),
                FCanExecuteAction::CreateRaw(this, &FGameDataRepositoryEditor::CanMoveEntryUp)
            ),
            NAME_None,
            NSLOCTEXT("GameDataRepository", "MoveUp", "Move Up"),
            NSLOCTEXT("GameDataRepository", "MoveUpTooltip", "Move selected entry up"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "ArrowUp")
        );

        ToolbarBuilder.AddToolBarButton(
            FUIAction(
                FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::OnMoveEntryDown),
                FCanExecuteAction::CreateRaw(this, &FGameDataRepositoryEditor::CanMoveEntryDown)
            ),
            NAME_None,
            NSLOCTEXT("GameDataRepository", "MoveDown", "Move Down"),
            NSLOCTEXT("GameDataRepository", "MoveDownTooltip", "Move selected entry down"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "ArrowDown")
        );
    }
    ToolbarBuilder.EndSection();
}

TSharedRef<SWidget> FGameDataRepositoryEditor::ImportMenuEntries()
{
    FMenuBuilder MenuBuilder(true, GetToolkitCommands());

    for (const auto& Serializer : Serializers)
    {
        const auto FormatName = Serializer.GetFormatName();
        // Add your dynamic import actions here
        MenuBuilder.AddMenuEntry(
            FText::Format(NSLOCTEXT("GameDataRepository", "ImportFile", "Import from {0}"), FormatName),
            FText::Format(
                NSLOCTEXT("GameDataRepository", "ImportTooltip", "Import entries from a {0} file"), FormatName),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::ImportGameDataRepository, Serializer))
        );
    }


    return MenuBuilder.MakeWidget();
}

void FGameDataRepositoryEditor::ImportGameDataRepository(const FGameDataEntrySerializerPtr Serializer) const {
    if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get(); DesktopPlatform != nullptr)
    {
        const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
        TArray<FString> FileNames;
        if (!DesktopPlatform->OpenFileDialog(
            ParentWindowWindowHandle,
            NSLOCTEXT("GameDataRepository", "Select file to import from", "Select file to import from...").ToString(),
            *FEditorDirectories::Get().GetLastDirectory(ELastDirectory::UNR),
            TEXT(""),
            Serializer.GetFileExtensionText(),
            EFileDialogFlags::None,
            FileNames))
        {
            return;
        }

        const FString& FileName = FileNames[0];
        FEditorDirectories::Get().SetLastDirectory(ELastDirectory::UNR, *FPaths::GetPath(FileName));

        FScopedSlowTask SlowTask(0, NSLOCTEXT("GameDataRepository", "Importing", "Importing..."));
        SlowTask.MakeDialog(false);
        if (FString ErrorMessage; Serializer.Deserialize(FileName, PropertyAccessor->GetGameDataRepository(), ErrorMessage))
        {
            RefreshList();
            FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("GameDataRepository", "ImportSuccessful", "Import was successful!"));
        }
        else
        {
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(MoveTemp(ErrorMessage)));
        }
    }
}

bool FGameDataRepositoryEditor::CanAddEntry() const
{
    return EntrySelector->GetSelectedEntries().Num() < std::numeric_limits<int32>::max();
}

bool FGameDataRepositoryEditor::CanMoveEntryUp() const
{
    if (EntrySelector->IsFiltering())
    {
        return false;
    }

    auto SelectedItems = EntrySelector->GetSelectedEntries();
    auto& AllEntries = EntrySelector->GetEntries();
    return SelectedItems.Num() == 1 && AllEntries.Num() > 0 && SelectedItems[0] != AllEntries[0];
}

bool FGameDataRepositoryEditor::CanMoveEntryDown() const
{
    if (EntrySelector->IsFiltering())
    {
        return false;
    }

    auto SelectedItems = EntrySelector->GetSelectedEntries();
    auto& AllEntries = EntrySelector->GetEntries();
    return SelectedItems.Num() == 1 && AllEntries.Num() > 0 && SelectedItems.Last() != AllEntries.Last();
}

bool FGameDataRepositoryEditor::CanDeleteEntry() const
{
    return EntrySelector->GetSelectedEntries().Num() > 0;
}

void FGameDataRepositoryEditor::OnEntrySelected(const TSharedPtr<FEntryRowData>& Entry)
{
    if (Entry != nullptr)
    {
        DetailsView->SetStructureData(Entry->Entry);
        CurrentRow.Emplace(Entry->Index, Entry->Id);
    }
    else
    {
        DetailsView->SetStructureData(nullptr);
        CurrentRow.Reset();
    }
}

TArray<TSharedPtr<FEntryRowData>> FGameDataRepositoryEditor::OnGetEntries() const
{
    return PropertyAccessor->GetEntries();
}

void FGameDataRepositoryEditor::OnAddEntry() const
{
    auto RowName = PropertyAccessor->GenerateUniqueRowName();
    if (!RowName.IsSet()) return;

    PropertyAccessor->AddEntry(*RowName);
    RefreshList();
}

void FGameDataRepositoryEditor::OnDeleteEntry()
{
    check(CurrentRow.IsSet());
    const auto& Entry = CurrentRow.GetValue();
    PropertyAccessor->RemoveEntry(Entry.Index);
    if (PropertyAccessor->GetNumEntries() == 0)
    {
        CurrentRow.Reset();
    }
    else if (Entry.Index <= PropertyAccessor->GetNumEntries())
    {
        int32 NewIndex = PropertyAccessor->GetNumEntries() - 1;

        CurrentRow.Emplace(NewIndex, PropertyAccessor->GetID(NewIndex));
    }
    RefreshList();
}

void FGameDataRepositoryEditor::OnMoveEntryUp()
{
    check(CurrentRow.IsSet());
    const auto& Entry = CurrentRow.GetValue();
    PropertyAccessor->SwapEntries(Entry.Index, Entry.Index - 1);
    CurrentRow.Emplace(Entry.Index - 1, PropertyAccessor->GetID(Entry.Index - 1));
    RefreshList();
}

void FGameDataRepositoryEditor::OnMoveEntryDown()
{
    check(CurrentRow.IsSet());
    const auto& Entry = CurrentRow.GetValue();
    PropertyAccessor->SwapEntries(Entry.Index, Entry.Index + 1);
    CurrentRow.Emplace(Entry.Index + 1, PropertyAccessor->GetID(Entry.Index + 1));
    RefreshList();
}

void FGameDataRepositoryEditor::RefreshList() const
{
    PropertyAccessor->RefreshRows();
    EntrySelector->RefreshList();
    if (CurrentRow.IsSet())
    {
        EntrySelector->SelectAtIndex(CurrentRow.GetValue().Index);
    }
}

void FGameDataRepositoryEditor::OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.GetPropertyName() != "Id")
    {
        return;
    }

    if (!CurrentRow.IsSet())
    {
        return;
    }
    auto& [Index, CurrentName] = CurrentRow.GetValue();
    if (const auto Id = PropertyAccessor->GetID(Index); !Id.IsValid() || !PropertyAccessor->VerifyRowNameUnique(Id))
    {
        PropertyAccessor->SetID(Index, CurrentName);
    }
    RefreshList();
}
