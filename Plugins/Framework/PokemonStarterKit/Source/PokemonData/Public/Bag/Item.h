// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "BattleUse.h"
#include "DataRetrieval/DataStructHandle.h"
#include "FieldUse.h"
#include "GameplayTagContainer.h"
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
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealNamePlural;

    /**
     * Name of a portion of this item as displayed by the game.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealPortionName;

    /**
     * Name of 2 or more portions of this item as displayed by the game.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealPortionNamePlural;

    /**
     * Pocket in the Bag where this item is stored.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "BagInfo",
              meta = (GetOptions = "PokemonData.ItemHelper.GetPocketNames"))
    FName Pocket;

    /**
     * Purchase price of this item.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Marts", meta = (ClampMin = 0, UIMin = 0))
    int32 Price;

    /**
     * Sell price of this item. If blank, is half the purchase price.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Marts", meta = (ClampMin = 0, UIMin = 0))
    int32 SellPrice;

    /**
     * Purchase price of this item in Battle Points (BP).
     */
    UPROPERTY(DisplayName = "BP Price", BlueprintReadOnly, Category = "Marts", EditAnywhere,
              meta = (ClampMin = 1, UIMin = 1))
    int32 BPPrice;

    /**
     * How this item can be used outside of battle.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Usage)
    EFieldUse FieldUse;

    /**
     * How this item can be used within a battle.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Usage)
    EBattleUse BattleUse;

    /**
     * Defines the categories that the items can belong to
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Usage, meta = (Categories = "Battle.Items.BattleUse"))
    FGameplayTagContainer BattleUsageCategories;

    /**
     * Words/phrases that can be used to group certain kinds of items."
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Metadata)
    TArray<FName> Tags;

    /**
     * Whether this item is consumed after use.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Usage)
    bool Consumable;

    /**
     * Whether the Bag shows how many of this item are in there.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Display)
    bool ShowQuantity;

    /**
     * Move taught by this HM, TM or TR.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Usage,
              meta = (GetOptions = "PokemonData.MoveHelper.GetMoveNames"))
    FName Move;

    /**
     * Description of this item.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText Description;

    FItem();

    /**
     * Get the name given to a portion of an item (when obtained or held)
     * @return The text to display
     */
    const FText &GetPortionName() const;

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

    /**
     * Can the item in question be held?
     * @return Can the item in question be held?
     */
    bool CanHold() const;
};

USTRUCT(BlueprintType, meta = (DisableSplitPin))
struct POKEMONDATA_API FItemHandle {
    GENERATED_BODY()
    DECLARE_DATA_HANDLE(FItemHandle, FItem)

    /**
     * The ID of the row in question.
     */
    UPROPERTY(EditAnywhere, meta = (GetOptions = "PokemonData.ItemHelper.GetItemNames"))
    FName RowID;
};

/**
 * Thin wrapper around a Pocket name, that forces the user to select a pocket name.
 */
USTRUCT(BlueprintType, meta = (DisableSplitPin))
struct POKEMONDATA_API FPocketKey {
    GENERATED_BODY()

    /**
     * The name of the pocket.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data,
              meta = (GetOptions = "PokemonData.ItemHelper.GetPocketNames"))
    FName PocketName;

    FString ExportText() const;
    void FromExportString(FStringView ExportString, int32 PortFlags = PPF_None);
};

/**
 * Function used to get the type hash of the pocket key, making it identical to the wrapped property.
 * @param Key The key structure
 * @return The return type in question
 */
inline uint32 GetTypeHash(const FPocketKey &Key) {
    return GetTypeHash(Key.PocketName);
}

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
     * Get the name given to a portion of an item (when obtained or held)
     * @param Item The item in question
     * @return The text to display
     */
    UFUNCTION(BlueprintPure, Category = Items)
    static const FText &GetPortionName(const FItem &Item);

    /**
     * Get if an item is a form of mail or not.
     * @param Item The item in question
     * @return Is the item of form of mail?
     */
    UFUNCTION(BlueprintPure, Category = Items)
    static bool IsMail(const FItem &Item);

    /**
     * Can the item in question be held?
     * @param Item The item in question
     * @return Can the item in question be held?
     */
    UFUNCTION(BlueprintPure, Category = Items)
    static bool CanHold(const FItem &Item);

    /**
     * Determine if the item quantity should be shown?
     * @param Item The item in question
     * @return Should the quantity be shown?
     */
    UFUNCTION(BlueprintPure, Category = Items)
    static bool ShouldShowQuantity(const FItem &Item);

    UFUNCTION(BlueprintPure, Category = ItemHandle,
              meta = (DisplayName = "Convert To Name", CompactNodeTitle = "->", BlueprintAutocast,
                      AutoCreateRefTerm = Struct))
    static FName ConvertItemHandleToName(const FItemHandle &Struct);

    UFUNCTION(BlueprintPure, Category = Name,
              meta = (DisplayName = "Convert To Item Handle", CompactNodeTitle = "->", BlueprintAutocast))
    static FItemHandle ConvertNameToItemHandle(FName Name);
};