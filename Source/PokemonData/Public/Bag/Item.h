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
#include "BattleUse.h"
#include "FieldUse.h"
#include "Item.generated.h"

/**
 * Represents the data for an Item
 */
USTRUCT(BlueprintType, meta = (DatabaseType="PBS"))
struct POKEMONDATA_API FItem : public FTableRowBase {
	GENERATED_BODY()

	/**
	 * ID of this item
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FName ID;

	/**
	 * Name of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FText RealName;

	/**
	 * Plural name of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealNamePlural;

	/**
	 * Name of a portion of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealPortionName;

	/**
	 * Name of 2 or more portions of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealPortionNamePlural;

	/**
	 * Pocket in the Bag where this item is stored.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	uint8 Pocket;

	/**
	 * Purchase price of this item.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0, UIMin = 0))
	int32 Price;

	/**
	 * Sell price of this item. If blank, is half the purchase price.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0, UIMin = 0))
	int32 SellPrice;

	/**
	 * Purchase price of this item in Battle Points (BP).
	 */
	UPROPERTY(DisplayName="BP Price", BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 1, UIMin = 1))
	int32 BPPrice;

	/**
	 * How this item can be used outside of battle.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EFieldUse FieldUse;

	/**
	 * How this item can be used within a battle.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EBattleUse BattleUse;

	/**
	 * Words/phrases that can be used to group certain kinds of items."
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> Tags;

	/**
	 * Whether this item is consumed after use.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool Consumable;

	/**
	 * Whether the Bag shows how many of this item are in there.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool ShowQuantity;

	/**
	 * Move taught by this HM, TM or TR.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName Move;

	/**
	 * Description of this item.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Description;

	FItem();
};
