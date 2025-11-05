// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Graphics/BattleRender.h"
#include "UObject/Interface.h"

#include "BattlerSprite.generated.h"

class UPaperFlipbook;
struct FMaterialInstanceWithSize;
// This class does not need to be modified.
UINTERFACE()
class UBattlerSprite : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents the sprite actor for the battler.
 */
class POKEMONBATTLE_API IBattlerSprite {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Set the battle sprite to the given material
     * @param Render The material to use for the battle sprite
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Visuals")
    void SetBattleSprite(const FBattleRender &Render);

    /**
     * Faint the battler
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Visuals")
    void Faint();
};