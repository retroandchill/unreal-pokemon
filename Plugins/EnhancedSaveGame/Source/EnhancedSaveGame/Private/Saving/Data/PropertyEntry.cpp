// "Unreal Pokémon" created by Retro & Chill.

#include "Saving/Data/PropertyEntry.h"

FPropertyEntry::FPropertyEntry(const FProperty *Property)
    : Data(UE::Ranges::MakeUniqueProperty(Property)), OwningObject(Property->GetOwnerUObject()) {
}

FPropertyEntry::FPropertyEntry(const FPropertyEntry &Other)
    : Data(UE::Ranges::CloneUniqueProperty(Other.Data)), OwningObject(Other.OwningObject) {
}

FPropertyEntry &FPropertyEntry::operator=(const FPropertyEntry &Other) {
    Data = UE::Ranges::CloneUniqueProperty(Other.Data);
    OwningObject = Other.OwningObject;
    return *this;
}

bool FPropertyEntry::Serialize(FArchive &Ar) {
    if (Data == nullptr || Data.get_deleter().GetProperty() == nullptr) {
        return false;
    }

    auto Slot = FStructuredArchiveFromArchive(Ar).GetSlot();
    Data.get_deleter().GetProperty()->SerializeItem(Slot, Data.get());
    return true;
}