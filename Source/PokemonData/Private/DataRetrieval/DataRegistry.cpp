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
#include "DataRetrieval/DataRegistry.h"

FDataRegistry::FDataRegistry() = default;

FDataRegistry::~FDataRegistry() = default;

FDataRegistry& FDataRegistry::GetInstance() {
	static FDataRegistry DataRegistry;
	return DataRegistry;
}

TUniquePtr<IGameData> FDataRegistry::CreateDataTableProxy(const UScriptStruct* StructType,
                                                          const TObjectPtr<UDataTable>& DataTable) const {
	check(StructType != nullptr);
	auto TypeName = StructType->GetFName();

	check(RegisteredConstructors.Contains(TypeName));
	return RegisteredConstructors[TypeName](DataTable);
}

bool FDataRegistry::IsTypeRegistered(const UScriptStruct* StructType) const {
	check(StructType != nullptr);
	auto TypeName = StructType->GetFName();
	return RegisteredConstructors.Contains(TypeName);
}
