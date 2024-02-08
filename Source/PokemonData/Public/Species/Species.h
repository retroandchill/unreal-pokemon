//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
#pragma once

#include "CoreMinimal.h"
#include "EvolutionMethod.h"
#include "LevelUpMove.h"
#include "UObject/Object.h"
#include "Species.generated.h"

/**
 * Represents an individual species object
 */
USTRUCT(BlueprintType, meta = (DatabaseType="PBS"))
struct POKEMONDATA_API FSpecies : public FTableRowBase {
	GENERATED_BODY()

	/**
	 * The internal ID used for lookup by the game
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FName ID;

	/**
	 * The name that is displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FText RealName;

	/**
	 * The name of this particular form that is displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FText FormName;

	/**
	 * The types this Pokémon can have
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Species Information")
	TArray<FName> Types;

	/**
	 * The base stats of this particular species
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	TMap<FName, int32> BaseStats;

	/**
	 * Determines a Pokémon's chance of being Male/Female
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Species Information")
	FName GenderRatio;

	/**
	 * The rate at which this Pokémon levels up from gaining Exp.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	FName GrowthRate;

	/**
	 * The base amount of Exp this species gives when defeated/caught
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
	int32 BaseExp;

	/**
	 * The EVs this species gives when defeated/caught
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	TMap<FName, uint8> EVs;

	/**
	 * The catch rate of this species
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Species Information")
	uint8 CatchRate;

	/**
	 * The base happiness of this species when they are caught
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Species Information")
	uint8 Happiness;

	/**
	 * The normal abilities this Pokémon can have
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<FName> Abilities;

	/**
	 * The hidden abilities this Pokémon can have
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<FName> HiddenAbilities;

	/**
	 * Moves learned up leveling up
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves")
	TArray<FLevelUpMove> Moves;

	/**
	 * Moves that can be learned TM/HM/TR or through a Move Tutor
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves")
	TArray<FName> TutorMoves;

	/**
	 * Moves that can be learned through breeding or by means of a Mirror Herb
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moves")
	TArray<FName> EggMoves;

	/**
	 * Methods of evolution that are possible for this species
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Evolution")
	TArray<FEvolutionMethod> Evolution;

	/**
	 * What breeding egg groups is this Pokémon a part of
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding")
	TArray<FName> EggGroups;

	/**
	 * On average how many steps does this Pokémon take to hatch?
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding", meta = (UIMin = 1, ClampMin = 1))
	int32 HatchSteps = 1;

	/**
	 * Any Incense items used for breeding?
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding")
	FName Incense;

	/**
	 * The species can this Pokémon produce?
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding")
	TArray<FName> Offspring;

	/**
	 * The category as displayed to the player on the Pokédex screen
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex")
	FText Category;

	/**
	 * The description for this species as displayed in the Pokédex
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex")
	FText PokedexEntry;

	/**
	 * This Pokémon's height in meters (m)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float Height;

	/**
	 * This Pokémon's weight in kilograms (kg)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float Weight;

	/**
	 * The color of this Pokémon (used for Pokédex filtering)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex")
	FName Color;

	/**
	 * The body shape of this Pokémon (used for Pokédex filtering)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex")
	FName Shape;

	/**
	 * The habitat of this Pokémon (used for Pokédex filtering)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pokédex")
	FName Habitat;

	/**
	 * Item(s) held 100% of the time
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Held Items")
	TArray<FName> WildItemCommon;

	/**
	 * Item(s) held 50% of the time
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Held Items")
	TArray<FName> WildItemUncommon;

	/**
	 * Item(s) held 5% of the time
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Held Items")
	TArray<FName> WildItemRare;

	/**
	 * The generation this Pokémon was introduced in
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata", meta = (UIMin = 1, ClampMin = 1))
	int32 Generation;

	/**
	 * Words/phrases that can be used to group the data"
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
	TArray<FName> Tags;
};
