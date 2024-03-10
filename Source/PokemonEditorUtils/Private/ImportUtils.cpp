// "Unreal Pokémon" created by Retro & Chill.
#include "ImportUtils.h"

#include "DesktopPlatformModule.h"

TArray<FStat> UImportUtils::GetMainStatsFromTable(UDataTable* const DataTable) {
	TArray<FStat*> References;
	DataTable->GetAllRows(TEXT("UImportUtils::GetMainStatsFromTable"), References);
	TArray<FStat> Ret;
	for (const auto Ref : References) {
		if (Ref->Type == EPokemonStatType::Battle)
			continue;

		Ret.Add(*Ref);
	}

	return Ret;
}

FText UImportUtils::SelectFile(const FString& FileTypes) {
	auto DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform == nullptr) {
		return FText::FromStringView(TEXT(""));
	}

	auto WindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	TArray<FString> SelectedFiles;
	DesktopPlatform->OpenFileDialog(WindowHandle, TEXT("Select a File"), FPaths::GetProjectFilePath(), TEXT(""),
	                                FileTypes, EFileDialogFlags::None, SelectedFiles);
	if (SelectedFiles.IsEmpty()) {
		return FText::FromStringView(TEXT(""));
	}

	return FText::FromStringView(SelectedFiles[0]);
}
