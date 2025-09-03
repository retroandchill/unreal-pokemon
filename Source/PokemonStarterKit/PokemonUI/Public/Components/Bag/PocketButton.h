// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"

#include "PocketButton.generated.h"

/**
 *
 */
UCLASS(Abstract)
class POKEMONUI_API UPocketButton : public UCommonButtonBase {
    GENERATED_BODY()

  public:
    void SetPocket(FName PocketIn);

  private:
    FName Pocket;
};
