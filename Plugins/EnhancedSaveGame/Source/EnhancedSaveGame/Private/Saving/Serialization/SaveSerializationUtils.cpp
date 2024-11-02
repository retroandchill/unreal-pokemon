// "Unreal Pokémon" created by Retro & Chill.


#include "Saving/Serialization/SaveSerializationUtils.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

FObjectData FObjectData::SerializeObject(UObject *Object) {
    if (!IsValid(Object)) {
        return FObjectData();
    }

    FObjectData Data = { .ObjectClass = Object->GetClass() };
    FMemoryWriter Writer(Data.Data, true);
    FObjectAndNameAsStringProxyArchive Archive(Writer, true);
    Writer.SetIsSaving(true);
    Object->Serialize(Archive);
    return Data;
}

UObject* FObjectData::DeserializeObject() const {
    if (!IsValid(ObjectClass) || Data.Num() <= 0) {
        return nullptr;
    }
     
    FMemoryReader Reader(Data, true);
    FObjectAndNameAsStringProxyArchive Archive(Reader, true);
    Reader.SetIsLoading(true);

    auto Object = NewObject<UObject>(GetTransientPackage(), ObjectClass);
    Object->Serialize(Archive);
    return Object;
}

FObjectData USaveSerializationUtils::SerializeObject(UObject *Object) {
    return FObjectData::SerializeObject(Object);
}

UObject * USaveSerializationUtils::DeserializeObject(const FObjectData &SerializedData) {
    return SerializedData.DeserializeObject();
}