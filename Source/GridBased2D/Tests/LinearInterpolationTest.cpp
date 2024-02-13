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
#include "GridUtils.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LinearInterpolationTest, "Tests.LinearInterpolationTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool LinearInterpolationTest::RunTest(const FString& Parameters) {
	// Make the test pass by returning true, or fail by returning false.
	bool Passed = TestEqual("Negative Duration", 70.0, GridBased2D::LinearInterpolation(30, 70, -3, 20));
	Passed &= TestEqual("Negative Delta", 30.0, GridBased2D::LinearInterpolation(30, 70, 20, -8));
	Passed &= TestEqual("Delta Past Duration", 70.0, GridBased2D::LinearInterpolation(30, 70, 20, 30));
	Passed &= TestEqual("Delta Within Duration", 50.0, GridBased2D::LinearInterpolation(30, 70, 20, 10));
	return Passed;
}
