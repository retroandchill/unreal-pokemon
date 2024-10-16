// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VariantObjectUtilities.generated.h"

struct FVariantObjectTemplate;

/**
 * Utility library for handling operations related to variant objects.
 */
UCLASS()
class UNREALRANGES_API UVariantObjectUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "Variant"))
    static void CreateVariantFromObject(const UScriptStruct* DataStruct, const UObject* Object, int32& Variant);
	DECLARE_FUNCTION(execCreateVariantFromObject);

    

};
