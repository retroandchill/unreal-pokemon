// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RangeV3.h"
#include "Structs/UnrealStruct.h"
#include "StructUtils/StructView.h"
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
    void PostLoad() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "RPG Entity")
    void CreateRequiredComponents();

    UFUNCTION(BlueprintCallable, Category = "RPG Entity",
              meta = (DeterminesOutputType = ComponentClass, DynamicOutputParam = ReturnValue))
    URPGComponent *CreateComponent(TSubclassOf<URPGComponent> ComponentClass);

    virtual const UScriptStruct *NativeGetEntityStruct() const
    {
        return nullptr;
    }

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Get Entity Struct", Category = "RPG Entity",
              meta = (ScriptName = "ManagedGetEntityStruct"))
    const UScriptStruct *K2_GetEntityStruct() const;

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext &Context) const override;

    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = Params))
    void InitializeComponents(const int32 &Params);

    DECLARE_FUNCTION(execInitializeComponents);

    void InitializeComponents();

    void InitializeComponents(FStructView Params);

    template <UEStruct T>
    void InitializeComponents(T &Struct)
    {
        InitializeComponents(FStructView(GetScriptStruct<T>(), &Struct));
    }

  private:
    UFUNCTION(meta = (ScriptMethod))
    void DefaultInitializeComponents();

    void GatherComponentReferences();

    FORCEINLINE auto GetAllComponents() const
    {
        return ranges::views::concat(RequiredComponents, AdditionalComponents);
    }

    UPROPERTY()
    TArray<TObjectPtr<URPGComponent>> RequiredComponents;

    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Components")
    TArray<TObjectPtr<URPGComponent>> AdditionalComponents;
};
