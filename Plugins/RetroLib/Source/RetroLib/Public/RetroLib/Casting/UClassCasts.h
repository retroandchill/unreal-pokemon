// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include "UObject/Object.h"
#endif

#include "RetroLib/Casting/InstanceOf.h"
#include "RetroLib/Concepts/Interfaces.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
	/**
     * Cast the underlying object of an interface into the specified object type. Will return nullptr if the cast is
     * invalid.
     * @tparam T The type to cast to.
     * @param Interface The interface to perform the cast on
     * @return The result of the cast.
     */
    RETROLIB_EXPORT template <typename T>
        requires std::is_base_of_v<UObject, T>
    T *CastInterface(const FScriptInterface &Interface) {
        return Cast<T>(Interface.GetObject());
    }

    /**
     * Cast the underlying object of an interface into the specified object type. Will assert that the cast does not
     * result in a null.
     * @tparam T The type to cast to.
     * @param Interface The interface to perform the cast on
     * @return The result of the cast.
     */
    RETROLIB_EXPORT template <typename T>
        requires std::is_base_of_v<UObject, T>
    T *CastInterfaceChecked(const FScriptInterface &Interface) {
        return CastChecked<T>(Interface.GetObject());
    }

    /**
     * Checks if the provided UClass instance matches the specified type T.
     *
     * @tparam T The type to compare against.
     * @param Class The UClass instance to be checked.
     * @return A boolean indicating whether the class associated with the UClass instance matches the type T.
     *         If T is derived from UObject, returns whether Class is a child of T.
     *         If T conforms to UnrealInterface, returns whether Class is a child of T::UClassType.
     */
    RETROLIB_EXPORT template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    constexpr bool TypesMatch(const UClass &Class) {
        if constexpr (std::derived_from<T, UObject>) {
            return Class.IsChildOf<T>();
        } else {
            static_assert(UnrealInterface<T>);
            return Class.IsChildOf<typename T::UClassType>();
        }
    }

    /**
     * Checks if the provided UClass pointer refers to a class matching the specified type T.
     *
     * @tparam T The type to compare against.
     * @param Class The UClass pointer to be checked.
     * @return A boolean indicating whether the class referred to by the pointer matches the specified type T.
     *         Returns false if the Class pointer is nullptr.
     *         Otherwise, delegates the check to the TypesMatch<T>(const UClass&) function.
     */
    RETROLIB_EXPORT template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    constexpr bool TypesMatch(const UClass *Class) {
        if (Class == nullptr) {
            return false;
        }

        return TypesMatch<T>(*Class);
    }

    /**
     * Checks if the provided FSoftClassPath refers to a class that matches the specified type T.
     *
     * @param Path The FSoftClassPath object representing the class path to be checked.
     * @return A boolean indicating whether the class referred to by the Path matches the specified type T.
     */
    RETROLIB_EXPORT template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    constexpr bool TypesMatch(const FSoftClassPath &Path) {
        return TypesMatch<T>(Path.TryLoadClass<UObject>());
    }

    /**
     * Checks if the provided UObject instance is of the specified UClass type.
     *
     * @param Object The UObject instance to check.
     * @param Class The UClass type to compare against.
     * @return A boolean indicating whether the Object is of the specified Class type.
     *         If the Class has the CLASS_Interface flag, returns whether the Object implements the interface.
     *         Otherwise, returns whether the Object is of the specified Class type.
     */
	RETROLIB_EXPORT inline bool TypesMatch(const UObject &Object, const UClass &Class) {
    	if (Class.HasAnyClassFlags(CLASS_Interface)) {
    		return Object.GetClass()->ImplementsInterface(&Class);
    	}

    	return Object.IsA(&Class);
    }

    /**
     * Retrieves the UClass object for a given type T.
     *
     * This function determines the appropriate UClass object based on whether the type T is
     * derived from UObject or conforms to the UnrealInterface concept.
     *
     * @tparam T The type for which to get the UClass.
     * @return The UClass object corresponding to type T.
     *         If T is derived from UObject, returns T::StaticClass().
     *         If T conforms to UnrealInterface, returns T::UClassType::StaticClass().
     */
    RETROLIB_EXPORT template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    constexpr UClass *GetClass() {
        if constexpr (std::derived_from<T, UObject>) {
            return T::StaticClass();
        } else {
            static_assert(UnrealInterface<T>);
            return T::UClassType::StaticClass();
        }
    }

    /**
     * Checks if the given class is a valid subclass of the specified superclass.
     *
     * @tparam T The name of the superclass to be checked against.
     * @param Class The class to be checked.
     * @return A boolean indicating whether className is a valid subclass of superClassName.
     */
    RETROLIB_EXPORT template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    constexpr bool IsValidSubclass(const UClass *Class) {
        if constexpr (std::derived_from<T, UObject>) {
            return Class->IsChildOf<T>();
        } else {
            static_assert(UnrealInterface<T>);
            if (Class->HasAnyClassFlags(CLASS_Interface)) {
                return Class->IsChildOf<typename T::UClassType>();
            }

            return Class->ImplementsInterface(T::UClassType::StaticClass());
        }
    }

    /**
     * Checks if the given UClass is instantiable.
     *
     * @param Class The UClass instance to check.
     * @return A boolean indicating whether the Class is instantiable.
     *         Returns false if the ClassName starts with "SKEL_" or "REINST_".
     */
	RETROLIB_EXPORT inline bool IsInstantiableClass(const UClass *Class) {
    	auto ClassName = Class->GetName();
    	ClassName.ToUpperInline();
    	// We don't want skeleton or reinstantiated classes to be loaded.
    	if (ClassName.StartsWith("SKEL_") || ClassName.StartsWith("REINST_")) {
    		return false;
    	}

    	return true;
    }
	
	RETROLIB_EXPORT template <std::derived_from<UObject> T>
	struct TInstanceChecker<T> {
		/**
		 * Checks if the given instance of type U is a valid instance of the desired base type T.
		 *
		 * @param Value The instance of type U to be checked for validity.
		 * @return True if the instance is valid, meaning that U is derived from T or is an instance
		 *         that can be dynamically cast to T. Otherwise, returns false.
		 */
		template <std::derived_from<UObject> U>
		constexpr bool operator()(const U& Value) const {
			if constexpr (std::derived_from<U, T>) {
				// Trivial case, U is derived from T, so we know with certainty that this is valid
				return true;
			} else {
				return Value.template IsA<T>();
			}
		}

		template <UnrealInterface U>
		constexpr bool operator()(const U& Value) const {
			if constexpr (std::derived_from<U, T>) {
				// Trivial case, U is derived from T, so we know with certainty that this is valid
				return true;
			} else {
				check(Value._getUObject() != nullptr);
				return Value._getUObject()->template IsA<T>();
			}
		}
	};

	RETROLIB_EXPORT template <UnrealInterface T>
	struct TInstanceChecker<T> {
		/**
		 * Checks if the given instance of type U is a valid instance of the desired base type T.
		 *
		 * @param Value The instance of type U to be checked for validity.
		 * @return True if the instance is valid, meaning that U is derived from T or is an instance
		 *         that can be dynamically cast to T. Otherwise, returns false.
		 */
		template <std::derived_from<UObject> U>
		constexpr bool operator()(const U& Value) const {
			if constexpr (std::derived_from<U, T>) {
				// Trivial case, U is derived from T, so we know with certainty that this is valid
				return true;
			} else {
				return Value.template Implements<typename T::UClassType>();
			}
		}

		template <UnrealInterface U>
		constexpr bool operator()(const U& Value) const {
			if constexpr (std::derived_from<U, T>) {
				// Trivial case, U is derived from T, so we know with certainty that this is valid
				return true;
			} else {
				check(Value._getUObject() != nullptr);
				return Value._getUObject()->template Implements<typename T::UClassType>();
			}
		}
	};
}
#endif