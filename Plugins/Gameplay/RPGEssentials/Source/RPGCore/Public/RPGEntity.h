// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "RPGEntity.generated.h"

class URPGComponent;

/**
 *
 */
UCLASS(Abstract, DisplayName = "RPG Entity")
class RPGCORE_API URPGEntity : public UObject
{
    GENERATED_BODY()

  public:
    constexpr static auto RPGInitFunction = TEXT("RPGInitFunction");

    UFUNCTION(BlueprintPure, Category = "RPG Entity")
    const UScriptStruct *GetEntityStruct() const;

    UFUNCTION(BlueprintPure, Category = "RPG Entity",
              meta = (DeterminesOutputType = ComponentClass, DynamicOutputParam = ReturnValue))
    URPGComponent *GetComponent(TSubclassOf<URPGComponent> ComponentClass) const;

    template <std::derived_from<URPGComponent> T>
    T *GetComponent(const TSubclassOf<URPGComponent> ComponentClass = T::StaticClass()) const
    {
        return CastChecked<T>(GetComponent(ComponentClass));
    }

  protected:
    void PostInitProperties() override;

    virtual const UScriptStruct *NativeGetEntityStruct() const
    {
        return nullptr;
    }

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Get Entity Struct", Category = "RPG Entity",
              meta = (ScriptName = "GetEntityStruct"))
    const UScriptStruct *K2_GetEntityStruct() const;

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext &Context) const override;
#endif

    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = Params))
    void InitializeComponents(int32 &Params);

    DECLARE_FUNCTION(execInitializeComponents);

  private:
    void GatherComponentReferences();

    void DiscoverAndBindInitFunctions();

    static void DiscoverAndBindInitFunction(URPGComponent *Component, const UScriptStruct *ExpectedStruct);

    static void BindComponentFunction(URPGComponent *Component, UFunction *Function);

    UPROPERTY()
    TArray<TObjectPtr<URPGComponent>> Components;
};
