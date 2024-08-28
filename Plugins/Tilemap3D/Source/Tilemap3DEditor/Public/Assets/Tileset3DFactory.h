// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"

#include "Tileset3DFactory.generated.h"

/**
 *
 */
UCLASS()
class TILEMAP3DEDITOR_API UTileset3DFactory : public UFactory {
    GENERATED_BODY()

  public:
    UTileset3DFactory();

    UObject *FactoryCreateNew(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags, UObject *Context,
                              FFeedbackContext *Warn) override;
};
