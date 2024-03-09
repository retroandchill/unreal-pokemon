// "Unreal Pokémon" created by Retro & Chill.
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
template <typename T>
	requires std::is_base_of_v<UObject, T>
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
template <typename T>
	requires std::is_base_of_v<UObject, T>
TSharedPtr<T> MakeSharedRoot(T* Obj) {
	Obj->AddToRoot();
	return TSharedPtr<T>(Obj, &RemoveObjectFromRoot);
}
