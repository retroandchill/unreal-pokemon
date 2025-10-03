// "Unreal Pokémon" created by Retro & Chill.

#include "Effects/EffectContainer.h"
#include "Blueprints/ThunkUtilities.h"
#include "OptionalPtr.h"
#include "Unreachable.h"

bool UEffectContainer::HasEffect(const FName Effect) const
{
    return EffectData.Contains(Effect);
}

const UScriptStruct *UEffectContainer::TryGetEffectType(const FName Effect) const
{
    return TOptionalPtr(EffectData.Find(Effect)).Map(&FInstancedStruct::GetScriptStruct).Get();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
CUSTOM_THUNK_STUB_CONST(bool, UEffectContainer::TryGetEffectData, FName, int32 &);

DEFINE_FUNCTION(UEffectContainer::execTryGetEffectData)
{
    const auto *Self = CastChecked<UEffectContainer>(Context);
    P_GET_PROPERTY(FNameProperty, Effect);
    P_GET_OPAQUE_STRUCT(DataProperty, Data);
    P_GET_RESULT(bool, Ret);

    P_FINISH

    P_NATIVE_BEGIN

    auto *ExistingData = Self->EffectData.Find(Effect);
    if (ExistingData == nullptr || ExistingData->GetScriptStruct() != DataProperty->Struct)
    {
        Ret = false;
        return;
    }

    DataProperty->CopyCompleteValue(Data, ExistingData->GetMemory());
    Ret = true;

    P_NATIVE_END
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeStatic
CUSTOM_THUNK_STUB(bool, UEffectContainer::SetEffectData, FName, const int32 &, bool);

DEFINE_FUNCTION(UEffectContainer::execSetEffectData)
{
    auto *Self = CastChecked<UEffectContainer>(Context);
    P_GET_PROPERTY(FNameProperty, Effect);
    P_GET_OPAQUE_STRUCT(DataProperty, Data);
    P_GET_PROPERTY(FBoolProperty, AllowTypeChange);
    P_GET_RESULT(bool, Ret);

    P_FINISH

    P_NATIVE_BEGIN

    if (auto *ExistingData = Self->EffectData.Find(Effect); ExistingData != nullptr)
    {
        if (AllowTypeChange || ExistingData->GetScriptStruct() == DataProperty->Struct)
        {
            ExistingData->InitializeAs(DataProperty->Struct, Data);
            Ret = true;
        }

        Ret = false;
    }
    else
    {
        auto &Struct = Self->EffectData.Emplace(Effect, FInstancedStruct());
        Struct.InitializeAs(DataProperty->Struct, Data);
        Ret = true;
    }

    P_NATIVE_END
}

void UEffectContainer::ResetEffect(const FName Effect)
{
    if (auto *Data = EffectData.Find(Effect); Data != nullptr)
    {
        Data->Reset();
    }
}

void UEffectContainer::ResetAllEffects()
{
    for (auto &[Key, Value] : EffectData)
    {
        Value.Reset();
    }
}

void UEffectContainer::RemoveEffect(const FName Effect)
{
    EffectData.Remove(Effect);
}

void UEffectContainer::ClearEffects()
{
    EffectData.Empty();
}