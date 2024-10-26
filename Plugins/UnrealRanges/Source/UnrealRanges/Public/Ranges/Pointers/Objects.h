// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Blueprints/Properties.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Utilities/Unreachable.h"

namespace UE::Ranges {
    /**
     * Retrieves a UObject pointer from a given property and data.
     *
     * This method visits the provided property and attempts to extract a UObject from the associated data.
     *
     * @param Property A pointer to the FProperty from which the object is to be extracted.
     * @param Data A pointer to the raw data from which the property value will be interpreted.
     * @return A UObject pointer extracted from the provided property and data.
     */
    UNREALRANGES_API UObject *GetObjectFromProperty(const FProperty *Property, const uint8 *Data);

    /**
     * Assigns a UObject to a given property and data.
     *
     * This function visits the provided property and assigns the specified UObject to it using the associated data.
     *
     * @param Property A pointer to the FProperty to which the object will be assigned.
     * @param Data A pointer to the raw data representing the property value.
     * @param Object A pointer to the UObject that will be assigned to the property.
     */
    UNREALRANGES_API void AssignObjectToProperty(const FProperty *Property, uint8 *Data, UObject *Object);

    /**
     * Concept for any pointer wrapper type that has a .Get() method that returns a pointer to a UObject.
     */
    template <typename T>
    concept CanGetObject = requires(T &&Object) {
        { Object.Get() } -> std::convertible_to<UObject *>;
    };

    /**
     * Retrieves a UObject pointer from a given object, interface, or pointer wrapper.
     *
     * This method attempts to extract or convert the provided object to a UObject pointer depending on the type of the
     * object.
     *
     * @tparam T The type of object pointer that is being passed through
     * @param Object The object from which the UObject pointer is to be retrieved. This can be an actual UObject
     * pointer, a pointer wrapper type that has a .Get() method, or an Unreal Interface type.
     * @return A UObject pointer extracted or converted from the provided object.
     */
    template <typename T>
        requires std::convertible_to<T, UObject *> || CanGetObject<T> ||
                 std::derived_from<std::remove_cvref_t<T>, FScriptInterface>
    UObject *GetObject(T &&Object) {
        if constexpr (std::convertible_to<T, UObject *>) {
            return std::forward<T>(Object);
        } else if constexpr (CanGetObject<T>) {
            return Object.Get();
        } else if constexpr (std::derived_from<std::remove_cvref_t<T>, FScriptInterface>) {
            return Object.GetObject();
        }

        Unreachable();
    }

    /**
     * Sets the value of a UObject property in the provided storage location.
     *
     * This method assigns a UObject value to a specified storage location using the given FObjectProperty.
     *
     * @param ObjectProperty A pointer to the FObjectProperty defining the property.
     * @param Storage A pointer to the storage where the UObject should be set.
     * @param Object A pointer to the UObject to be set in the storage.
     */
    UNREALRANGES_API void SetObject(const FObjectProperty *ObjectProperty, uint8 *Storage, UObject *Object);

    /**
     * Sets a UObject pointer in the provided storage for a given interface property.
     *
     * This method sets the object for the interface property and updates the internal
     * interface pointer to the native interface address of the provided object.
     *
     * @param ObjectProperty A pointer to the FInterfaceProperty describing the interface type.
     * @param Storage A pointer to the raw memory where the property value is stored.
     * @param Object The UObject to be set for the interface property.
     */
    UNREALRANGES_API void SetObject(const FInterfaceProperty *ObjectProperty, uint8 *Storage, UObject *Object);

    /**
     * Visitor for checking either an object or an interface property.
     */
    using FObjectPropertyVisitor = TPropertyVisitor<FObjectProperty, FInterfaceProperty>;
} // namespace UE::Ranges