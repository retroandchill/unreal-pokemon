// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RetroLib/Concepts/Structs.h"
#include <memory>

#include "DataStructHandle.generated.h"

namespace Pokemon::Data {
    /**
     * Concept for a struct that is used for a data proxy.
     * @tparam T The type to verify the struct status against
     */
    template <typename T>
    concept DataStruct = Retro::UEStruct<T> && std::is_base_of_v<FIndexedTableRow, T>;

    /**
     * Concept for a data struct handle type
     * @tparam T The type that we want to check
     */
    template <typename T>
    concept DataStructHandle = DataStruct<typename std::remove_cvref_t<T>::FValueType> && requires(T &&Struct) {
        { Struct.RowID } -> std::convertible_to<FName>;
    };

    constexpr auto DataStructRowID = TEXT("RowID");

#if WITH_METADATA
    POKEMONDATA_API bool IsValidDataTableStruct(const UScriptStruct *Struct);
#endif

    /**
     * Speciality class to handle the destruction of a struct created using reflection
     */
    struct POKEMONDATA_API FStructDestructor {

        /**
         * Default constructor, won't do anything if invoked
         */
        FStructDestructor() = default;

        /**
         * Initialize a destructor for the given struct type.
         * @param StructClass The struct type in question
         */
        explicit FStructDestructor(UScriptStruct *StructClass) : StructClass(StructClass) {
        }

        /**
         * Operator used to free the underlying struct type
         * @param Struct The struct to free
         */
        template <typename T>
        void operator()(T *Struct) const {
            if (Struct != nullptr && StructClass != nullptr) {
                StructClass->DestroyStruct(StructClass);
                FMemory::Free(Struct);
            }
        }

        /**
         * Get the struct class that needs to be destroyed
         * @return The struct class that needs to be destroyed
         */
        UScriptStruct *GetStruct() const {
            return StructClass;
        }

      private:
        TObjectPtr<UScriptStruct> StructClass;
    };

    /**
     * Wrapper type for initializing a struct by its underlying type.
     */
    struct POKEMONDATA_API FStructWrapper {

        /**
         * Default constructor. Points to nullptr if used.
         */
        FStructWrapper() = default;

        /**
         * Create a new wrapper from the underlying struct type.
         * @param StructClass The type of struct to initialize
         */
        explicit FStructWrapper(UScriptStruct *StructClass)
            : Struct(FMemory::Malloc(StructClass->GetStructureSize()), FStructDestructor(StructClass)) {
            StructClass->InitializeStruct(Struct.get());
        }

        /**
         * Assignment operator from the underlying struct class, destruction will happen automatically due to the unique
         * ptr.
         * @param StructClass The type of struct to initialize
         * @return A reference to this object
         */
        FStructWrapper &operator=(UScriptStruct *StructClass) {
            Struct = std::unique_ptr<void, FStructDestructor>(FMemory::Malloc(StructClass->GetStructureSize()),
                                                              FStructDestructor(StructClass));
            StructClass->InitializeStruct(Struct.get());
            return *this;
        }

        /**
         * Get the underlying struct data.
         * @return The underlying struct data
         */
        template <typename T>
            requires DataStructHandle<T>
        T *Get() const {
            return static_cast<T *>(Struct.get());
        }

        /**
         * Get the struct class that is being wrapped
         * @return The struct class that is being wrapped
         */
        UScriptStruct *GetStruct() const {
            return Struct.get_deleter().GetStruct();
        }

#if WITH_METADATA
        TArray<TSharedPtr<FString>> GetStructOptions() const;
#endif

        FName GetRowID() const;

        void SetRowID(FName RowID);

        FString ExportText() const;
        void FromExportString(FStringView ExportString, int32 PortFlags = PPF_None);

      private:
        std::unique_ptr<void, FStructDestructor> Struct;
    };
} // namespace Pokemon::Data

USTRUCT(BlueprintType, BlueprintInternalUseOnly)
struct POKEMONDATA_API FDataStructHandle {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FName RowID;
};

UCLASS()
class POKEMONDATA_API UDataStructHandleUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = DataHandles, CustomThunk, meta = (CustomStructureParam = DataHandle))
    static bool NotEqual_HandleHandle(const FDataStructHandle &DataHandle, FName Other);
    DECLARE_FUNCTION(execNotEqual_HandleHandle);
};

#define DECLARE_DATA_HANDLE(ClassName, StructType)                                                                     \
  public:                                                                                                              \
    using FValueType = StructType;                                                                                     \
    ClassName() = default;                                                                                             \
    ClassName(FName RowID) : RowID(RowID) {                                                                            \
    }                                                                                                                  \
    ClassName(const ANSICHAR *RowID) : RowID(RowID) {                                                                  \
    }                                                                                                                  \
    ClassName(const WIDECHAR *RowID) : RowID(RowID) {                                                                  \
    }                                                                                                                  \
    ClassName(const UTF8CHAR *RowID) : RowID(RowID) {                                                                  \
    }                                                                                                                  \
    operator FName() const {                                                                                           \
        return RowID;                                                                                                  \
    }                                                                                                                  \
    friend uint32 GetTypeHash(const ClassName &Key) {                                                                  \
        return GetTypeHash(Key.RowID);                                                                                 \
    }                                                                                                                  \
    friend bool operator==(const ClassName &LHS, const ClassName &RHS) {                                               \
        return LHS.RowID == RHS.RowID;                                                                                 \
    }                                                                                                                  \
    friend bool operator==(const ClassName &LHS, const FName &RHS) {                                                   \
        return LHS.RowID == RHS;                                                                                       \
    }                                                                                                                  \
    friend bool operator==(const FName &LHS, const ClassName &RHS) {                                                   \
        return LHS == RHS.RowID;                                                                                       \
    }
