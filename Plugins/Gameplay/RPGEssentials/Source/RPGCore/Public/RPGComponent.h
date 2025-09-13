// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
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

    FRPGComponentInitializer& operator=(UFunction* Function);

    void Execute(URPGComponent* Component, FStructView Data) const;
    
private:
    UPROPERTY()
    TObjectPtr<UFunction> InitFunction;
};

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
#define BindInitFunctionDynamic(Type, FunctionName) BindInitFunction(FindFieldChecked<UFunction>(Type::StaticClass(), GET_FUNCTION_NAME_CHECKED(Type, FunctionName))

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
    void Initialize(const FStructView Data) {
        InitFunction.Execute(this, Data);   
    }

    void BindInitFunction(UFunction *Function)
    {
        InitFunction = Function;
    }

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    URPGEntity* GetOwningEntity() const
    {
        return OwningEntity;
    }

    UFUNCTION(BlueprintPure, Category = "RPG Component",
        meta = (DeterminesOutputType = ComponentClass, DynamicOutputParam = ReturnValue))
    URPGComponent *GetSiblingComponent(TSubclassOf<URPGComponent> ComponentClass) const;

    template <std::derived_from<URPGComponent> T>
    T* GetSiblingComponent(const TSubclassOf<URPGComponent> ComponentClass = T::StaticClass()) const
    {
        return CastChecked<T>(GetSiblingComponent(ComponentClass));
    }
    
private:
    UPROPERTY(EditAnywhere, Category = "RPG Component")
    FRPGComponentInitializer InitFunction;

    UPROPERTY(BlueprintGetter = GetOwningEntity, Category = "RPG Component")
    TObjectPtr<URPGEntity> OwningEntity;
};
