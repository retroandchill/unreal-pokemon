// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UnrealInjector/Public/Services/Service.h"

#include "TestService.generated.h"

/**
 * Test service used for automated tests.
 */
UCLASS()
class UNREALINJECTORTESTS_API UTestService : public UService
{
    GENERATED_BODY()

  public:
    UPROPERTY(BlueprintReadWrite, Category = "Test")
    int32 TestValue = 42;
};
