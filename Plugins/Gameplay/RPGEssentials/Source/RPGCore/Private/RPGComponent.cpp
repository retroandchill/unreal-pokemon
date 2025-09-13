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

URPGComponent *URPGComponent::GetSiblingComponent(const TSubclassOf<URPGComponent> ComponentClass) const
{
    return OwningEntity->GetComponent(ComponentClass);
}