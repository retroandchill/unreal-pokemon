// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSManagedGCHandle.h"
#include "DataHandle.h"

struct FGameDataTypeManagedActions
{
    using FIsValidGameDataHandle = bool(__stdcall *)(FGCHandleIntPtr);
    using FGetDataHandleOptions = void(__stdcall *)(FGCHandleIntPtr, TArray<FDataHandleEntry> *);

    FIsValidGameDataHandle IsValidGameDataHandle = nullptr;
    FGetDataHandleOptions GetDataHandleOptions = nullptr;
};

/**
 *
 */
class GAMEDATAACCESSTOOLSEDITOR_API FGameDataTypeManagedCallbacks
{
    FGameDataTypeManagedCallbacks() = default;
    ~FGameDataTypeManagedCallbacks() = default;

  public:
    static FGameDataTypeManagedCallbacks &Get();

    void SetActions(const FGameDataTypeManagedActions &Callbacks)
    {
        Actions = Callbacks;
    }

    bool IsValidGameDataHandle(const FGCHandle &Handle) const
    {
        return Actions.IsValidGameDataHandle(Handle.GetHandle());
    }

    TArray<FDataHandleEntry> GetDataHandleOptions(const FGCHandle &Handle) const;

  private:
    FGameDataTypeManagedActions Actions;
};
