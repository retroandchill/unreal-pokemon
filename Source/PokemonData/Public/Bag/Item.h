// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleUse.h"
#include "FieldUse.h"

/**
 * Represents the data for an Item
 */
USTRUCT(BlueprintType)
struct  POKEMONDATA_API FItem : public FDataTableRowHandle
{
	/**
	 * ID of this item
	 */
	UPROPERTY(BlueprintReadOnly)
	FName ID;

	/**
	 * Name of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly)
	FText RealName;

	/**
	 * Plural name of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly)
	FText RealNamePlural;

	/**
	 * Name of a portion of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly)
	FText RealPortionName;

	/**
	 * Name of 2 or more portions of this item as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly)
	FText RealPortionNamePlural;

	/**
	 * Pocket in the Bag where this item is stored.
	 */
	UPROPERTY(BlueprintReadOnly)
	uint8 Pocket;

	/**
	 * Purchase price of this item.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 Price;

	/**
	 * Sell price of this item. If blank, is half the purchase price.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 SellPrice;

	/**
	 * Purchase price of this item in Battle Points (BP).
	 */
	UPROPERTY(DisplayName="BP Price", BlueprintReadOnly)
	int32 BPPrice;

	/**
	 * How this item can be used outside of battle.
	 */
	UPROPERTY(BlueprintReadOnly)
	EFieldUse FieldUse;

	/**
	 * How this item can be used within a battle.
	 */
	UPROPERTY(BlueprintReadOnly)
	EBattleUse BattleUse;

	/**
	 * Words/phrases that can be used to group certain kinds of items."
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<FName> Tags;

	/**
	 * Whether this item is consumed after use.
	 */
	UPROPERTY(BlueprintReadOnly)
	bool Consumable;

	/**
	 * Whether the Bag shows how many of this item are in there.
	 */
	UPROPERTY(BlueprintReadOnly)
	bool ShowQuanity;

	/**
	 * Move taught by this HM, TM or TR.
	 */
	UPROPERTY(BlueprintReadOnly)
	FName Move;

	/**
	 * Description of this item.
	 */
	UPROPERTY(BlueprintReadOnly)
	FText Description;
	
};
