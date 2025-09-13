// "Unreal Pokémon" created by Retro & Chill.

#include "RPGComponent.h"
#include "RPGEntity.h"

FRPGComponentInitializer::FRPGComponentInitializer(UFunction *InitFunction) : InitFunction(InitFunction)
{
}

FRPGComponentInitializer &FRPGComponentInitializer::operator=(UFunction *Function)
{
    InitFunction = Function;
    return *this;
}

void FRPGComponentInitializer::Execute(URPGComponent *Component, const FStructView Data) const
{
    if (InitFunction == nullptr)
        return;

    Component->ProcessEvent(InitFunction, Data.GetMemory());
}

void URPGComponent::PreInitialize(URPGEntity *InOwningEntity)
{
    OwningEntity = InOwningEntity;
    NativePreInitialize();
    K2_PreInitialize();
}

URPGComponent *URPGComponent::GetSiblingComponentInternal(const TSubclassOf<URPGComponent> ComponentClass) const
{
    return OwningEntity->GetComponent(ComponentClass);
}

UFunction *URPGComponent::BindInitFunctionInternal(const FName FunctionName)
{
    auto *FoundFunction = FindUField<UFunction>(GetClass(), FunctionName);
    if (FoundFunction != nullptr)
    {
        InitFunction = FRPGComponentInitializer(FoundFunction);
    }
    return FoundFunction;
}