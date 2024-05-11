// "Unreal Pokémon" created by Retro & Chill.
#include "Bag/Item.h"
#include "DataManager.h"
#include "Mainpulation/RangeHelpers.h"

#include <ranges>

FItem::FItem() = default;

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

TArray<FName> UItemHelper::GetItemNames() {
    return FDataManager::GetInstance().GetDataTable<FItem>().GetTableRowNames();
}

TArray<FName> UItemHelper::GetPokeBallNames() {
    auto Rows = FDataManager::GetInstance().GetDataTable<FItem>().GetAllRows();
    return RangeHelpers::CreateRange(Rows)
        | std::views::filter([](const FItem* Item) { return Item->IsPokeBall(); })
        | std::views::transform([](const FItem* Item) { return Item->ID; })
        | RangeHelpers::TToArray<FName>();
    
}

bool UItemHelper::IsMail(const FItem &Item) {
    return Item.IsMail();
}
