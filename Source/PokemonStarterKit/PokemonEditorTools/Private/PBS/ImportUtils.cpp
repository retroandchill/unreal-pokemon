// "Unreal Pokémon" created by Retro & Chill.
#include "PBS/ImportUtils.h"
#include "DesktopPlatformModule.h"
#include "GameplayTagsEditorModule.h"
#include "GameplayTagsManager.h"
#include "PokemonDataSettings.h"

TArray<FStat> UImportUtils::GetMainStatsFromTable(const UDataTable *const DataTable)
{
    TArray<FStat *> References;
    DataTable->GetAllRows(TEXT("UImportUtils::GetMainStatsFromTable"), References);
    TArray<FStat> Ret;
    for (const auto Ref : References)
    {
        if (Ref->Type == EPokemonStatType::Battle)
            continue;

        Ret.Add(*Ref);
    }

    return Ret;
}

FText UImportUtils::SelectFile(const FString &FileTypes)
{
    auto DesktopPlatform = FDesktopPlatformModule::Get();
    if (DesktopPlatform == nullptr)
    {
        return FText::FromStringView(TEXT(""));
    }

    auto WindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
    TArray<FString> SelectedFiles;
    DesktopPlatform->OpenFileDialog(WindowHandle, TEXT("Select a File"), FPaths::GetProjectFilePath(), TEXT(""),
                                    FileTypes, EFileDialogFlags::None, SelectedFiles);
    if (SelectedFiles.IsEmpty())
    {
        return FText::FromStringView(TEXT(""));
    }

    return FText::FromStringView(SelectedFiles[0]);
}

const TMap<uint8, FName> &UImportUtils::GetPocketNames()
{
    return GetDefault<UPokemonDataSettings>()->PocketNames;
}

bool UImportUtils::TryAddGameplayTagToIni(FName TagSource, const FString &TagName, FString &Error)
{
    auto &Manager = UGameplayTagsManager::Get();
    if (const auto RequestedTag = Manager.RequestGameplayTag(FName(TagName), false); RequestedTag.IsValid())
    {
        Error.Empty();
        return true;
    }

    FText ErrorMsg;
    if (!Manager.IsValidGameplayTagString(TagName, &ErrorMsg))
    {
        Error = ErrorMsg.ToString();
        return false;
    }

    if (!IGameplayTagsEditorModule::Get().AddNewGameplayTagToINI(TagName, TEXT(""), TagSource))
    {
        Error = TEXT("An unknown error occurred!");
        return false;
    }

    Error.Empty();
    return true;
}
