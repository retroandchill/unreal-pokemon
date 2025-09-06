// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattlerSprite.h"
#include "GameFramework/Actor.h"

#include "BattlerSpriteBase.generated.h"

struct FBattleRender;

UCLASS()
class POKEMONBATTLE_API ABattlerSpriteBase : public AActor, public IBattlerSprite
{
    GENERATED_BODY()

  public:
    void SetBattleSprite_Implementation(const FBattleRender &Render) override;

    UFUNCTION(BlueprintNativeEvent, Category = Creation)
    FTransform GetBattleSpriteTransform(UClass *AssetType);

  private:
    UPROPERTY()
    TObjectPtr<USceneComponent> RenderComponent;
};