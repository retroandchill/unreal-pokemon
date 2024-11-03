// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"

namespace UE::Ranges {

    /**
     * Cast the underlying object of an interface into the specified object type. Will return nullptr if the cast is
     * invalid.
     * @tparam T The type to cast to.
     * @param Interface The interface to perform the cast on
     * @return The result of the cast.
     */
    template <typename T>
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
    template <typename T>
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
    template <typename T>
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
    template <typename T>
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
    template <typename T>
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
    UNREALRANGES_API bool TypesMatch(const UObject &Object, const UClass &Class);

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
    template <typename T>
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
    template <typename T>
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
    UNREALRANGES_API bool IsInstantiableClass(const UClass* Class);

} // namespace UE::Ranges