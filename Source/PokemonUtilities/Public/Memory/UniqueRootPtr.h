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
 * Contains a unique reference to a UObject and manages its memory through the root to protect it from Garbage Collection.
 * The object is released from the root (and later garbage collected if no UPROPERTY() fields reference it) when this
 * object is destroyed.
 * @tparam T The UObject type this object points to
 */
template <typename T, typename = std::enable_if_t<std::is_base_of_v<UObject, T>>>
class TUniqueRootPtr {
public:
	/**
	 * Instantiates a pointer to null
	 */
	TUniqueRootPtr() = default;

	/**
	 * Instantiates a pointer to the provided argument
	 * @param Ptr The UObject this object it to point to
	 */
	explicit TUniqueRootPtr(T* Ptr) : Pointer(Ptr) {
		if (Pointer != nullptr)
			Pointer->AddToRoot();
	}

	/**
	 * Remove the UObject from the root to allow it to be garbage collected
	 */
	~TUniqueRootPtr() {
		if (Pointer != nullptr)
			Pointer->RemoveFromRoot();
	}

	explicit TUniqueRootPtr(const TUniqueRootPtr<T> &Other) = delete;
	TUniqueRootPtr<T> &operator=(const TUniqueRootPtr<T> &Other) = delete;

	/**
	 * Transfer ownership of the pointer to this new one
	 * @param Other The previous owner of this memory
	 */
	explicit TUniqueRootPtr(TUniqueRootPtr<T>&& Other) : Pointer(Other.Release()) {
	}

	/**
	 * Transfer ownership of the pointer to this new one
	 * @param Other The previous owner of this memory
	 * @return A reference to this object
	 */
	TUniqueRootPtr<T> &operator=(TUniqueRootPtr<T> &&Other) {
		Pointer = Other.Release();
		return *this;
	};

	/**
	 * Reset the value of the pointer to a new value removing the old one from the root
	 * @param Ptr The new pointer vlaue
	 */
	void Reset(T* Ptr) noexcept {
		if (Pointer != nullptr)
			Pointer->RemoveFromRoot();

		Pointer = Ptr;
		if (Pointer != nullptr)
			Pointer->AddToRoot();
	}

	/**
	 * Set the value of this pointer to null without removing the underlying object from root. The value is now the
	 * responsibility of the new owner.
	 * @return The value of the pointer, released to the responsibility of the new owner
	 */
	T* Release() noexcept {
		T* OldPtr = Pointer.Get();
		Pointer = nullptr;
		return OldPtr;
	}

	/**
	 * Dereference the pointer and use it directly
	 * @return An explicit reference to this object
	 */
	T& operator*() const noexcept {
		return *Pointer;
	}

	/**
	 * Dereference the pointer to access its fields/methods
	 * @return A nullable reference to this object
	 */
	T* operator->() const noexcept {
		return Pointer.Get();
	}

	/**
	 * Get the raw pointer contained within the object
	 * @return The raw pointer value contained within
	 */
	T* Get() const noexcept {
		return Pointer.Get();
	}

	/**
	 * Comparison to a raw pointer object
	 * @param Other The raw reference to the other object
	 * @return Are the two pointers the same memory
	 */
	bool operator==(T* Other) const {
		return Pointer.Get() == Other;
	}
	

private:
	/**
	 * Pointer to the UObject instance
	 */
	TObjectPtr<T> Pointer;
};
