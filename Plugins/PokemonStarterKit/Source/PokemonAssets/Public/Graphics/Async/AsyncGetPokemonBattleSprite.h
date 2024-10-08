// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Graphics/SpriteLoader.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Ranges/Pointers/AsyncLoadHandle.h"
#include "PaperFlipbook.h"

#include "AsyncGetPokemonBattleSprite.generated.h"

class IPokemon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPokemonSpriteLoaded, UPaperFlipbook*, Sprite);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPokemonSpriteLoadFailed);

/**
 * 
 */
UCLASS()
class POKEMONASSETS_API UAsyncGetPokemonBattleSprite : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Sprites|Pokémon", meta = (CallableWithoutWorldContext))
    static UAsyncGetPokemonBattleSprite* GetAsyncGetPokemonBattleSprite(const TScriptInterface<IPokemon>& Pokemon, bool bBack = false);
    
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Sprites|Pokémon", meta = (CallableWithoutWorldContext))
    static UAsyncGetPokemonBattleSprite* GetAsyncGetSpeciesBattleSprite(FName Species, bool bBack = false,
        const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    void Activate() override;
    
private:
    void OnLoadComplete(const TOptional<UPaperFlipbook &> &Flipbook);
    
    UPROPERTY(BlueprintAssignable)
    FOnPokemonSpriteLoaded LoadCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnPokemonSpriteLoadFailed LoadFailed;
    
    UPROPERTY()
    FName Species;

    UPROPERTY()
    bool bBack;

    UPROPERTY()
    FPokemonAssetParams AdditionalParams;

    TSharedPtr<UE::Ranges::TAsyncLoadHandle<UPaperFlipbook>> LoadHandle;
};
