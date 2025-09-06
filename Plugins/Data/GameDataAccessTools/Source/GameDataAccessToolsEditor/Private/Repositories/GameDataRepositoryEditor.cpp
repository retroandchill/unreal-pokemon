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
                                           const TSharedPtr<IToolkitHost> &InitToolkitHost, UGameDataRepository *Asset)
{
    GameDataRepository = Asset;

    const auto Layout = FTabManager::NewLayout("GameDataRepositoryEditor_Layout")
                            ->AddArea(FTabManager::NewPrimaryArea()
                                          ->SetOrientation(Orient_Horizontal)
                                          ->Split(FTabManager::NewStack()->SetSizeCoefficient(0.3f)->AddTab(
                                              "EntrySelectionTab", ETabState::OpenedTab))
                                          ->Split(FTabManager::NewStack()->SetSizeCoefficient(0.7f)->AddTab(
                                              "EntryEditTab", ETabState::OpenedTab)));

    Serializers = IGameDataAccessToolsEditorModule::Get().GetAvailableSerializers(Asset->GetClass());

    const auto ToolbarExtender = MakeShared<FExtender>();
    ToolbarExtender->AddToolBarExtension(
        "Asset", EExtensionHook::After, GetToolkitCommands(),
        FToolBarExtensionDelegate::CreateRaw(this, &FGameDataRepositoryEditor::FillToolbar));
    AddToolbarExtender(ToolbarExtender);

    InitAssetEditor(Mode, InitToolkitHost, "GameDataRepositoryEditor", Layout, true, true, Asset);
}

void FGameDataRepositoryEditor::RegisterTabSpawners(const TSharedRef<FTabManager> &InTabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(
        NSLOCTEXT("GameDataRepository", "GameDataRepository", "Game Data Asset"));

    InTabManager
        ->RegisterTabSpawner("EntrySelectionTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs &) {
                                 return SNew(
                                     SDockTab)[SAssignNew(EntrySelector, SGameDataRepositoryEntrySelector)
                                                   .OnEntrySelected(this, &FGameDataRepositoryEditor::OnEntrySelected)
                                                   .OnGetEntries(this, &FGameDataRepositoryEditor::OnGetEntries)];
                             }))
        .SetDisplayName(NSLOCTEXT("GameDataRepository", "EntrySelectionTab", "Entries"))
        .SetGroup(WorkspaceMenuCategory.ToSharedRef());

    FPropertyEditorModule &PropertyEditorModule =
        FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    FStructureDetailsViewArgs DetailsViewArgsStruct;

    auto *CurrentEntry = GameDataRepository->GetMutableEntryAtIndex(0);
    const auto EntryStruct = CurrentEntry != nullptr
                                 ? MakeShared<FStructOnScope>(GameDataRepository->GetEntryStruct(), CurrentEntry)
                                 : TSharedPtr<FStructOnScope>();
    DetailsView = PropertyEditorModule.CreateStructureDetailView(DetailsViewArgs, DetailsViewArgsStruct, EntryStruct);
    DetailsView->GetOnFinishedChangingPropertiesDelegate().AddRaw(this, &FGameDataRepositoryEditor::OnPropertyChanged);
    InTabManager
        ->RegisterTabSpawner("EntryEditTab", FOnSpawnTab::CreateLambda([this](const FSpawnTabArgs &) {
                                 return SNew(SDockTab)[DetailsView->GetWidget().ToSharedRef()];
                             }))
        .SetDisplayName(NSLOCTEXT("GameDataRepository", "Details", "Details"))
        .SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FGameDataRepositoryEditor::UnregisterTabSpawners(const TSharedRef<FTabManager> &InTabManager)
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

