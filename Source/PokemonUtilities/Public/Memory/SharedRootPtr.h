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
 * Contains a shared reference to a UObject and manages its memory through the root to protect it from Garbage Collection.
 * The object is released from the root (and later garbage collected if no UPROPERTY() fields reference it) when all
 * TSharedRootPtr references to this class are destroyed
 * @tparam T The UObject type this object points to
 */
template <typename T, typename = std::enable_if_t<std::is_base_of_v<UObject, T>>>
class TSharedRootPtr {
public:
	/**
	 * Create a new shared pointer with a reference to nothing
	 */
	TSharedRootPtr() = default;

	/**
	 * Construct this object from a raw pointer to the object. Adds it to the root and sets the reference counter to 1
	 * @param Ptr The pointer to share
	 */
	explicit TSharedRootPtr(T* Ptr) {
		Pointer = Ptr;
		if (Pointer != nullptr) {
			*ReferenceCount = 1;
		}
	}

	/**
	 * Decrement the reference counter and delete the pointer if the count has hit 0
	 */
	~TSharedRootPtr() {
		CleanUp();
	}

	/**
	 * Create a copy of the pointer and increment the reference counter
	 * @param Other The object to copy from
	 */
	explicit TSharedRootPtr(const TSharedRootPtr<T>& Other) {
		Pointer = Other.Pointer;
		ReferenceCount = Other.ReferenceCount;

		if (Pointer != nullptr) {
			(*ReferenceCount)++;
		}
	}

	/**
	 * Create a copy of the pointer and increment the reference counter
	 * @param Other The object to copy from
	 * @return A reference to this pointer
	 */
	TSharedRootPtr<T>& operator=(const TSharedRootPtr<T>& Other) {
		CleanUp();
		
		Pointer = Other.Pointer;
		ReferenceCount = Other.ReferenceCount;

		if (Pointer != nullptr) {
			(*ReferenceCount)++;
		}
		
		return *this;
	}

	/**
	 * Move the content of this shared pointer object to this one keeping the reference counter the same and nulling
	 * out the contents of the original
	 * @param Other The shared pointer to move the data from
	 */
	explicit TSharedRootPtr(TSharedRootPtr<T> &&Other) noexcept {
		Pointer = Other.Pointer;
		ReferenceCount = Other.ReferenceCount;

		Other.Pointer = nullptr;
		Other.ReferenceCount = nullptr;
	}

	/**
	 * Move the content of this shared pointer object to this one keeping the reference counter the same and nulling
	 * out the contents of the original
	 * @param Other The shared pointer to move the data from
	 * @return A reference to this object
	 */
	TSharedRootPtr<T> &operator=(TSharedRootPtr<T> &&Other) noexcept {
		CleanUp();
		
		Pointer = Other.Pointer;
		ReferenceCount = Other.ReferenceCount;

		Other.Pointer = nullptr;
		Other.ReferenceCount = nullptr;

		return *this;
	}

	/**
	 * Reset the value of the pointer to a new value removing the old one from the root
	 * @param Ptr The new pointer value
	 */
	void Reset(T* Ptr) noexcept {
		CleanUp();

		Pointer = Ptr;
		ReferenceCount = new uint32(1); // NOSONAR
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
	 * Get the current number of references to this pointer
	 * @return The current reference count
	 */
	uint32 GetCount() const {
		return *ReferenceCount;
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
	 * Perform a clean up operation on the current contents of this pointer. Mainly used to handle destruction and
	 * when this pointer is assigned a new value.
	 */
	void CleanUp() {
		(*ReferenceCount)--;
		if (*ReferenceCount == 0) {
			if (Pointer != nullptr)
				Pointer->RemoveFromRoot();

			delete ReferenceCount; // NOSONAR
		}
	}
	
	/**
	 * Pointer to the UObject instance
	 */
	TObjectPtr<T> Pointer;

	/**
	 * A raw pointer to the reference counter this is deleted when this gets decremented to 0 by the destructor
	 */
	uint32 *ReferenceCount = new uint32(0); // NOSONAR
};