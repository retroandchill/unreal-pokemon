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

    template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    constexpr bool TypesMatch(const UClass *Class) {
        if (Class == nullptr) {
            return false;
        }

        return TypesMatch<T>(*Class);
    }

    template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    constexpr bool TypesMatch(const FSoftClassPath &Path) {
        return TypesMatch<T>(Path.TryLoadClass<UObject>());
    }

    UNREALRANGES_API bool TypesMatch(const UObject& Object, const UClass& Class);

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

    template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    constexpr bool IsValidSubclass(const UClass* Class) {
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

} // namespace UE::Ranges