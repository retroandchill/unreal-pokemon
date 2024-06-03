// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "BattleUse.h"
#include "FieldUse.h"
#include "IndexedTableRow.h"

#include "Item.generated.h"

/**
 * Represents the data for an Item
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "PBS"))
struct POKEMONDATA_API FItem : public FIndexedTableRow {
    GENERATED_BODY()

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
    UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (GetOptions = "PokemonData.ItemHelper.GetPocketNames"))
    FName Pocket;

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
    UPROPERTY(DisplayName = "BP Price", BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 1, UIMin = 1))
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
    UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (GetOptions = "PokemonData.MoveHelper.GetMoveNames"))
    FName Move;

    /**
     * Description of this item.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FText Description;

    FItem();

    /**
     * Determine if this item is a TM?
     * @return If this item is a TM
     */
    bool IsTM() const;

    /**
     * Determine if this item is an HM?
     * @return If this item is a HM
     */
    bool IsHM() const;

    /**
     * Determine if this item is a TR?
     * @return If this item is a TR
     */
    bool IsTR() const;

    /**
     * Get if this item is a Poké Ball or not.
     * @return Is the item in question a type of Poké Ball?
     */
    bool IsPokeBall() const;

    /**
     * Get if this item is a form of mail or not.
     * @return Is this item of form of mail?
     */
    bool IsMail() const;

    /**
     * Is this item a Key Item?
     * @return Is this item a Key Item?
     */
    bool IsKeyItem() const;

    /**
     * Determine if this item is important at all?
     * @return Is this item important?
     */
    bool IsImportant() const;

    /**
     * Determine if the item quantity should be shown?
     * @return Should the quantity be shown?
     */
    bool ShouldShowQuantity() const;
};

/**
 * Blueprint function library for getting item data out.
 */
UCLASS()
class POKEMONDATA_API UItemHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible item names.
     * @return The list of all possible item names.
     */
    UFUNCTION()
    static TArray<FName> GetItemNames();

    /**
     * Get the list of all possible Poké Ball names.
     * @return The list of all possible Poké Ball names.
     */
    UFUNCTION()
    static TArray<FName> GetPokeBallNames();

    /**
     * Get the names of the pockets used by the game
     * @return The names of the pockets
     */
    UFUNCTION()
    static TArray<FName> GetPocketNames();

    /**
     * Get if an item is a form of mail or not.
     * @param Item The item in question
     * @return Is the item of form of mail?
     */
    UFUNCTION(BlueprintPure, Category = Items)
    static bool IsMail(const FItem &Item);
};

/**
 * Thin wrapper around a Pocket name, that forces the user to select a pocket name.
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FPocketKey {
    GENERATED_BODY()

    /**
     * The name of the pocket.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, meta = (GetOptions = "PokemonData.ItemHelper.GetPocketNames"))
    FName PocketName;
};

/**
 * Function used to get the type hash of the pocket key, making it identical to the wrapped property.
 * @param Key The key structure
 * @return The return type in question
 */
inline uint32 GetTypeHash(const FPocketKey& Key) {
    return GetTypeHash(Key.PocketName);
}