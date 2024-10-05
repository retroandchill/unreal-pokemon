// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Algorithm/FindFirst.h"
#include "Ranges/Concepts/Structs.h"
#include "Ranges/Concepts/UObjectPointer.h"
#include "Ranges/Views/Enumerate.h"
#include "Ranges/Views/FilterTuple.h"

namespace UE::Ranges {

    template <typename... T>
        requires ((std::is_base_of_v<UObject, T> || UnrealInterface<T>) && ...)
    struct TVariantObject;

    template <typename T>
    concept VariantObjectStruct = UEStruct<T> && requires(T &&Object) {
        []<typename... A>(TVariantObject<A...>&&){}(Forward<T>(Object));
    };
    
    template <typename T>
        requires VariantObjectStruct<T>
    class TVariantObjectCustomization;

    /**
     * Specialized variant type that handles a subset of UObject subclasses and interfaces.
     * @tparam T The types that are valid
     */
    template <typename... T>
        requires ((std::is_base_of_v<UObject, T> || UnrealInterface<T>) && ...)
    struct TVariantObject {
        static constexpr bool bHasIntrusiveUnsetOptionalState = true;

        /**
         * Default construct, creates a variant object that exists within an invalid state. Only to be used as a
         * default constructor. If passed into a TOptional it will be considered unset.
         */
        TVariantObject() = default;
        
        /**
         * Construct a new object from the given object
         * @tparam U The type of object to construct from
         * @param Object The object to construct from
         */
        template <typename U>
            requires UObjectPointer<U> && (std::same_as<T, U> || ...)
        explicit TVariantObject(U* Object) : ContainedObject(Object), TypeIndex(GetTypeIndex<U>()) {
            check(Object != nullptr)
        }

        /**
         * Construct a new object from the given interface
         * @tparam U The type of interface to construct from
         * @param Object The interface to construct from
         */
        template <typename U>
            requires UnrealInterface<U> && (std::same_as<T, U> || ...)
        explicit TVariantObject(const TScriptInterface<U>& Object) : ContainedObject(Object.GetObject()), TypeIndex(GetTypeIndex<U>()) {
            check(Object != nullptr)
        }

        /**
         * Attempt to construct a new object from the given object. will raise a fatal error if the wrong type.
         * @param Object The object to construct from
         */
        explicit TVariantObject(UObject* Object) : ContainedObject(Object), TypeIndex(GetTypeIndex(Object)) {
        }

        /**
         * Attempt to construct a new object from the given interface, will raise a fatal error if the wrong type.
         * @param Object The inteface to construct from
         */
        explicit TVariantObject(const FScriptInterface& Object) : ContainedObject(Object.GetObject()), TypeIndex(GetTypeIndex(Object.GetObject())) {
        }

        virtual ~TVariantObject() = default;

        /**
         * Access any of the members on UObject, regardless of the underlying type.
         * @return The underlying object
         */
        UObject* operator->() const {
            return ContainedObject;
        }

        /**
         * Dereference the underlying UObject, regardless of the underlying type.
         * @return The underlying object
         */
        UObject& operator*() const {
            return *ContainedObject;
        }

        /**
         * Check if the variant is of the given type
         * @tparam U The underlying type to check against
         * @return Is this the given type
         */
        template <typename U>
            requires (std::same_as<T, U> || ...)
        bool IsType() const {
            return TypeIndex == GetTypeIndex<U>();
        }

    protected:
        /**
         * Get the raw TObjectPtr object, used mainly for linking this type to the garbage collector if needed
         * @return The raw object pointer
         */
        TObjectPtr<UObject>& GetObjectPtr() {
            return ContainedObject;
        }

        /**
         * Get the raw TObjectPtr object, used mainly for linking this type to the garbage collector if needed
         * @return The raw object pointer
         */
        const TObjectPtr<UObject>& GetObjectPtr() const {
            return ContainedObject;
        }

