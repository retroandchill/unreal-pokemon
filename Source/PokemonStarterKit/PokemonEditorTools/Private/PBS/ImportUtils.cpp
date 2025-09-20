// "Unreal Pokémon" created by Retro & Chill.
#include "PBS/ImportUtils.h"
#include "DesktopPlatformModule.h"
#include "GameplayTagsEditorModule.h"
#include "GameplayTagsManager.h"

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
