// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetUtilities.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICASSETLOADER_API UAssetUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    static FString GetFullAssetName(FName Identifier, FStringView Prefix);

};
