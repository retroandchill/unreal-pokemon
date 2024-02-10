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
 * Remove the specified object from the root
 * @param Ptr The object to remove from the root
 */
inline void RemoveObjectFromRoot(UObject* Ptr) {
	Ptr->RemoveFromRoot();
}

/**
 * Special class that handling removing a UObject from the root
 */
class FRootDeleter {
public:
	FRootDeleter() = default;
	FRootDeleter(const FRootDeleter&) = default;

	/**
	 * Removes the pointed UObject from the root allowing it to be garbage collected
	 * @param Ptr 
	 */
	void operator()(UObject* Ptr) const {
		RemoveObjectFromRoot(Ptr);
	}
};

/**
 * Contains a unique reference to a UObject and manages its memory through the root to protect it from Garbage Collection.
 * The object is released from the root (and later garbage collected if no UPROPERTY() fields reference it) when this
 * object is destroyed.
 * @tparam T The UObject type this object points to
 */
template <typename T>
using TUniqueRootPtr = TUniquePtr<T, FRootDeleter>;

/**
 * Add the specified object to the root and place it inside a specialized Unique Pointer to remove it upon destruction
 * @tparam T The UObject type to make part of the root
 * @param Obj The UObject to add to the root
 * @return A unique reference to the object added to the root
 */
template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<UObject, T>>>
TUniqueRootPtr<T> MakeUniqueRoot(T* Obj) {
	Obj->AddToRoot();
	return TUniqueRootPtr<T>(Obj);
}

/**
 * Add the specified object to the root and place it inside a specialized Unique Pointer to remove it once all shared
 * references have been deleted
 * @tparam T The UObject type to make part of the root
 * @param Obj The UObject to add to the root
 * @return A shared reference to the object added to the root
 */
template<typename T, typename = std::enable_if_t<std::is_base_of_v<UObject, T>>>
TSharedPtr<T> MakeSharedRoot(T* Obj) {
	Obj->AddToRoot();
	return TSharedPtr<T>(Obj, &RemoveObjectFromRoot);
}


