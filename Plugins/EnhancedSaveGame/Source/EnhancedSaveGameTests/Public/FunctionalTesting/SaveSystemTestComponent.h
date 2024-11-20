// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ScopedTestComponent.h"
#include "SaveSystemTestComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENHANCEDSAVEGAMETESTS_API USaveSystemTestComponent : public UScopedTestComponent {
    GENERATED_BODY()

public:
    void AddScopedComponent(AScopedFunctionalTest &TestObject) override;


};