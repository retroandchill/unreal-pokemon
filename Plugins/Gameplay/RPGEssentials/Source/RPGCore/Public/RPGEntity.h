// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/StructView.h"
#include "UObject/Object.h"

#include "RPGEntity.generated.h"

class URPGComponent;

template <typename T>
    concept CoreStructType = requires {
    { TBaseStructure<std::remove_cvref_t<T>>::Get() } -> std::same_as<UScriptStruct *>;
    };

/**
 * Concept for any USTRUCT in the editor.
 */
template <typename T>
concept DeclaredStruct = requires {
    { std::remove_cvref_t<T>::StaticStruct() } -> std::same_as<UScriptStruct *>;
};

template <typename T>
concept UEStruct = CoreStructType<T> || DeclaredStruct<T>;

template <typename T>
        requires UEStruct<T>
constexpr UScriptStruct *GetScriptStruct()
{
    if constexpr (DeclaredStruct<T>)
    {
        return std::remove_cvref_t<T>::StaticStruct();
    }
    else
    {
        return TBaseStructure<std::remove_cvref_t<T>>::Get();
    }
}

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
              meta = (ScriptName = "ManagedGetEntityStruct"))
    const UScriptStruct *K2_GetEntityStruct() const;

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext &Context) const override;

    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = Params))
    void InitializeComponents(const int32 &Params);

    DECLARE_FUNCTION(execInitializeComponents);

    void InitializeComponents(FStructView Params);

    template <UEStruct T>
    void InitializeComponents(T& Struct)
    {
        InitializeComponents(FStructView(GetScriptStruct<T>(), &Struct));   
    }

  private:
    void GatherComponentReferences();

    void DiscoverAndBindInitFunctions();

    static void DiscoverAndBindInitFunction(URPGComponent *Component, const UScriptStruct *ExpectedStruct);

    static void BindComponentFunction(URPGComponent *Component, UFunction *Function);

    UPROPERTY()
    TArray<TObjectPtr<URPGComponent>> Components;
};
