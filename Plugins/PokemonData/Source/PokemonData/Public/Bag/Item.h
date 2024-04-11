// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "BattleUse.h"
#include "CoreMinimal.h"
#include "FieldUse.h"
#include "Item.generated.h"

/**
 * Represents the data for an Item
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "PBS"))
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
};