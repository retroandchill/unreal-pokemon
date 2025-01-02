// "Unreal Pokémon" created by Retro & Chill.
#include "Bag/Item.h"
#include "DataManager.h"
#include "Misc/OutputDeviceNull.h"
#include "PokemonDataSettings.h"

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

FString FPocketKey::ExportText() const {
    FString Ret;
    StaticStruct()->ExportText(Ret, this, this, nullptr, PPF_None, nullptr);
    return Ret;
}

void FPocketKey::FromExportString(FStringView ExportString, int32 PortFlags) {
    if (ExportString.IsEmpty()) {
        return;
    }

    PocketName = NAME_None;

    FOutputDeviceNull NullOut;
    StaticStruct()->ImportText(ExportString.GetData(), this, nullptr, PortFlags, &NullOut, TEXT("FPocketKey"), true);
}

TArray<FName> UItemHelper::GetItemNames() {
    return FDataManager::GetInstance().GetDataTable<FItem>().GetTableRowNames();
}

TArray<FName> UItemHelper::GetPokeBallNames() {
    auto Bytes = FDataManager::GetInstance().GetDataTable<FItem>().GetAllRows();
    // clang-format off
    return FDataManager::GetInstance().GetDataTable<FItem>().GetAllRows() |
           Retro::Ranges::Views::Filter<&FItem::IsPokeBall>() |
           Retro::Ranges::Views::Transform<&FItem::ID>() |
           Retro::Ranges::To<TArray>();
    // clang-format on
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

bool UItemHelper::ShouldShowQuantity(const FItem &Item) {
    return Item.ShouldShowQuantity();
}

FName UItemHelper::ConvertItemHandleToName(const FItemHandle &Struct) {
    return Struct;
}

FItemHandle UItemHelper::ConvertNameToItemHandle(FName Name) {
    return Name;
}