// "Unreal Pokémon" created by Retro & Chill.
#include "Battle/Status.h"
#include "DataManager.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Map.h"

TArray<FName> UStatusEffectHelper::GetStatusEffectNames() {
    static auto &Table = FDataManager::GetInstance().GetDataTable<FStatus>();
    auto Rows = Table.GetAllRows();
    // clang-format off
    return Rows |
           UE::Ranges::Map(&FStatus::ID) |
           UE::Ranges::ToArray;
    // clang-format on
}

FName UStatusEffectHelper::ConvertStatusHandleToName(const FStatusHandle &Struct) {
    return Struct;
}

FStatusHandle UStatusEffectHelper::ConvertNameToStatusHandle(FName Name) {
    return Name;
}