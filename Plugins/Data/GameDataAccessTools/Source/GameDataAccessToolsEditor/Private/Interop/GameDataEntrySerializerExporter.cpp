// "Unreal Pokémon" created by Retro & Chill.


#include "Interop/GameDataEntrySerializerExporter.h"
#include "GameDataRepository.h"
#include "Serialization/GameDataEntrySerializer.h"

void UGameDataEntrySerializerExporter::AddToStagingArray(FScriptArray &StagingArray,
                                                         UAssetGameDataRepository *Repository, const uint8 *Data)
{
    const auto* ArrayProperty = UGameDataEntrySerializer::GetGameDataEntriesProperty(Repository);
    FScriptArrayHelper ArrayHelper(ArrayProperty, &StagingArray);

    const int32 NewIndex = ArrayHelper.AddValue();
    auto *NewEntry = ArrayHelper.GetRawPtr(NewIndex);
    Repository->GetEntryStruct()->CopyScriptStruct(NewEntry, Data);
    
}