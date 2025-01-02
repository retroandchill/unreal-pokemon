// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Exceptions/InvalidArgumentException.h"
#include "RetroLib/Utils/Unreachable.h"

#if !RETROLIB_WITH_MODULES
#include <array>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
	/**
     * Concept used to check that a property get actually get a value out using a static method.
     *
     * @tparam T The type to validate the concept against.
     */
    template <typename T>
    concept CanGetValue = requires(T &&Property, const void *Data) { T::GetPropertyValue(Data); };

    /**
     * @class TPropertyVisitor
     *
     * @brief A templated class designed to visit and manipulate properties in Unreal Engine.
     *
     * This class is constructed using a specific property type and provides methods to retrieve type indices and
     * property values.
     *
     * @tparam T A list of property types derived from FProperty.
     */
    template <typename... T>
        requires((std::derived_from<T, FProperty> && CanGetValue<T>) && ...)
    class TPropertyVisitor {
      public:
        /**
         * Constructs a TPropertyVisitor with the provided property, initializing the TypeIndex.
         *
         * @param Property A pointer to the FProperty object to be used for initializing the visitor.
         * @return An instance of TPropertyVisitor with the TypeIndex set based on the provided property.
         * @throws FInvalidArgumentException if the property type is not found within the predefined set of classes.
         */
        explicit TPropertyVisitor(const FProperty *Property) : TypeIndex(GetTypeIndex(Property)) {
        }

        /**
         * Retrieves the index of the property type from a predefined set of classes.
         *
         * @param Property A pointer to the FProperty object whose type index is to be determined.
         * @return The index of the property type if found within the predefined set of classes.
         * @throws FInvalidArgumentException if the property type is not found within the predefined set of classes.
         */
        static size_t GetTypeIndex(const FProperty *Property) {
            static std::array Classes = {T::StaticClass()...};
            auto ValidClass =
                std::ranges::find_if(Classes, [Property](const FFieldClass *Class) { return Property->IsA(Class); });
            if (ValidClass == Classes.end()) {
                throw FInvalidArgumentException("Invalid property type!");
            }

            return std::distance(Classes.begin(), ValidClass);
        }

        template <typename F>
        constexpr static decltype(auto) Visit(const FProperty *Property, F &&Functor) {
            constexpr std::array Invocations = {&TPropertyVisitor::VisitProperty<T, F>...};
            return std::invoke(Invocations[GetTypeIndex(Property)], Property, std::forward<F>(Functor));
        }

        /**
         * Invokes the appropriate VisitSingle method based on the TypeIndex for the given data and functor.
         *
         * @param Data A pointer to the data to be passed to the VisitSingle method.
         * @param Functor The functor to be invoked with the data.
         * @return The result of invoking the VisitSingle method specified by the TypeIndex with the given data and
         * functor.
         * @tparam F The signature of the functional callback
         */
        template <typename F>
        constexpr decltype(auto) Visit(const uint8 *Data, F &&Functor) {
            constexpr std::array Invocations = {&TPropertyVisitor::VisitSingle<T, F>...};
            return std::invoke(Invocations[TypeIndex], Data, std::forward<F>(Functor));
        }

      private:
        template <typename U, typename F>
            requires(std::same_as<T, U> || ...)
        constexpr static decltype(auto) VisitSingle(const uint8 *Data, F &&Functor) {
            return std::invoke(std::forward<F>(Functor), U::GetPropertyValue(Data));
        }

        template <typename U, typename F>
            requires(std::same_as<T, U> || ...)
        constexpr static decltype(auto) VisitProperty(const FProperty *Property, F &&Functor) {
            return std::invoke(std::forward<F>(Functor), static_cast<const U *>(Property));
        }

        size_t TypeIndex;
    };

    /**
     * Retrieves a UObject pointer from a given property and data.
     *
     * This method visits the provided property and attempts to extract a UObject from the associated data.
     *
     * @param Property A pointer to the FProperty from which the object is to be extracted.
     * @param Data A pointer to the raw data from which the property value will be interpreted.
     * @return A UObject pointer extracted from the provided property and data.
     */
    RETROLIB_API UObject *GetObjectFromProperty(const FProperty *Property, const uint8 *Data);

    /**
     * Assigns a UObject to a given property and data.
     *
     * This function visits the provided property and assigns the specified UObject to it using the associated data.
     *
     * @param Property A pointer to the FProperty to which the object will be assigned.
     * @param Data A pointer to the raw data representing the property value.
     * @param Object A pointer to the UObject that will be assigned to the property.
     */
    RETROLIB_API void AssignObjectToProperty(const FProperty *Property, uint8 *Data, UObject *Object);

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
    RETROLIB_API void SetObject(const FObjectProperty *ObjectProperty, uint8 *Storage, UObject *Object);

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
    RETROLIB_API void SetObject(const FInterfaceProperty *ObjectProperty, uint8 *Storage, UObject *Object);

    /**
     * Visitor for checking either an object or an interface property.
     */
    using FObjectPropertyVisitor = TPropertyVisitor<FObjectProperty, FInterfaceProperty>;
}
#endif