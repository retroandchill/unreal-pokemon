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

/**
 * Signifies a generic static registry for different data types. Used to create a static registry that sits below
 * Unreal Engine and doesn't require the classes to be UObjects
 */
template <typename T, typename ...Args>
class TPokeRegistry {
public:
	/**
	 * Factory function that produces a unique pointer to referenced type
	 */
	using FFactoryFunction = TFunction<TUniquePtr<T>(Args...)>;

	/**
	 * Register the given class for the given key using the default constructor
	 * @param Key The key to use for the registry
	 */
	template<typename Derived>
	void RegisterClass(FName Key) {
		RegisterFactory(Key, ConstructDerived<Derived>);
	}

	/**
	 * Register the given method as a factory function for the given key
	 * @param Key The key to use for the registry
	 * @param FactoryFunction 
	 */
	void RegisterFactory(FName Key, const FFactoryFunction &FactoryFunction) {
		RegisteredConstructors.Add(Key, FactoryFunction);
	}

	/**
	 * Construct an instance of the given class
	 * @param Key The key to look up the constructor for
	 * @param Arguments The arguments to pass to the factory function to
	 * @return A unique reference to the factory instance
	 */
	TUniquePtr<T> Construct(FName Key, Args... Arguments) const {
		check(RegisteredConstructors.Contains(Key));
		return RegisteredConstructors[Key](Arguments...);
	}

	/**
	 * Get if the type in question is registered in as a valid constructor
	 * @param Key The for the type in question
	 * @return Is the type registered?
	 */
	bool IsTypeRegistered(FName Key) const {
		return RegisteredConstructors.Contains(Key);
	}

private:
	template<typename Derived>
	static TUniquePtr<T> ConstructDerived(Args... Arguments) {
		return MakeUnique<Derived>(Arguments...);
	}
	
	/**
	 * The set of registered constructors for the data table proxy types
	 */
	TMap<FName, FFactoryFunction> RegisteredConstructors;
};
