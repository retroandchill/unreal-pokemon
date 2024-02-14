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
#include "GridBased2DEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Charset/CharsetAssetActions.h"

constexpr auto GLoctextNamespace = "FGridBased2DEditorModule";

void FGridBased2DEditorModule::StartupModule() {
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FGridBased2DEditorModule::OnPostEngineInit);
}

void FGridBased2DEditorModule::OnPostEngineInit() {
	// Register asset types
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	auto Paper2DAssetCategoryBit = AssetTools.FindAdvancedAssetCategory(FName(TEXT("Paper2D")));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FCharsetAssetActions(Paper2DAssetCategoryBit)));
}

void FGridBased2DEditorModule::ShutdownModule() {
    
}
    
IMPLEMENT_MODULE(FGridBased2DEditorModule, GridBased2DEditor)