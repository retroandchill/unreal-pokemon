// "Unreal Pokémon" created by Retro & Chill.

#include "Saving/SaveGameManager/InMemorySaveGameSystem.h"
#include "EnhancedSaveGameModule.h"
#include "RetroLib.h"

using namespace EnhancedSaveGame;

bool FInMemorySaveGameSystem::PlatformHasNativeUI() {
    return false;
}

bool FInMemorySaveGameSystem::DoesSaveSystemSupportMultipleUsers() {
    return false;
}

bool FInMemorySaveGameSystem::DoesSaveGameExist(const TCHAR *Name, const int32 UserIndex) {
    // clang-format off
    return Retro::Optionals::OfNullable(SaveGames.Find(Name)) |
           Retro::Optionals::Transform([UserIndex](const TMap<int32, TArray<uint8>> &Map) {
               return Map.Contains(UserIndex);
           }) |
           Retro::Optionals::OrElseValue(false);
    // clang-format on
}

ISaveGameSystem::ESaveExistsResult FInMemorySaveGameSystem::DoesSaveGameExistWithResult(const TCHAR *Name,
                                                                                        const int32 UserIndex) {
    // clang-format off
    return Retro::Optionals::OfNullable(SaveGames.Find(Name)) |
           Retro::Optionals::Transform([UserIndex](const TMap<int32, TArray<uint8>> &Map) {
               return Map.Find(UserIndex);
           }) |
           Retro::Optionals::Transform([](const TArray<uint8> &) {
               return ESaveExistsResult::OK;
           }) |
           Retro::Optionals::OrElseValue(ESaveExistsResult::DoesNotExist);
    // clang-format on
}

bool FInMemorySaveGameSystem::SaveGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex,
                                       const TArray<uint8> &Data) {
    UE_LOG(LogEnhancedSaveGame, Display, TEXT("Saving to save game '%s' slot %d"), Name, UserIndex)
    auto &Slots = SaveGames.FindOrAdd(Name);
    Slots.Emplace(UserIndex, Data);
    return true;
}

bool FInMemorySaveGameSystem::LoadGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex,
                                       TArray<uint8> &Data) {
    UE_LOG(LogEnhancedSaveGame, Display, TEXT("Loading save game '%s' slot %d"), Name, UserIndex)
    // clang-format off
    auto DataSlot = Retro::Optionals::OfNullable(SaveGames.Find(Name)) |
                    Retro::Optionals::Transform([UserIndex](const TMap<int32, TArray<uint8>> &Map) {
                        return Map.Find(UserIndex);
                    });
    DataSlot | Retro::Optionals::IfPresent([&Data](const TArray<uint8> &Loaded) {
        Data = Loaded;
    });
    // clang-format on

    return DataSlot.IsSet();
}

bool FInMemorySaveGameSystem::DeleteGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex) {
    // clang-format off
    return Retro::Optionals::OfNullable(SaveGames.Find(Name)) |
        Retro::Optionals::Transform([UserIndex](TMap<int32, TArray<uint8>> &Map) { return Map.Remove(UserIndex); }) |
        Retro::Optionals::Transform([](int32 Num) { return Num > 0; })|
        Retro::Optionals::OrElseValue(false);
    // clang-format on
}