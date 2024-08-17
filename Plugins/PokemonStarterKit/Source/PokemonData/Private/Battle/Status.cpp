// "Unreal Pokémon" created by Retro & Chill.
#include "Battle/Status.h"
#include "DataManager.h"
#include "RangeHelpers.h"
#include "Ranges/Views/ToArray.h"
#include <range/v3/view/transform.hpp>

TArray<FName> UStatusEffectHelper::GetStatusEffectNames() {
    static auto &Table = FDataManager::GetInstance().GetDataTable<FStatus>();
    auto Rows = Table.GetAllRows();
    return UE::Ranges::CreateRange(Rows) |
           ranges::views::transform([](const FStatus *Status) { return Status->ID; }) | UE::Ranges::ToArray;
}