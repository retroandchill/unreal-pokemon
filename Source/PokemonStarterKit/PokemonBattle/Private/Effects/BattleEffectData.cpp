// "Unreal Pokémon" created by Retro & Chill.

#include "Effects/BattleEffectData.h"

bool FEnumStorage::Serialize(FStructuredArchive::FSlot Ar)
{
    const auto EnumAttribute =
        Ar.EnterAttribute(GET_MEMBER_NAME_STRING_VIEW_CHECKED(FEnumStorage, NativeEnum).GetData());
    EnumAttribute << NativeEnum;

    auto ValueAttribute = Ar.EnterAttribute(GET_MEMBER_NAME_STRING_VIEW_CHECKED(FEnumStorage, Value).GetData());
    ValueAttribute << Value;

    return true;
}

FSmallStructStorage::FSmallStructStorage(UScriptStruct *Struct, const uint8 *Data) : NativeStruct(Struct)
{
    Struct->InitializeDefaultValue(Storage.GetData());
    if (Data != nullptr)
    {
        Struct->CopyScriptStruct(Storage.GetData(), Data);
    }
}

TOptional<FSmallStructStorage> FSmallStructStorage::Create(UScriptStruct *Struct, const uint8 *Data)
{
    if (Struct->GetStructureSize() <= MaxSize &&
        (Struct->StructFlags & (STRUCT_IsPlainOldData | STRUCT_NoDestructor)) != 0)
    {
        return FSmallStructStorage(Struct, Data);
    }

    return NullOpt;
}

bool FSmallStructStorage::Serialize(FStructuredArchive::FSlot Ar)
{
    const auto StructAttribute =
        Ar.EnterAttribute(GET_MEMBER_NAME_STRING_VIEW_CHECKED(FSmallStructStorage, NativeStruct).GetData());
    StructAttribute << NativeStruct;
    const auto DataAttribute =
        Ar.EnterAttribute(GET_MEMBER_NAME_STRING_VIEW_CHECKED(FSmallStructStorage, Storage).GetData());
    NativeStruct->SerializeItem(DataAttribute, Storage.GetData(), nullptr);
    return true;
}

bool FBattleEffectData::Serialize(FStructuredArchive::FSlot Ar)
{
    auto TypeAttribute = Ar.EnterAttribute(GET_MEMBER_NAME_STRING_VIEW_CHECKED(FBattleEffectData, Type).GetData());
    TypeAttribute << Type;

    auto DataAttribute = Ar.EnterAttribute(GET_MEMBER_NAME_STRING_VIEW_CHECKED(FBattleEffectData, Data).GetData());
    switch (Type)
    {
    case EBattleEffectDataType::None:
        // Don't serialize anything if a monostate
        break;
    case EBattleEffectDataType::Bool:
        DataAttribute << Data.BoolValue;
        break;
    case EBattleEffectDataType::Int32:
        DataAttribute << Data.Int32Value;
        break;
    case EBattleEffectDataType::Float:
        DataAttribute << Data.FloatValue;
        break;
    case EBattleEffectDataType::Name:
        DataAttribute << Data.NameValue;
        break;
    case EBattleEffectDataType::Enum:
        Data.EnumValue.Serialize(DataAttribute);
        break;
    case EBattleEffectDataType::SmallStruct:
        Data.SmallStructValue.Serialize(DataAttribute);
        break;
    }

    return true;
}
