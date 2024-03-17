// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "Characters/Charset.h"
#include "CharsetFactory.generated.h"

/**
 * Editor Factory for Charset objects
 */
UCLASS(HideCategories=Object)
class GRIDBASED2DEDITOR_API UCharsetFactory : public UFactory {
	GENERATED_BODY()

public:
	UCharsetFactory();

	UCharset* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context,
	                           FFeedbackContext* Warn) override;
	bool ShouldShowInNewMenu() const override;
};
