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
#include "DataRetrieval/GameData.h"
#include "DataRetrieval/DataTableProxy.h"
#include "Meta/PokeRegistry.h"

/**
 * Internal registry used to handle the tracking of the data table structs
 */
class POKEMONDATA_API FDataRegistry {
	FDataRegistry();
	~FDataRegistry();

public:
	/**
	 * Factory function that produces a unique pointer to the proxy type
	 */
	using FFactoryFunction = TFunction<TUniquePtr<IGameData>(UDataTable*)>;

	/**
	 * The internal registry used to actually retrieve the objects.
	 * @return The registry in question to retrieve
	 */
	using FInternalRegistry = TPokeRegistry<IGameData, UDataTable*>;

	/**
	 * Get the singleton instance of the class
	 * @return A reference to the only instance of this class
	 */
	static FDataRegistry& GetInstance();

	/**
	 * Register the constructor of the given type
	 * @tparam T The type that the proxy references
	 * @return The registration succeeded
	 */
	template <typename T>
	bool RegisterStruct() {
		UScriptStruct* TypeStruct = T::StaticStruct();
		Registry.RegisterClass<TDataTableProxy<T>>(TypeStruct->GetFName());
		return true;
	}

	/**
	 * Create a data table proxy of the specified type
	 * @param StructType The type that the proxy references
	 * @param DataTable The data table asset this proxy points to
	 * @return A unique reference to the proxy
	 */
	TUniquePtr<IGameData> CreateDataTableProxy(const UScriptStruct* StructType,
	                                           const TObjectPtr<UDataTable>& DataTable) const;

	/**
	 * Get if the type in question is registered in as a valid constructor
	 * @param StructType The struct type in question
	 * @return Is the type registered?
	 */
	bool IsTypeRegistered(const UScriptStruct* StructType) const;

private:
	/**
	 * The internal registry type that handles this particular registration
	 */
	FInternalRegistry Registry;
};