void FGameDataRepositoryEditor::FillToolbar(FToolBarBuilder &ToolbarBuilder)
{
    // Add import section
    ToolbarBuilder.BeginSection("Import");
    {
        // Add import button that will be populated with dynamic actions
        ToolbarBuilder.AddComboButton(
            FUIAction(), FOnGetContent::CreateSP(this, &FGameDataRepositoryEditor::ImportMenuEntries),
            NSLOCTEXT("GameDataRepository", "Import", "Import"),
            NSLOCTEXT("GameDataRepository", "ImportTooltip", "Import data from various sources"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Reimport"));
    }
    ToolbarBuilder.EndSection();

    // Add entry management section
    ToolbarBuilder.BeginSection("EntryManagement");
    {
        ToolbarBuilder.AddToolBarButton(
            FUIAction(FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::OnAddEntry),
                      FCanExecuteAction::CreateRaw(this, &FGameDataRepositoryEditor::CanAddEntry)),
            NAME_None, NSLOCTEXT("GameDataRepository", "AddEntry", "Add"),
            NSLOCTEXT("GameDataRepository", "AddEntryTooltip", "Add new entry"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "Plus"));

        ToolbarBuilder.AddToolBarButton(
            FUIAction(FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::OnDeleteEntry),
                      FCanExecuteAction::CreateRaw(this, &FGameDataRepositoryEditor::CanDeleteEntry)),
            NAME_None, NSLOCTEXT("GameDataRepository", "DeleteEntry", "Delete"),
            NSLOCTEXT("GameDataRepository", "DeleteEntryTooltip", "Delete selected entry"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "Cross"));

        ToolbarBuilder.AddToolBarButton(
            FUIAction(FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::OnMoveEntryUp),
                      FCanExecuteAction::CreateRaw(this, &FGameDataRepositoryEditor::CanMoveEntryUp)),
            NAME_None, NSLOCTEXT("GameDataRepository", "MoveUp", "Move Up"),
            NSLOCTEXT("GameDataRepository", "MoveUpTooltip", "Move selected entry up"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "ArrowUp"));

        ToolbarBuilder.AddToolBarButton(
            FUIAction(FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::OnMoveEntryDown),
                      FCanExecuteAction::CreateRaw(this, &FGameDataRepositoryEditor::CanMoveEntryDown)),
            NAME_None, NSLOCTEXT("GameDataRepository", "MoveDown", "Move Down"),
            NSLOCTEXT("GameDataRepository", "MoveDownTooltip", "Move selected entry down"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "ArrowDown"));
    }
    ToolbarBuilder.EndSection();
}

TSharedRef<SWidget> FGameDataRepositoryEditor::ImportMenuEntries()
{
    FMenuBuilder MenuBuilder(true, GetToolkitCommands());

    for (const auto &Serializer : Serializers)
    {
        const auto FormatName = Serializer->GetFormatName();
        // Add your dynamic import actions here
        MenuBuilder.AddMenuEntry(
            FText::Format(NSLOCTEXT("GameDataRepository", "ImportFile", "Import from {0}"), FormatName),
            FText::Format(NSLOCTEXT("GameDataRepository", "ImportTooltip", "Import entries from a {0} file"),
                          FormatName),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateSP(this, &FGameDataRepositoryEditor::ImportGameDataRepository,
                                               Serializer.Get())));
    }

    return MenuBuilder.MakeWidget();
}

