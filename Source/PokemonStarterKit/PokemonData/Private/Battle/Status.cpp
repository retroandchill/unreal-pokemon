// "Unreal Pokémon" created by Retro & Chill.
#include "Battle/Status.h"
#include "DataManager.h"
#include "RetroLib/Ranges/Algorithm/To.h"

TArray<FName> UStatusEffectHelper::GetStatusEffectNames() {
    static auto &Table = FDataManager::GetInstance().GetDataTable<FStatus>();
    auto Rows = Table.GetAllRows();
    // clang-format off
    return Rows |
           Retro::Ranges::Views::Transform(&FStatus::ID) |
           Retro::Ranges::To<TArray>();
    // clang-format on
}

FName UStatusEffectHelper::ConvertStatusHandleToName(const FStatusHandle &Struct) {
    return Struct;
}

FStatusHandle UStatusEffectHelper::ConvertNameToStatusHandle(FName Name) {
    return Name;
}