// "Unreal Pokémon" created by Retro & Chill.
#if WITH_EDITOR
#include "Saving/SaveGameManager/InMemorySaveGameSystem.h"
#include "EnhancedSaveGameModule.h"
#include "OptionalPtr.h"

using namespace EnhancedSaveGame;

bool FInMemorySaveGameSystem::PlatformHasNativeUI()
{
    return false;
}

bool FInMemorySaveGameSystem::DoesSaveSystemSupportMultipleUsers()
{
    return false;
}

bool FInMemorySaveGameSystem::DoesSaveGameExist(const TCHAR *Name, const int32 UserIndex)
{
    return TOptionalPtr(SaveGames.Find(Name))
        .MapToValue(false, [UserIndex](const TMap<int32, TArray<uint8>> *Map) {
               return Map->Contains(UserIndex);
           });
}

ISaveGameSystem::ESaveExistsResult FInMemorySaveGameSystem::DoesSaveGameExistWithResult(const TCHAR *Name,
                                                                                        const int32 UserIndex)
{
    return TOptionalPtr(SaveGames.Find(Name))
        .Map([UserIndex](const TMap<int32, TArray<uint8>> *Map) {
               return Map->Find(UserIndex);
           })
        .MapToValue(ESaveExistsResult::DoesNotExist, [](const TArray<uint8> *) {
               return ESaveExistsResult::OK;
           });
}

bool FInMemorySaveGameSystem::SaveGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex,
                                       const TArray<uint8> &Data)
{
    UE_LOG(LogEnhancedSaveGame, Display, TEXT("Saving to save game '%s' slot %d"), Name, UserIndex)
    auto &Slots = SaveGames.FindOrAdd(Name);
    Slots.Emplace(UserIndex, Data);
    return true;
}

bool FInMemorySaveGameSystem::LoadGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex,
                                       TArray<uint8> &Data)
{
    UE_LOG(LogEnhancedSaveGame, Display, TEXT("Loading save game '%s' slot %d"), Name, UserIndex)
    // clang-format off
    auto DataSlot = TOptionalPtr(SaveGames.Find(Name))
        .Map([UserIndex](const TMap<int32, TArray<uint8>> *Map) {
                        return Map->Find(UserIndex);
                    });
    DataSlot.IfPresent([&Data](const TArray<uint8> *Loaded) {
        Data = *Loaded;
    });
    // clang-format on

    return DataSlot.IsSet();
}

bool FInMemorySaveGameSystem::DeleteGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex)
{
    return TOptionalPtr(SaveGames.Find(Name)).MapToValue(0, [UserIndex](TMap<int32, TArray<uint8>> *Map) { return Map->Remove(UserIndex); }) > 0;
}
#endif