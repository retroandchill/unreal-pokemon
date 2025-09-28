// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Structs/UnrealStruct.h"
#include "UObject/Object.h"
#include <bit>

#include "BattleEffectData.generated.h"

USTRUCT()
struct FEnumStorage
{
    GENERATED_BODY()

    FEnumStorage() = default;
    explicit FEnumStorage(UEnum *Enum, const uint8 Value = 0) : Value(Value), NativeEnum(Enum)
    {
    }

    bool Serialize(FStructuredArchive::FSlot Ar);

  private:
    UPROPERTY()
    uint8 Value = 0;

    UPROPERTY()
    UEnum *NativeEnum = nullptr;
};

template <>
struct TStructOpsTypeTraits<FEnumStorage> : TStructOpsTypeTraitsBase2<FEnumStorage>
{
    enum
    {
        WithStructuredSerializer = true,
    };
};

USTRUCT()
struct FSmallStructBuffer
{
    GENERATED_BODY()

    static constexpr size_t MaxSize = 16;

    FSmallStructBuffer() = default;

    uint8 *GetData()
    {
        return Storage.data();
    }

  private:
    friend struct FSmallStructStorage;
    std::array<uint8, MaxSize> Storage;
};

USTRUCT()
struct POKEMONBATTLE_API FSmallStructStorage
{
    GENERATED_BODY()

    static constexpr size_t MaxSize = 16;

    FSmallStructStorage() = default;

    template <UEStruct T>
        requires(std::is_trivial_v<T> && (sizeof(T) <= MaxSize))
    explicit FSmallStructStorage(T &&Struct) : NativeStruct(GetScriptStruct<T>())
    {
        std::construct_at(std::bit_cast<T *>(std::addressof(Storage), Forward<T>(Struct)));
    }

  private:
    explicit FSmallStructStorage(UScriptStruct *Struct, const uint8 *Data);

  public:
    static TOptional<FSmallStructStorage> Create(UScriptStruct *Struct, const uint8 *Data = nullptr);

    bool Serialize(FStructuredArchive::FSlot Ar);

  private:
    UPROPERTY()
    FSmallStructBuffer Storage;

    UPROPERTY()
    UScriptStruct *NativeStruct = nullptr;
};

template <>
struct TStructOpsTypeTraits<FSmallStructStorage> : TStructOpsTypeTraitsBase2<FSmallStructStorage>
{
    enum
    {
        WithStructuredSerializer = true,
    };
};

UENUM()
enum class EBattleEffectDataType : uint8
{
    None,
    Bool,
    Int32,
    Float,
    Name,
    Enum,
    SmallStruct
};

/**
 *
 */
USTRUCT()
struct POKEMONBATTLE_API FBattleEffectData
{
    GENERATED_BODY()

    FBattleEffectData() = default;

    bool Serialize(FStructuredArchive::FSlot Ar);

  private:
    union FData {
        std::monostate None;
        bool BoolValue;
        int32 Int32Value;
        float FloatValue;
        FName NameValue;
        FEnumStorage EnumValue;
        FSmallStructStorage SmallStructValue;
    };

    FData Data = {.None = std::monostate()};
    EBattleEffectDataType Type = EBattleEffectDataType::None;
};

template <>
struct TStructOpsTypeTraits<FBattleEffectData> : TStructOpsTypeTraitsBase2<FBattleEffectData>
{
    enum
    {
        WithStructuredSerializer = true,
    };
};