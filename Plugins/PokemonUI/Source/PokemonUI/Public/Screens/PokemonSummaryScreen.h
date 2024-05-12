// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"
#include "PokemonSummaryScreen.generated.h"

class UHoldItemInfo;
class IPokemon;
class USummaryPages;
class USummaryNameInfo;
class UImage;
/**
 * Screen for displaying a Pokémon's summary.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonSummaryScreen : public UScreen {
    GENERATED_BODY()

protected:
    void NativeConstruct() override;
    
public:
    void SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Party, int32 InitialIndex);

private:
    void SetPokemon(const TScriptInterface<IPokemon>& Pokemon);
    
    /**
     * The widget containing the name information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USummaryNameInfo> NameInfo;

    /**
     * The widget containing the item information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHoldItemInfo> ItemInfo;

    /**
     * The widget that displays the Pokémon sprite
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonSprite;

    /**
     * The widget containing the name information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USummaryPages> SummaryPages;
};
