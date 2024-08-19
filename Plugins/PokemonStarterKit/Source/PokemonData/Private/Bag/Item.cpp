// "Unreal Pokémon" created by Retro & Chill.
#include "Bag/Item.h"
#include "DataManager.h"
#include "PokemonDataSettings.h"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/transform.hpp"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Map.h"

FItem::FItem() = default;

const FText &FItem::GetPortionName() const {
    return RealPortionName.IsEmpty() ? RealName : RealPortionName;
}

bool FItem::IsTM() const {
    return FieldUse == EFieldUse::TM;
}

bool FItem::IsHM() const {
    return FieldUse == EFieldUse::HM;
}

bool FItem::IsTR() const {
    return FieldUse == EFieldUse::TR;
}

bool FItem::IsImportant() const {
    return IsKeyItem() || IsHM() || IsTM();
}

bool FItem::IsPokeBall() const {
    static const auto PokeBall = FName("PokeBall");
    static const auto SnagBall = FName("SnagBall");
    return Tags.Contains(PokeBall) || Tags.Contains(SnagBall);
}

bool FItem::IsMail() const {
    static const auto Mail = FName("Mail");
    static const auto IconMail = FName("IconMail");
    return Tags.Contains(Mail) || Tags.Contains(IconMail);
}

bool FItem::IsKeyItem() const {
    static const auto KeyItem = FName("KeyItem");
    return Tags.Contains(KeyItem);
}

bool FItem::ShouldShowQuantity() const {
    return ShowQuantity && !IsImportant();
}

bool FItem::CanHold() const {
    return !IsImportant();
}

TArray<FName> UItemHelper::GetItemNames() {
    return FDataManager::GetInstance().GetDataTable<FItem>().GetTableRowNames();
}

TArray<FName> UItemHelper::GetPokeBallNames() {
    auto Rows = FDataManager::GetInstance().GetDataTable<FItem>().GetAllRows();
    return Rows | UE::Ranges::Filter(&FItem::IsPokeBall) |
           UE::Ranges::Map(&FItem::ID) | UE::Ranges::ToArray;
}

TArray<FName> UItemHelper::GetPocketNames() {
    auto &Pockets = GetDefault<UPokemonDataSettings>()->PocketNames;
    TArray<FName> Names;
    for (const auto &[Key, Value] : Pockets) {
        Names.Add(Value);
    }

    return Names;
}

const FText &UItemHelper::GetPortionName(const FItem &Item) {
    return Item.GetPortionName();
}

bool UItemHelper::IsMail(const FItem &Item) {
    return Item.IsMail();
}

bool UItemHelper::CanHold(const FItem &Item) {
    return Item.CanHold();
}
