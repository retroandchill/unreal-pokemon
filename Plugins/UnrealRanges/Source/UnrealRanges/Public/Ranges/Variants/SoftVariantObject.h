// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "VariantObject.h"
#include "Ranges/Pointers/AsyncLoadHandle.h"

namespace UE::Ranges {

    template <typename T>
        requires VariantObject<T>
    struct TSoftVariantObject;
    
    namespace Detail {
        template <typename>
        struct TIsSoftVariantObject : std::false_type {};

        template <typename... T>
        struct TIsSoftVariantObject<TSoftVariantObject<T...>> : std::true_type {};
    }

    template <typename T>
    concept SoftVariantObject = Detail::TIsSoftVariantObject<T>::value;

    template <typename T>
    concept SoftVariantObjectStruct = UEStruct<T> && SoftVariantObject<T>;
    
    template <typename T>
        requires SoftVariantObjectStruct<T>
    class TSoftVariantObjectCustomization;

    
    /**
     * 
     */
    template <typename T>
        requires VariantObject<T>
    struct TSoftVariantObject {
        static constexpr bool bHasIntrusiveUnsetOptionalState = true;
	    using IntrusiveUnsetOptionalStateType = TSoftVariantObject;

        template <typename... A>
        requires std::constructible_from<TSoftObjectPtr<T>, A...>
        explicit TSoftVariantObject(A&&... Args) : Ptr(Forward<A>(Args)...) {
            check(!Ptr.IsNull())
        }

        explicit TSoftVariantObject(const T& Object) : Ptr(&Object.Get()) {}
        
        /**
         * Returns asset name string, leaving off the /package/path part
         * @return the asset name string, leaving off the /package/path part
         */
        FString GetAssetName() const {
            return Ptr.GetAssetName();
        }

        /**
         * Returns /package/path string, leaving off the asset name
         * @return /package/path string, leaving off the asset name
         */
        FString GetLongPackageName() const {
            return Ptr.GetAssetName();
        }

        /**
         * Hash function
         * @return The hash for this type
         */
        uint32 GetPtrTypeHash() const {
            return GetTypeHash(Ptr);
        }

        /**
         * Test if this points to a live UObject
         * @return if this points to a live UObject
         */
        bool IsValid() const {
            return Ptr.IsValid();
        }

        /**
         * Synchronously load (if necessary) and return the asset object represented by this asset ptr, and attempts to
         * cast it to the given type.
         * @tparam U The type to try to cast to
         * @return the asset object represented by this asset ptr
         */
        template <typename U>
            requires (T::template StaticIsValidType<T>())
        TOptional<U &> LoadSynchronous() const {
            return Optionals::OfNullable(Cast<U>(Ptr.LoadSynchronous()));
        }

        /**
         * Synchronously load (if necessary) and return the asset object represented by this asset ptr
         * @return the asset object represented by this asset ptr
         */
        TOptional<T> LoadSynchronous() const {
            auto Result = Ptr.LoadSynchronous();
            if (!T::IsValidType(Result)) {
                return TOptional<T>();
            }

            return TOptional<T>(Result);
        }

        TSharedRef<TAsyncLoadHandle<T>> LoadAsync() const {
            return TAsyncLoadHandle<T>::Create(ToSoftObjectPath());
        }

        /**
         * Returns the StringObjectPath that is wrapped by this SoftObjectPtr
         * @return The StringObjectPath that is wrapped by this SoftObjectPtr
         */
        const FSoftObjectPath &ToSoftObjectPath() const {
            return Ptr.ToSoftObjectPath();
        }

        /**
         * Returns string representation of reference, in form /package/path.assetname
         * @return The string representation of reference, in form /package/path.assetname
         */
        FString ToString() const {
            return Ptr.ToString();
        }

        bool operator==(FIntrusiveUnsetOptionalState) const {
            return Ptr.IsNull();
        }

    private:
        friend struct TOptional<TSoftVariantObject>;

        template <typename U>
            requires SoftVariantObjectStruct<U>
        friend class TSoftVariantObjectCustomization;

        explicit TSoftVariantObject(FIntrusiveUnsetOptionalState) {}

        TSoftVariantObject& operator=(FIntrusiveUnsetOptionalState) {
            Ptr.Reset();
            return *this;
        }
        
        TSoftObjectPtr<> Ptr;
    };
}