    public:
        /**
         * Get a reference to the held value, will raise a fatal error if the wrong type.
         * @tparam U The type of the value
         * @return 
         */
        template <typename U>
            requires (std::same_as<T, U> || ...)
        U& Get() const {
            return *CastChecked<U>(ContainedObject);
        }

        UObject& Get() const {
            check(IsValid(ContainedObject));
            return *ContainedObject;
        }

        template <typename U>
            requires std::same_as<std::nullptr_t, U> || (std::same_as<T, U> || ...)
        TOptional<U&> TryGet() const {
            return Optionals::OfNullable(Cast<U>(ContainedObject));
        }

        TOptional<UObject&> TryGet() const {
            return Optionals::OfNullable(ContainedObject);
        }
        
        template <typename U>
            requires std::same_as<std::nullptr_t, U> || (std::same_as<T, U> || ...)
        static constexpr uint64 GetTypeIndex() {
            constexpr std::array<bool, sizeof...(T) + 1> TypesMatch = { std::same_as<U, std::nullptr_t>, std::same_as<U, T>... };
            auto Find = ranges::find_if(TypesMatch, [](bool Matches) { return Matches; });
            check(Find != TypesMatch.end())
            return std::distance(TypesMatch.begin(), Find);
        }

        static uint64 GetTypeIndex(const UObject* Object) {
            auto TypeCheck = []<typename U>(const UObject* O) { return IsValidType<U>(O); };
            constexpr std::array TypeChecks = {
                &TVariantObject::IsValidType<std::nullptr_t>,
                &TVariantObject::IsValidType<T>...
            };
            auto Find = ranges::find_if(TypeChecks, [Object](auto&& Callback) { return Callback(Object); });
            check(Find != TypeChecks.end())
            return std::distance(TypeChecks.begin(), Find);
        }

        uint64 GetTypeIndex() const {
            return TypeIndex;
        }

        template <typename U>
            requires UObjectPointer<U> && (std::same_as<T, U> || ...)
        void Set(U* Object) {
            ContainedObject = Object;
            TypeIndex = GetTypeIndex<U>();
        }

        template <typename U>
            requires UnrealInterface<U> && (std::same_as<T, U> || ...)
        void Set(const TScriptInterface<U>& Object) {
            ContainedObject = Object.GetObject();
            TypeIndex = GetTypeIndex<U>();
        }

        void Set(UObject* Object) {
            ContainedObject = Object;
            TypeIndex = GetTypeIndex(Object);
        }

        void Set(const FScriptInterface& Object) {
            ContainedObject = Object.GetObject();
            TypeIndex = GetTypeIndex(Object.GetObject());
        }

        bool operator==(const TVariantObject& Other) const {
            return TypeIndex == Other.TypeIndex && ContainedObject == Other.ContainedObject;
        }

        virtual bool Serialize(FArchive& Ar) {
            Ar << ContainedObject << TypeIndex;
            return true;
        }

    private:
        explicit TVariantObject(FIntrusiveUnsetOptionalState) : ContainedObject(nullptr) {}

        TVariantObject& operator=(FIntrusiveUnsetOptionalState) {
            ContainedObject = nullptr;
            TypeIndex = GetTypeIndex<std::nullptr_t>();
            return *this;
        }
        
        template <typename U>
            requires std::same_as<std::nullptr_t, U> || (std::same_as<T, U> || ...)
        static constexpr bool IsValidType(const UObject* Object) {
            if constexpr (std::is_same_v<std::nullptr_t, U>) {
                return Object == nullptr;
            } else if constexpr (UnrealInterface<U>) {
                return Object->Implements<typename U::UClassType>();
            } else {
                return Object->IsA<U>();
            }
        }

        friend struct TOptional<TVariantObject>;
        friend class TVariantObjectCustomization<T>;
        
        TObjectPtr<UObject> ContainedObject;
        uint64 TypeIndex = GetTypeIndex<std::nullptr_t>();
    };
}
    
    
    