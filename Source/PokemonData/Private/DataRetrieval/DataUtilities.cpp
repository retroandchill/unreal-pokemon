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
#include "DataRetrieval/DataUtilities.h"

void UDataUtilities::GetData(UObject* ContextObject, const UScriptStruct* StructType, FName RowName,
                             FTableRowBase& OutRow) {
	// We should never hit this!  stubs to avoid NoExport on the class.
	check(false);
}

TSet<FName> UDataUtilities::GetAllDataIDs(UObject* ContextObject, const UScriptStruct* StructType) {
	const auto DataSubsystem = Cast<UDataSubsystem>(
			USubsystemBlueprintLibrary::GetGameInstanceSubsystem(ContextObject, UDataSubsystem::StaticClass()));
	check(DataSubsystem != nullptr);
	return DataSubsystem->GetDataTable(StructType).GetTableRowNames();
}

bool UDataUtilities::IsDataRowNameValid(UObject* ContextObject, const UScriptStruct* StructType, FName RowName) {
	const auto DataSubsystem = Cast<UDataSubsystem>(
			USubsystemBlueprintLibrary::GetGameInstanceSubsystem(ContextObject, UDataSubsystem::StaticClass()));
	check(DataSubsystem != nullptr);
	return DataSubsystem->GetDataTable(StructType).IsRowNameValid(RowName);
}
