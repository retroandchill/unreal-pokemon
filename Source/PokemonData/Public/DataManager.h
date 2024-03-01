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
//====================================================================================================================#pragma once

#include "CoreMinimal.h"
#include "DataRetrieval/DataTableProxy.h"

/**
 * A singleton class used to retrieve the game data when needed
 */
class POKEMONDATA_API FDataManager {
	FDataManager();
	~FDataManager();

	FDataManager(const FDataManager &) = delete;
	FDataManager(FDataManager &&) = delete;

	FDataManager &operator=(const FDataManager&) = delete;
	FDataManager &operator=(FDataManager&&) = delete;

public:
	/**
	 * Get the singleton instance of the Data Manager
	 * @return A reference to the data manager
	 */
	static FDataManager &GetInstance();
	
	/**
	 * Get the data table that contains data of the specified type
	 * @tparam T The type to look up the table for
	 * @return A reference to the table proxy object
	 */
	template <typename T>
	const TDataTableProxy<T>& GetDataTable() const {
		UScriptStruct* StructClass = T::StaticStruct();
		auto RowName = StructClass->GetFName();
		check(DataTables.Contains(RowName));

		auto TableInterface = DataTables[RowName].Get();
		check(StructClass->GetName() == TableInterface->GetStructType()->GetName())

		auto TableOut = static_cast<TDataTableProxy<T>*>(TableInterface);
		return *TableOut;
	}

	/**
	 * Get the data table that contains data of the specified type
	 * @param StructType The type to look up the table for
	 * @return A reference to the table proxy object
	 */
	const IGameData& GetDataTable(TObjectPtr<const UScriptStruct> StructType) const;

private:
	/**
	 * The list of data tables in the game
	 */
	TMap<FName, TUniquePtr<IGameData>> DataTables;
};
