// "Unreal Pokémon" created by Retro & Chill.
#include "Bag/Item.h"
#include "DataManager.h"

FItem::FItem() = default;

bool FItem::IsMail() const {
    static const FName Mail = FName("Mail");
    static const FName IconMail = FName("IconMail");
    return Tags.Contains(Mail) || Tags.Contains(IconMail);
}

TArray<FName> UItemHelper::GetItemNames() {
    return FDataManager::GetInstance().GetDataTable<FItem>().GetTableRowNames();
}

bool UItemHelper::IsMail(const FItem &Item) {
    return Item.IsMail();
}