void FGameDataRepositoryEditor::ImportGameDataRepository(const UGameDataEntrySerializer *Serializer) const
{
    if (IDesktopPlatform *DesktopPlatform = FDesktopPlatformModule::Get(); DesktopPlatform != nullptr)
    {
        const void *ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
        TArray<FString> FileNames;
        if (!DesktopPlatform->OpenFileDialog(
                ParentWindowWindowHandle,
                NSLOCTEXT("GameDataRepository", "Select file to import from", "Select file to import from...")
                    .ToString(),
                *FEditorDirectories::Get().GetLastDirectory(ELastDirectory::UNR), TEXT(""),
                Serializer->GetFileExtensionText(), EFileDialogFlags::None, FileNames))
        {
            return;
        }

        const FString &FileName = FileNames[0];
        FEditorDirectories::Get().SetLastDirectory(ELastDirectory::UNR, *FPaths::GetPath(FileName));

        FScopedSlowTask SlowTask(0, NSLOCTEXT("GameDataRepository", "Importing", "Importing..."));
        SlowTask.MakeDialog(false);
        if (FString ErrorMessage; Serializer->Deserialize(FileName, GameDataRepository, ErrorMessage))
        {
            RefreshList();
            FMessageDialog::Open(EAppMsgType::Ok,
                                 NSLOCTEXT("GameDataRepository", "ImportSuccessful", "Import was successful!"));
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
    auto &AllEntries = EntrySelector->GetEntries();
    return SelectedItems.Num() == 1 && AllEntries.Num() > 0 && SelectedItems[0] != AllEntries[0];
}

bool FGameDataRepositoryEditor::CanMoveEntryDown() const
{
    if (EntrySelector->IsFiltering())
    {
        return false;
    }

    auto SelectedItems = EntrySelector->GetSelectedEntries();
    auto &AllEntries = EntrySelector->GetEntries();
    return SelectedItems.Num() == 1 && AllEntries.Num() > 0 && SelectedItems.Last() != AllEntries.Last();
}

bool FGameDataRepositoryEditor::CanDeleteEntry() const
{
    return EntrySelector->GetSelectedEntries().Num() > 0;
}

void FGameDataRepositoryEditor::OnEntrySelected(const TSharedPtr<FEntryRowData> &Entry)
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
    TArray<TSharedPtr<FEntryRowData>> Entries;
    for (int32 i = 0; i < GameDataRepository->GetNumEntries(); i++)
    {

        auto *Entry = GameDataRepository->GetMutableEntryAtIndex(i);
        const auto ID = GameDataRepository->IDProperty->GetPropertyValue_InContainer(Entry);
        Entries.Emplace(
            MakeShared<FEntryRowData>(i, ID, MakeShared<FStructOnScope>(GameDataRepository->GetEntryStruct(), Entry)));
    }
    return Entries;
}

void FGameDataRepositoryEditor::OnAddEntry() const
{
    auto RowName = GameDataRepository->GenerateUniqueRowName();
    if (!RowName.IsSet())
        return;

    GameDataRepository->AddNewEntry(*RowName);
    RefreshList();
}

void FGameDataRepositoryEditor::OnDeleteEntry()
{
    check(CurrentRow.IsSet());
    const auto &Entry = CurrentRow.GetValue();
    GameDataRepository->RemoveEntryAtIndex(Entry.Index);
    if (GameDataRepository->GetNumEntries() == 0)
    {
        CurrentRow.Reset();
    }
    else if (Entry.Index <= GameDataRepository->GetNumEntries())
    {
        int32 NewIndex = GameDataRepository->GetNumEntries() - 1;

        const auto *CurrentEntry = GameDataRepository->GetMutableEntryAtIndex(NewIndex);
        CurrentRow.Emplace(NewIndex, GameDataRepository->IDProperty->GetPropertyValue_InContainer(CurrentEntry));
    }
    RefreshList();
}

void FGameDataRepositoryEditor::OnMoveEntryUp()
{
    check(CurrentRow.IsSet());
    const auto &Entry = CurrentRow.GetValue();
    GameDataRepository->SwapEntries(Entry.Index, Entry.Index - 1);
    const auto *CurrentEntry = GameDataRepository->GetMutableEntryAtIndex(Entry.Index - 1);
    CurrentRow.Emplace(Entry.Index - 1, GameDataRepository->IDProperty->GetPropertyValue_InContainer(CurrentEntry));
    RefreshList();
}

void FGameDataRepositoryEditor::OnMoveEntryDown()
{
    check(CurrentRow.IsSet());
    const auto &Entry = CurrentRow.GetValue();
    GameDataRepository->SwapEntries(Entry.Index, Entry.Index + 1);
    const auto *CurrentEntry = GameDataRepository->GetMutableEntryAtIndex(Entry.Index + 1);
    CurrentRow.Emplace(Entry.Index + 1, GameDataRepository->IDProperty->GetPropertyValue_InContainer(CurrentEntry));
    RefreshList();
}

void FGameDataRepositoryEditor::RefreshList() const
{
    EntrySelector->RefreshList();
    if (CurrentRow.IsSet())
    {
        EntrySelector->SelectAtIndex(CurrentRow.GetValue().Index);
    }
}

void FGameDataRepositoryEditor::OnPropertyChanged(const FPropertyChangedEvent &PropertyChangedEvent)
{
    if (PropertyChangedEvent.GetPropertyName() != UGameDataRepositoryBase::EntryIDProperty)
    {
        return;
    }

    if (!CurrentRow.IsSet())
    {
        return;
    }

    auto &[Index, CurrentName] = CurrentRow.GetValue();
    auto *Entry = GameDataRepository->GetMutableEntryAtIndex(Index);
    if (const auto Id = GameDataRepository->IDProperty->GetPropertyValue_InContainer(Entry);
        !Id.IsValid() || !GameDataRepository->VerifyRowNameUnique(Id))
    {
        GameDataRepository->IDProperty->SetValue_InContainer(Entry, Id);
        if (auto *Key = GameDataRepository->RowIndices.FindKey(Index);
            ensureMsgf(Key != nullptr, TEXT("Failed to find key for index %d"), Index))
        {
            GameDataRepository->RowIndices.Remove(*Key);
        }

        GameDataRepository->RowIndices.Add(Id, Index);
    }
    RefreshList();
}
