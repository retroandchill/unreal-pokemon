// "Unreal Pokémon" created by Retro & Chill.

#include "Saving/Serialization/EnhancedSaveGame.h"
#include "StructUtils/StructView.h"

// ReSharper disable once CppMemberFunctionMayBeStatic
bool UEnhancedSaveGame::TryGetData(FGameplayTag, int32 &) const
{
    checkf(false, TEXT("This should never be called directly"));
    return false;
}

DEFINE_FUNCTION(UEnhancedSaveGame::execTryGetData)
{
    const auto *Self = CastChecked<UEnhancedSaveGame>(Context);
    P_GET_STRUCT(FGameplayTag, Tag);
    Stack.StepCompiledIn<FStructProperty>(nullptr);
    const auto DataProperty = CastFieldChecked<FStructProperty>(Stack.MostRecentProperty);
    auto *DataStruct = Stack.MostRecentPropertyAddress;

    P_FINISH;

    P_NATIVE_BEGIN

    auto *ExistingData = Self->Data.Find(Tag);
    if (ExistingData == nullptr || DataProperty->Struct != ExistingData->GetScriptStruct())
    {
        *static_cast<bool *>(RESULT_PARAM) = false;
        return;
    }

    auto *StoredData = ExistingData->GetMemory();
    DataProperty->CopyValuesInternal(DataStruct, StoredData, 1);
    *static_cast<bool *>(RESULT_PARAM) = true;

    P_NATIVE_END
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UEnhancedSaveGame::AddData(FGameplayTag, const int32 &)
{
    checkf(false, TEXT("This should never be called directly"));
}

DEFINE_FUNCTION(UEnhancedSaveGame::execAddData)
{
    auto *Self = CastChecked<UEnhancedSaveGame>(Context);
    P_GET_STRUCT(FGameplayTag, Tag);
    Stack.StepCompiledIn<FStructProperty>(nullptr);
    const auto DataProperty = CastFieldChecked<FStructProperty>(Stack.MostRecentProperty);
    const auto *DataStruct = Stack.MostRecentPropertyAddress;

    P_FINISH;

    P_NATIVE_BEGIN

    Self->Data.Emplace(Tag, FInstancedStruct(FConstStructView(DataProperty->Struct, DataStruct)));

    P_NATIVE_END
}

bool UEnhancedSaveGame::HasData(const FGameplayTag Tag) const
{
    return Data.Contains(Tag);
}