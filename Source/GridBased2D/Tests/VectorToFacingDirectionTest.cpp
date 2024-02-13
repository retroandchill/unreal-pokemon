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
#include "Misc/AutomationTest.h"

#include <sstream>

#include "GridUtils.h"
#include "Characters/FacingDirection.h"

IMPLEMENT_COMPLEX_AUTOMATION_TEST(VectorToFacingDirectionTest, "Tests.VectorToFacingDirectionTest",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void VectorToFacingDirectionTest::GetTests(TArray<FString>& OutBeautifiedNames,
                                           TArray<FString>& OutTestCommands) const {
	OutBeautifiedNames.Add("Up");
	OutTestCommands.Add("0 -1 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Up)));
	
	OutBeautifiedNames.Add("Left");
	OutTestCommands.Add("-1 0 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Left)));
	
	OutBeautifiedNames.Add("Right");
	OutTestCommands.Add("1 0 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Right)));
	
	OutBeautifiedNames.Add("Down");
	OutTestCommands.Add("0 1 " + FString::FromInt(static_cast<uint8>(EFacingDirection::Down)));
}

bool VectorToFacingDirectionTest::RunTest(const FString& Parameters) {
	std::stringstream InputStream(TCHAR_TO_UTF8(*Parameters));
	double X;
	double Y;
	int32 DirValue;
	InputStream >> X >> Y >> DirValue;

	auto ExpectedDir = static_cast<EFacingDirection>(DirValue);
	auto ActualDir = GridBased2D::VectorToFacingDirection(FVector2D(X, Y));
	return TestTrue("Direction should be valid!", ActualDir.IsSet()) &&
		TestEqual("Direction should match vector!", ExpectedDir, ActualDir.GetValue());
}
