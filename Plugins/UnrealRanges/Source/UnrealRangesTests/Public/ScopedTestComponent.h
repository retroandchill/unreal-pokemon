// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ranges/Utilities/Methods.h"
#include "ScopedTestComponent.generated.h"


class AScopedFunctionalTest;

UCLASS(Abstract, ClassGroup=(FunctionalTesting), meta=(BlueprintSpawnableComponent))
class UNREALRANGESTESTS_API UScopedTestComponent : public UActorComponent {
    GENERATED_BODY()

public:
    virtual void AddScopedComponent(AScopedFunctionalTest& TestObject) ABSTRACT_METHOD;


};