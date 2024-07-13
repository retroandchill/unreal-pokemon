// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Refreshable.h"
#include "Pokemon/Breeding/PokemonGender.h"

#include "PokemonInfoButtonBase.generated.h"

struct FItem;
class UImage;
class UCommonNumericTextBlock;
class UProgressBar;
class UCommonTextBlock;
class IPokemon;

/**
 * Base implementation for handling the Pokémon stat panel.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonInfoButtonBase : public UCommonButtonBase, public IRefreshable {
    GENERATED_BODY()

  public:
    /**
     * Get the Pokémon that this panel displays the information for
     * @return The Pokémon that this panel displays the information for
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Pokémon")
    const TScriptInterface<IPokemon> &GetPokemon() const;
    
    /**
     * Get the index of the Pokémon that this panel displays the information for
     * @return The index of the Pokémon that this panel displays the information for
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Pokémon")
    int32 GetPartyIndex() const;

    /**
     * Set the Pokémon for the panel setting how everything is displayed
     * @param NewPokemon The Pokémon to set this panel to
     * @param Index The index of this particular panel
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    void SetPokemon(TScriptInterface<IPokemon> NewPokemon, int32 Index);

    /**
     * Swap held Pokémon with another panel
     * @param Other The other panel
     */
    void SwapPokemon(UPokemonInfoButtonBase &Other);

    /**
     * Returns if this panel represents a Pokémon that is currently active in battle
     * @return Is this Pokémon currently active
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsActivated() const;

    /**
     * Returns if the current Pokémon is fainted or not?
     * @return Is the current Pokémon fainted?
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsPokemonFainted() const;

    /**
     * Returns if this panel is the target of a Pokémon position switch
     * @return Is this Pokémon being considered for a swap out
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsSwapping() const;

    UFUNCTION(BlueprintCallable, Category = Display)
    void Refresh() override;

  protected:
    /**
     * Set the Blueprint specific details of the widget based on the information from the state of the Pokémon
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "User Interface|Visuals")
    void RefreshVisuals();

    /**
     * Get the image to use for the held item graphic
     * @param Item The item to be displayed
     * @return The item's graphic
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "User Interface|Visuals")
    UObject *GetHeldItemGraphic(FName Item);

    /**
     * Set up how the Pokémon's gender should be displayed
     * @param Gender The gender of the displayed Pokémon
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "User Interface|Visuals")
    void SetDisplayedGender(EPokemonGender Gender);

  private:
    /**
     * Refresh all the information related to the Pokémon
     */
    void RefreshPokemonInfo();

    /**
     * The Pokémon reference that is currently being held onto
     */
    UPROPERTY(BlueprintGetter = GetPokemon, Category = "Pokémon")
    TScriptInterface<IPokemon> Pokemon;

    /**
     * The index of the Pokémon that this panel displays the information for
     */
    UPROPERTY(BlueprintGetter = GetPartyIndex, Category = "Pokémon")
    int32 PokemonIndex;

    /**
     * The image that displays the icon of the Pokémon to the player
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonIcon;

    /**
     * The image asset that forms the held item icon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> HeldItemIcon;

    /**
     * The text that displays the name for the Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> NameText;

    /**
     * The text block that displays the level for the Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonNumericTextBlock> LevelText;

    /**
     * The text block that displays the current and total HP of the Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> HPText;

    /**
     * The image that displays the HP bar when necessary
     */
    UPROPERTY(DisplayName = "HP Bar", meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar;
};
