// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/StructView.h"
#include "UObject/Object.h"

#include "RPGComponent.generated.h"

class URPGEntity;
/**
 *
 */
UCLASS(Abstract, DisplayName = "RPG Component", EditInlineNew)
class RPGCORE_API URPGComponent : public UObject
{
    GENERATED_BODY()

  public:
    DECLARE_DELEGATE_OneParam(FInitComponent, FStructView);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    URPGEntity *GetOwningEntity() const
    {
        return OwningEntity;
    }

    UFUNCTION(BlueprintPure, Category = "RPG Component",
              meta = (DeterminesOutputType = ComponentClass, DynamicOutputParam = ReturnValue))
    URPGComponent *GetSiblingComponent(TSubclassOf<URPGComponent> ComponentClass) const;

    template <std::derived_from<URPGComponent> T>
    T *GetSiblingComponent(const TSubclassOf<URPGComponent> ComponentClass = T::StaticClass()) const
    {
        return CastChecked<T>(GetSiblingComponent(ComponentClass));
    }

  private:
    FInitComponent InitDelegate;

    UPROPERTY(BlueprintGetter = GetOwningEntity, Category = "RPG Component")
    TObjectPtr<URPGEntity> OwningEntity;

    friend class URPGEntity;
};
