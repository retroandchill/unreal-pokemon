// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UE5Coro.h"

#include "PokemonUIAsyncActions.generated.h"

/**
 * 
 */
UCLASS()
class POKEMONUI_API UPokemonUIAsyncActions : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Messages", meta = (Latent, LatentInfo = LatentInfo, WorldContext = WorldContextObject, AutoCreateRefTerm = Message))
    static FVoidCoroutine DisplayMessage(const UObject* WorldContextObject, const FText& Message, const FLatentActionInfo& LatentInfo = FLatentActionInfo());

};
