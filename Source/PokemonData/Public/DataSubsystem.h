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
#include "DataRetrieval/DataTableProxy.h"
#include "DataRetrieval/GameData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataSubsystem.generated.h"

/**
 * Persistent subsystem used to get the data tables
 */
UCLASS(BlueprintType)
class POKEMONDATA_API UDataSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UDataSubsystem();

	/**
	 * Get the data table that contains data of the specified type
	 * @tparam T The type to look up the table for
	 * @return A reference to the table proxy object
	 */
	template <typename T>
	const TDataTableProxy<T> &GetDataTable() const {
		UScriptStruct *StructClass = T::StaticStruct();
		auto RowName = StructClass->GetFName();
		check(DataTables.Contains(RowName));

		auto TableOut = dynamic_cast<TDataTableProxy<T> *>(DataTables[RowName].Get());
		check(TableOut != nullptr);

		return *TableOut;
	}

	/**
	 * Get the data table that contains data of the specified type
	 * @param StructType The type to look up the table for
	 * @return A reference to the table proxy object
	 */
	const IGameData &GetDataTable(TObjectPtr<const UScriptStruct> StructType) const;

private:
	/**
	 * The list of data tables in the game
	 */
	TMap<FName, TUniquePtr<IGameData>> DataTables;
	
};
