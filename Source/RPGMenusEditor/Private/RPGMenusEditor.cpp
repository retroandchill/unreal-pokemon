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
#include "RPGMenusEditor.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Data/Windowskin.h"
#include "Windowskin/WindowskinAssetActions.h"
#include "Windowskin/WindowskinThumbnailRenderer.h"

constexpr auto GLoctextNamespace = "FRPGMenusEditorModule";

void FRPGMenusEditorModule::StartupModule() {
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FRPGMenusEditorModule::OnPostEngineInit);
}

void FRPGMenusEditorModule::OnPostEngineInit() {
	// Register asset types
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    
	AssetTools.RegisterAssetTypeActions(MakeShared<FWindowskinAssetActions>());

	// Register thubmnails
	UThumbnailManager::Get().RegisterCustomRenderer(UWindowskin::StaticClass(), UWindowskinThumbnailRenderer::StaticClass());
}

void FRPGMenusEditorModule::ShutdownModule() {
    // No special shutdown needed
}
    
IMPLEMENT_MODULE(FRPGMenusEditorModule, RPGMenusEditor)