// "Unreal Pokémon" created by Retro & Chill.

#include "Saving/Serialization/EnhancedSaveGame.h"

bool UEnhancedSaveGame::AddObjectToSaveGame(const FGameplayTag &Tag, const TScriptInterface<ISerializable> &Object)
{
    if (Data.Contains(Tag))
    {
        return false;
    }

    Data.Emplace(Tag, FObjectData::SerializeObject(Object.GetObject()));
    return true;
}

UObject *UEnhancedSaveGame::LoadObjectFromSaveGame(const TSubclassOf<UObject> &Class, const FGameplayTag &Tag) const
{
    auto SerializedObject = Data.Find(Tag);
    if (SerializedObject == nullptr)
    {
        return nullptr;
    }

    if (!Class->IsChildOf(SerializedObject->ObjectClass))
    {
        return nullptr;
    }

    return SerializedObject->DeserializeObject();
}

bool UEnhancedSaveGame::LoadDataIntoObject(const FGameplayTag &Tag,
                                           const TScriptInterface<ISerializable> &TargetObject) const
{
    auto SerializedObject = Data.Find(Tag);
    if (!IsValid(TargetObject.GetObject()) || SerializedObject == nullptr)
    {
        return false;
    }

    if (!TargetObject.GetObject()->IsA(SerializedObject->ObjectClass))
    {
        return false;
    }

    SerializedObject->DeserializeObject(TargetObject.GetObject());
    return true;
}