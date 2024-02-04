// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameData.h"

/**
 * Proxy class that stores a data table and allows the retrieval of properties from it
 */
template <typename T>
class POKEMONDATA_API TDataTableProxy final : public FGCObject, public IGameData {
public:
 explicit TDataTableProxy(const TObjectPtr<UDataTable>& DataTable) : DataTable(DataTable) {
 }

 explicit TDataTableProxy(TObjectPtr<UDataTable>&& DataTable) : DataTable(std::move(DataTable)) {
 }

 virtual void AddReferencedObjects(FReferenceCollector& Collector) override {
   Collector.AddReferencedObject(DataTable);
 }

 virtual FString GetReferencerName() const override {
  return FString::Printf(TEXT("TDataTableProxy<%s>"), typeid(T).name());
 }

 virtual UStruct *GetStructType() const override {
  return T::StaticStruct();
 }

 virtual T* GetData(FName ID) const override {
  return DataTable->FindRow<T>(ID, "Failed to find the specified data!");
 }

 virtual TSet<FName> GetTableRowNames() const override {
  return TSet(DataTable->GetRowNames());
 }

 virtual bool IsRowNameValid(FName ID) const override {
  return GetTableRowNames().Contains(ID);
 }

private:
 /**
  * A pointer to the data table asset that this proxy object contains
  */
 TObjectPtr<UDataTable> DataTable;
};
