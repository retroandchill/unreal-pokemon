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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathUtilities.generated.h"

/**
 * Common utility functions for math
 */
UCLASS()
class COMMONMATH_API UMathUtilities : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Linear interpolation between two values, given the duration of the change and the time passed since the start of
	 * the change (delta)
	 * @param StartValue The starting value for the change
	 * @param EndValue Where the value should be at the end of the duration
	 * @param Duration The duration of the change
	 * @param Delta The change in time
	 * @return The interpolated value
	 */
	UFUNCTION(BlueprintPure, Category = "Utilities|Math")
	static double LinearInterpolation(double StartValue, double EndValue, double Duration, double Delta);

	/**
	 * Linear interpolation between two values, given the duration of the change and the time passed since the start of
	 * the change (delta)
	 * @param StartValue The starting value for the change
	 * @param EndValue Where the value should be at the end of the duration
	 * @param Duration The duration of the change
	 * @param Delta The change in time
	 * @return The interpolated value
	 */
	static float LinearInterpolationF(float StartValue, float EndValue, float Duration, float Delta);
	
};
