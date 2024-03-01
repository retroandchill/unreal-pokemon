// "Unreal Pokémon" created by Retro & Chill.
#include "Windowskin/WindowskinFactory.h"

UWindowskinFactory::UWindowskinFactory() {
	SupportedClass = UWindowskin::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UWindowskin* UWindowskinFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn) {
	return NewObject<UWindowskin>(InParent, InClass, InName, Flags);
}

bool UWindowskinFactory::ShouldShowInNewMenu() const {
	return true;
}
