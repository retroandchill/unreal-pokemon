// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Asserts.h"

/**
 * Thin wrapper around FDataTableRowHandle used to allow it to behave like a pointer
 * @tparam T The row that is being pointed to
 */
template <typename T>
requires std::is_base_of_v<FTableRowBase, T>
class TRowPointer {
public:
	/**
	 * Create a pointer with the given data table and row name
	 * @param DataTable The data table that contains the row
	 * @param RowName The contained row name
	 */
	TRowPointer(const UDataTable &DataTable, FName RowName) {
		TableRowHandle.DataTable = &DataTable;
		TableRowHandle.RowName = RowName;
	}

	/**
	 * Get a raw reference to the pointed object
	 * @return The pointed object in question
	 */
	T* Get() const {
		return TableRowHandle.GetRow<T>(TEXT("Could not find row!"));
	}

	/**
	 * Dereference the pointed table
	 * @return The pointed row
	 */
	T& operator*() const {
		auto Row = TableRowHandle.GetRow<T>(TEXT("Could not find row!"));
		ASSERT(Row != nullptr)
		return *Row;
	}

	/**
	 * Dereference the pointed table
	 * @return The pointed row
	 */
	T* operator->() const {
		auto Row = TableRowHandle.GetRow<T>(TEXT("Could not find row!"));
		ASSERT(Row != nullptr)
		return Row;
	}

	/**
	 * Overloaded operator to allow comparisons between this data struct
	 * @param Other The pointed object
	 * @return Are the two pointers the same
	 */
	bool operator==(const TRowPointer& Other) const {
		return TableRowHandle.RowName == Other.TableRowHandle.RowName;
	}

	/**
	 * Overloaded operator to allow comparisons to raw pointers
	 * @param Other The pointed object
	 * @return Are the two pointers the same
	 */
	bool operator==(T* Other) const {
		return Get() == Other;
	}

private:
	/**
	 * The wrapped row handle used to actually retrieve the data
	 */
	FDataTableRowHandle TableRowHandle;
};
