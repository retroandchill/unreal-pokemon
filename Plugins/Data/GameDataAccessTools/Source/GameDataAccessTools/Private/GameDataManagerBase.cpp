// "Unreal Pokémon" created by Retro & Chill.

#include "GameDataManagerBase.h"

UGameDataManagerBase *UGameDataManagerBase::Create(TSubclassOf<UGameDataManagerBase> ManagerClass, FName Name)
{
    auto *NewManager =
        NewObject<UGameDataManagerBase>(GetTransientPackage(), ManagerClass, Name, RF_Public | RF_MarkAsRootSet);
    NewManager->Initialize();
    return NewManager;
}

void UGameDataManagerBase::Destroy()
{
    Deinitialize();
    ClearFlags(RF_MarkAsRootSet);
}