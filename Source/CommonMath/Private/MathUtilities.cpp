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
#include "MathUtilities.h"

template <typename T>
T LinearInterpolation_Internal(T StartValue, T EndValue, T Duration, T Delta) {
	if (Duration <= 0)
		return EndValue;

	if (Delta <= 0)
		return StartValue;

	if (Delta >= Duration)
		return EndValue;

	return StartValue + (EndValue - StartValue) * Delta / Duration;
}

double UMathUtilities::LinearInterpolation(double StartValue, double EndValue, double Duration, double Delta) {
	return LinearInterpolation_Internal(StartValue, EndValue, Duration, Delta);
}

float UMathUtilities::LinearInterpolationF(float StartValue, float EndValue, float Duration, float Delta) {
	return LinearInterpolation_Internal(StartValue, EndValue, Duration, Delta);
}
