// "Unreal Pokémon" created by Retro & Chill.

#include "Savers/ComponentSaver.h"
#include "RPGComponent.h"

bool UComponentSaver::Supports(const TSubclassOf<URPGComponent> Component) const
{
    if (K2_Supports(Component))
    {
        return true;
    }

    return NativeSupports(Component);
}

FRPGComponentSaveDataHandle UComponentSaver::SaveData(const URPGComponent *Component) const
{
    FRPGComponentSaveDataHandle Handle(Component->GetClass(), GetVersion());

    NativeSaveData(Component, Handle);
    K2_SaveData(Component, Handle);

    return Handle;
}

void UComponentSaver::LoadData(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const
{
    NativeLoadData(Component, SaveData);
    K2_LoadData(Component, SaveData);
}

void UComponentSaver::NativeSaveData(const URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const
{
    // No implementation here
}

void UComponentSaver::NativeLoadData(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const
{
    // No implementation here
}