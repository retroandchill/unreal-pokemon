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

	virtual UStruct* GetStructType() const override {
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
