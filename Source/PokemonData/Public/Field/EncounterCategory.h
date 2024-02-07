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

/**
 * Denotes one of the various encounter types that can occur in the world
 */
UENUM(BlueprintType)
enum class EEncounterCategory : uint8 {
	/**
	 * Not any of the specified encounter types. This is typically used for encounters that are resolved by some kind
	 * of scripted event. (e.i. Using a move like Rock Smash or Headbutt)
	 */
	None,

	/**
	 * An encounter on a specific type of terrain such as tall grass
	 */
	Land,

	/**
	 * An encounter that can happen on any tile of the map
	 */
	Cave,

	/**
	 * An encounter that occurs when surfing
	 */
	Water,

	/**
	 * An encounter that occurs when fishing
	 */
	Fishing,

	/**
	 * An encounter that only occurs during some kind of special event such as a Bug-Catching Contest
	 */
	Contest
};
