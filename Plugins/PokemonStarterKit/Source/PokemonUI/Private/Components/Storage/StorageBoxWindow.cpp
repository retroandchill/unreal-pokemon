// "Unreal Pokémon" created by Retro & Chill.


#include "Components/Storage/StorageBoxWindow.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Storage/StorageBoxIcon.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/Ints.h"
#include "Storage/StorageBox.h"

const TScriptInterface<IStorageBox> & UStorageBoxWindow::GetStorageBox() const {
    return StorageBox;
}

void UStorageBoxWindow::SetStorageBox(const TScriptInterface<IStorageBox> &InStorageBox) {
    StorageBox = InStorageBox;
    StorageBox->GetStoredPokemon() |
        UE::Ranges::ForEach(this, &UStorageBoxWindow::CreateStorageBoxIcon);
}

void UStorageBoxWindow::CreateStorageBoxIcon(const TScriptInterface<IPokemon> &Pokemon) {
    check(IsValid(StorageBoxIconClass))
    auto Widget = WidgetTree->ConstructWidget(StorageBoxIconClass);
    if (IsValid(Pokemon.GetObject())) {
        Widget->SetPokemon(Pokemon);
    } else {
        Widget->ResetPokemon();
    }
    SlotOption(Widget);
}