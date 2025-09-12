// "Unreal Pokémon" created by Retro & Chill.

#include "RPGComponent.h"
#include "RPGEntity.h"

URPGComponent *URPGComponent::GetSiblingComponent(const TSubclassOf<URPGComponent> ComponentClass) const
{
    return OwningEntity->GetComponent(ComponentClass);
}