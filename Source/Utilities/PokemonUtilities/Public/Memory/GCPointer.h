// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Template object for a pointer held by the garbage collector
 * @tparam T The UObject type held by this object
 */
template <typename T>
    requires std::is_base_of_v<UObject, T>
class TGCPointer : public FGCObject {
  public:
    /**
     * Construct an empty pointer with the reference nulled out
     */
    TGCPointer() = default;

    /**
     * Instantiate a new pointer from the given object pointer
     * @param Object The pointer to add as a reference
     */
    explicit TGCPointer(T *Object) : HeldObject(Object) {}

    /**
     * Assign a new pointer into this object
     * @param Object The new object a point to
     * @return A reference to this object
     */
    TGCPointer &operator=(T *Object) {
        HeldObject = Object;
        return *this;
    }

    /**
     * Equality operator directly with the pointed type
     * @param Other The object type being pointed to
     * @return Are the two pointers equal
     */
    bool operator==(T *Other) const { return HeldObject == Other; }

    /**
     * Convert the held pointer to the type held by itself
     */
    explicit operator T() const { return HeldObject; }

    void AddReferencedObjects(FReferenceCollector &Collector) override { Collector.AddReferencedObject(HeldObject); }

    FString GetReferencerName() const override {
        return FString::Format(TEXT("TGCPointer<{ClassName}>"),
                               FStringFormatNamedArguments({{TEXT("ClassName"), T::StaticClass()->GetName()}}));
    }

    /**
     * Get a reference to this object, provided it has not been garbage collected
     * @return The object held by this pointer
     */
    T *Get() const { return HeldObject.Get(); }

    /**
     * Dereferences the held object returning it by reference
     * @return The object held by this pointer
     */
    T &operator*() const { check(HeldObject != nullptr) return *HeldObject; }

    /**
     * Dereferences the held object returning it by reference
     * @return The object held by this pointer
     */
    T *operator->() const { return HeldObject; }

  private:
    TObjectPtr<T> HeldObject;
};
