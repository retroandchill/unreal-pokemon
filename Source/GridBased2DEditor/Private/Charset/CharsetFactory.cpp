// "Unreal Pokémon" created by Retro & Chill.
#include "Charset/CharsetFactory.h"

UCharsetFactory::UCharsetFactory() {
	SupportedClass = UCharset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UCharset* UCharsetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn) {
	return NewObject<UCharset>(InParent, InClass, InName, Flags);
}

bool UCharsetFactory::ShouldShowInNewMenu() const {
	return true;
}
