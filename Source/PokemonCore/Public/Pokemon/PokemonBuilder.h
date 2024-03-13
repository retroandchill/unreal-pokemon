// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonDTO.h"
#include "UObject/Object.h"
#include "PokemonBuilder.generated.h"

class IPokemon;
/**
 * Builder object used to take the Pokémon DTO struct and convert it into the actual Pokémon data class
 */
UCLASS()
class POKEMONCORE_API UPokemonBuilder : public UObject {
	GENERATED_BODY()

public:
	/**
	 * Set the species for the new Pokémon
	 * @param Species The ID of the species
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder* Species(FName Species);

	/**
	 * Set the level for the new Pokémon
	 * @param Level The level to construct at
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder* Level(int32 Level);

	/**
	 * Set the personality value for the new Pokémon
	 * @param PersonalityValue The personality value to set
	 * @return A reference to this object
	 */
	UPokemonBuilder* PersonalityValue(uint32 PersonalityValue);

	/**
	 * Set the nickname for the new Pokémon
	 * @param Nickname The nickname to set
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder* Nickname(FText Nickname);

	/**
	 * Set the gender for the new Pokémon
	 * @param Gender The gender of the new Pokémon
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder* Gender(EPokemonGender Gender);

	/**
	 * Set the shiny state for the new Pokémon
	 * @param bShiny Should the Pokémon be shiny
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder* Shiny(bool bShiny);

	/**
	 * Set the entire state block for the Pokémon
	 * @param StatBlock The stat block
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder* StatBlock(const FStatBlockDTO &StatBlock);

	/**
	 * Set the entire state block for the Pokémon
	 * @param StatBlock The stat block
	 * @return A reference to this object
	 */
	UPokemonBuilder* StatBlock(FStatBlockDTO &&StatBlock);

	/**
	 * Set the Exp. total for the new Pokémon
	 * @param Exp The total Exp.
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder* Exp(int32 Exp);

	/**
	 * Set the current HP for the new Pokémon
	 * @param CurrentHP What should the current HP of the Pokémon be
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder* CurrentHP(int32 CurrentHP);

	/**
	 * Set the IVs for the Pokémon that should be overridden
	 * @param IVs The IVs that are being overridden
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder *IVs(const TMap<FName, int32>& IVs);

	/**
	 * Set the IVs for the Pokémon that should be overridden
	 * @param IVs The IVs that are being overridden
	 * @return A reference to this object
	 */
	UPokemonBuilder *IVs(TMap<FName, int32>&& IVs);

	/**
	 * Override an individual ID on the Pokémon
	 * @param Stat The stat to set the IV of
	 * @param IV The value of the IV
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder *IV(FName Stat, int32 IV);

	/**
	 * Set the EVs for the Pokémon that should be overridden
	 * @param EVs The EVs that are being overridden
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder *EVs(const TMap<FName, int32>& EVs);

	/**
	 * Set the EVs for the Pokémon that should be overridden
	 * @param EVs The EVs that are being overridden
	 * @return A reference to this object
	 */
	UPokemonBuilder *EVs(TMap<FName, int32>&& EVs);

	/**
	 * Override an individual ID on the Pokémon
	 * @param Stat The stat to set the EV of
	 * @param EV The value of the EV
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder *EV(FName Stat, int32 EV);

	/**
	 * Set the Nature of the Pokémon in question
	 * @param Nature The Nature of the Pokémon in question
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder *Nature(FName Nature);

	/**
	 * Set the ability of the Pokémon
	 * @param AbilityIndex The index of the Pokémon's ability
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder *Ability(int32 AbilityIndex);

	/**
	 * Set the moves of the Pokémon
	 * @param Moves The IDs of the Pokémon's moves
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder *Moves(const TArray<FName> &Moves);

	/**
	 * Set the moves of the Pokémon
	 * @param Moves The IDs of the Pokémon's moves
	 * @return A reference to this object
	 */
	UPokemonBuilder *Moves(TArray<FName> &&Moves);

	/**
	 * Set the held item of the Pokémon
	 * @param Item The ID of the item to hold
	 * @return A reference to this object
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon|Builder")
	UPokemonBuilder *Item(FName Item);
	
	/**
	 * Get the underlying Data Transfer Object used for construction
	 * @return The DTO in question
	 */
	const FPokemonDTO& GetDTO() const;

	/**
	 * Use the currently set data to build the Pokémon
	 * @return The created Pokémon
	 */
	TSharedRef<IPokemon> Build() const;

	/**
	 * Use the currently set data to build the Pokémon
	 * @param ClassName The specific class to build
	 * @return The created Pokémon
	 */
	TSharedRef<IPokemon> Build(FName ClassName) const;

private:
	/**
	 * The internal DTO used for the actual object construction
	 */
	UPROPERTY()
	FPokemonDTO DTO;
};

#define ADD_OPTIONAL(Builder, PropertyName) if (PropertyName.IsSet()) (Builder).PropertyName(PropertyName.GetValue())