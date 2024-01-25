// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleUse.h"
#include "FieldUse.h"
#include "Item.generated.h"

/**
 * Represents the data for an Item
 */
USTRUCT(BlueprintType)
struct  POKEMONDATA_API FItem : public FTableRowBase 
{
	GENERATED_BODY()
	
	/**
	 * ID of this item
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName ID;

	/**
	 * Name of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
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
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Price;

	/**
	 * Sell price of this item. If blank, is half the purchase price.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 SellPrice;

	/**
	 * Purchase price of this item in Battle Points (BP).
	 */
	UPROPERTY(DisplayName="BP Price", BlueprintReadOnly, EditAnywhere)
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
