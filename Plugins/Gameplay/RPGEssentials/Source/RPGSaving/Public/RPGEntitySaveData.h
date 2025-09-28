// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "StructUtils/StructView.h"
#include "UObject/Object.h"

#include "RPGEntitySaveData.generated.h"

class URPGComponent;
class URPGEntity;

USTRUCT(BlueprintType, meta = (UseProperties))
struct FRPGComponentSaveData final
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Version = 1;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSubclassOf<URPGComponent> ComponentClass;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    FInstancedStruct Data;

    FRPGComponentSaveData() = default;
    explicit FRPGComponentSaveData(const TSubclassOf<URPGComponent> ComponentClass, const int32 Version = 0)
        : Version(Version), ComponentClass(ComponentClass)
    {
    }
};

USTRUCT(BlueprintType)
struct FRPGComponentSaveDataHandle
{
    GENERATED_BODY()

    FRPGComponentSaveDataHandle() = default;
    explicit FRPGComponentSaveDataHandle(TSubclassOf<URPGComponent> ComponentClass, int32 Version = 0)
        : Data(MakeShared<FRPGComponentSaveData>(ComponentClass, Version))
    {
    }

    int32 GetVersion() const
    {
        return Data->Version;
    }

    TSubclassOf<URPGComponent> GetComponentClass() const
    {
        return Data->ComponentClass;
    }

    FInstancedStruct &GetData() const
    {
        return Data->Data;
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    bool Serialize(const FStructuredArchive::FSlot Ar)
    {
        static FRPGComponentSaveData Defaults;
        FRPGComponentSaveData::StaticStruct()->SerializeItem(Ar, &Data.Get(), &Defaults);
        return true;
    }

  private:
    TSharedRef<FRPGComponentSaveData> Data = MakeShared<FRPGComponentSaveData>();
};

template <>
struct TStructOpsTypeTraits<FRPGComponentSaveDataHandle> : TStructOpsTypeTraitsBase2<FRPGComponentSaveDataHandle>
{
    enum
    {
        WithStructuredSerializer = true,
        WithNetSerializer = false,
        WithNetSharedSerialization = false
    };
};

USTRUCT(BlueprintType, meta = (Readonly, RecordStruct, UseProperties))
struct FAdditionalComponentDataKey
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSubclassOf<URPGComponent> ComponentClass;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Index = 0;

    FAdditionalComponentDataKey() = default;

    explicit FAdditionalComponentDataKey(const TSubclassOf<URPGComponent> ComponentClass, const int32 Index = 0)
        : ComponentClass(ComponentClass), Index(Index)
    {
    }

    bool operator==(const FAdditionalComponentDataKey &) const = default;

    FORCEINLINE friend uint32 GetTypeHash(const FAdditionalComponentDataKey &Data)
    {
        return HashCombine(GetTypeHash(Data.ComponentClass), GetTypeHash(Data.Index));
    }
};

/**
 *
 */
USTRUCT(BlueprintType, meta = (UseProperties))
struct FRPGEntitySaveData final
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, SaveGame)
    int32 Version = 1;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TSubclassOf<URPGEntity> EntityClass;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<TFieldPath<FObjectProperty>, FRPGComponentSaveDataHandle> RequiredComponents;

    UPROPERTY(BlueprintReadWrite, SaveGame)
    TMap<FAdditionalComponentDataKey, FRPGComponentSaveDataHandle> AdditionalComponents;

    FRPGEntitySaveData() = default;
    explicit FRPGEntitySaveData(const TSubclassOf<URPGEntity> EntityClass, const int32 Version = 0)
        : Version(Version), EntityClass(EntityClass)
    {
    }
};

USTRUCT(BlueprintType)
struct RPGSAVING_API FRPGEntitySaveDataHandle
{
    GENERATED_BODY()

    FRPGEntitySaveDataHandle() = default;
    explicit FRPGEntitySaveDataHandle(TSubclassOf<URPGEntity> EntityClass, int32 Version = 0)
        : Data(MakeShared<FRPGEntitySaveData>(EntityClass, Version))
    {
    }

    int32 GetVersion() const
    {
        return Data->Version;
    }

    TSubclassOf<URPGEntity> GetEntityClass() const
    {
        return Data->EntityClass;
    }

    TMap<TFieldPath<FObjectProperty>, FRPGComponentSaveDataHandle> &GetRequiredComponents() const
    {
        return Data->RequiredComponents;
    }

    TMap<FAdditionalComponentDataKey, FRPGComponentSaveDataHandle> &GetAdditionalComponents() const
    {
        return Data->AdditionalComponents;
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    bool Serialize(FArchive &Ar)
    {
        static FRPGEntitySaveData Defaults;
        FRPGEntitySaveData::StaticStruct()->SerializeItem(Ar, &Data.Get(), &Defaults);
        return true;
    }

  private:
    TSharedRef<FRPGEntitySaveData> Data = MakeShared<FRPGEntitySaveData>();
};

template <>
struct TStructOpsTypeTraits<FRPGEntitySaveDataHandle> : TStructOpsTypeTraitsBase2<FRPGEntitySaveDataHandle>
{
    enum
    {
        WithSerializer = true,
        WithNetSerializer = false,
        WithNetSharedSerialization = false
    };
};