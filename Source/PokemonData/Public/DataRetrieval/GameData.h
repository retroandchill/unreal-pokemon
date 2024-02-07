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
#include "UObject/Interface.h"

/**
 * Basic interface to manage getting data from a data table
 */
class POKEMONDATA_API IGameData {
public:
	virtual ~IGameData();
	
	/**
	 * Get the type of struct this proxy points to
	 * @return The type of struct contained within the table
	 */
	virtual UStruct* GetStructType() const = 0;

	/**
  * Get the specified row from the data table
  * @param ID he ID to get the data from
  * @return The retrieved row from the database
  */
	virtual FTableRowBase* GetData(FName ID) const = 0;

	/**
  * Get the list of IDs in the table
  * @return A unique set of row names
  */
	virtual TSet<FName> GetTableRowNames() const = 0;

	/**
  * Check if the provided row name is valid or not
  * @param ID The ID to check against
  * @return If there is a row defined with the provided ID
  */
	virtual bool IsRowNameValid(FName ID) const = 0;
};
