// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Structs/UnrealStruct.h"
#include "StructUtils/StructView.h"
#include "UObject/Object.h"

#include "RPGComponent.generated.h"

class URPGComponent;
class URPGEntity;

USTRUCT(DisplayName = "RPG Component Initializer")
struct RPGCORE_API FRPGComponentInitializer
{
    GENERATED_BODY()

    FRPGComponentInitializer() = default;
    explicit FRPGComponentInitializer(UFunction *InitFunction);

    FRPGComponentInitializer &operator=(UFunction *Function);

    void Execute(URPGComponent *Component, FStructView Data) const;

  private:
    UPROPERTY()
    TObjectPtr<UFunction> InitFunction;

#if WITH_EDITOR
    friend class FRPGComponentInitializerCustomization;
#endif
};

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
#define BindInitFunctionDynamic(Type, FunctionName) BindInitFunction(FindFieldChecked<UFunction>(Type::StaticClass(), &Type::FunctionName, GET_FUNCTION_NAME_CHECKED(Type, FunctionName))

/**
 * @brief Abstract base class for RPG components.
 *
 * Represents a modular and extendable component system for RPG entities. It is designed to be used
 * as a blueprintable base class for defining various types of components that interact with RPG entities.
 *
 * @note This class should not be instantiated directly. Extend it to define specific components.
 */
UCLASS(Abstract, DisplayName = "RPG Component", EditInlineNew)
class RPGCORE_API URPGComponent : public UObject
{
    GENERATED_BODY()

  public:
    void PreInitialize(URPGEntity *InOwningEntity);

    void Initialize(const FStructView Data)
    {
        InitFunction.Execute(this, Data);
    }

    void BindInitFunction(UFunction *Function)
    {
        InitFunction = Function;
    }

    template <std::derived_from<URPGComponent> UserClass, UEStruct T>
    void BindInitFunction(const TSubclassOf<URPGComponent> ComponentClass, void (UserClass::*)(T),
                          const FName FunctionName)
    {
        BindInitFunction(FindFieldChecked<UFunction>(ComponentClass, FunctionName));
    }

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

  protected:
    virtual void NativePreInitialize()
    {
    }

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Pre-Initialize", Category = "RPG Component",
              meta = (ScriptName = "PreInitialize"))
    void K2_PreInitialize();

  private:
    UFUNCTION(meta = (ScriptMethod))
    UFunction *BindInitFunctionInternal(const FName FunctionName);

    UPROPERTY(EditAnywhere, Category = "Initialization")
    FRPGComponentInitializer InitFunction;

    UPROPERTY(BlueprintGetter = GetOwningEntity, Category = "RPG Component")
    TObjectPtr<URPGEntity> OwningEntity;
};
