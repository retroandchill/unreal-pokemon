// "Unreal Pokémon" created by Retro & Chill.
#include "Bag/Item.h"
#include "DataManager.h"

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
    std::span RowSpan(Rows.GetData(), Rows.Num());
    auto FilteredView = std::views::all(RowSpan)
        | std::views::filter([](const FItem* Item) { return Item->IsPokeBall(); })
        | std::views::transform([](const FItem* Item) { return Item->ID; });
    
    TArray<FName> Ret;
    for (FName Name : FilteredView) {
        Ret.Add(Name);
    }
    return Ret;
    
}

bool UItemHelper::IsMail(const FItem &Item) {
    return Item.IsMail();
}
