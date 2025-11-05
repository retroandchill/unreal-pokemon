// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Storage/StorageBoxWindow.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Storage/StorageBoxIcon.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "Storage/StorageBox.h"

const TScriptInterface<IStorageBox> &UStorageBoxWindow::GetStorageBox() const {
    return StorageBox;
}

void UStorageBoxWindow::SetStorageBox(const TScriptInterface<IStorageBox> &InStorageBox) {
    StorageBox = InStorageBox;
    StorageBox->GetStoredPokemon() |
        Retro::Ranges::ForEach(Retro::BindMethod<&UStorageBoxWindow::CreateStorageBoxIcon>(this));
}

void UStorageBoxWindow::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    if (OldIndex == NewIndex) {
        return;
    }

    auto StoredPokemon = NewIndex != INDEX_NONE ? StorageBox->GetStoredPokemon(NewIndex) : TOptional<IPokemon &>();
    OnSelectedPokemonChanged.Broadcast(StoredPokemon);
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