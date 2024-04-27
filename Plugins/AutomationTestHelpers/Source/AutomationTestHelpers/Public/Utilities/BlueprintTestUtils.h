// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintTestUtils.generated.h"

/**
 * Utility class for blueprint related operations.
 */
UCLASS()
class AUTOMATIONTESTHELPERS_API UBlueprintTestUtils : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    static UClass* LoadBlueprintClassByName(FStringView ClassName);

};
