// "Unreal Pokémon" created by Retro & Chill.


#include "Saving/SaveGameManager/InMemorySaveGameSystem.h"
#include "EnhancedSaveGameModule.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/IfPresent.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OrElse.h"

using namespace EnhancedSaveGame;


bool FInMemorySaveGameSystem::PlatformHasNativeUI() {
    return false;
}

bool FInMemorySaveGameSystem::DoesSaveSystemSupportMultipleUsers() {
    return false;
}

bool FInMemorySaveGameSystem::DoesSaveGameExist(const TCHAR *Name, const int32 UserIndex) {
    // clang-format off
    return UE::Optionals::OfNullable(SaveGames.Find(Name)) |
           UE::Optionals::Map([UserIndex](const TMap<int32, TArray<uint8>> &Map) {
               return Map.Contains(UserIndex);
           }) |
           UE::Optionals::OrElse(false);
    // clang-format on

}

ISaveGameSystem::ESaveExistsResult FInMemorySaveGameSystem::DoesSaveGameExistWithResult(const TCHAR *Name,
    const int32 UserIndex) {
    // clang-format off
    return UE::Optionals::OfNullable(SaveGames.Find(Name)) |
           UE::Optionals::Map([UserIndex](const TMap<int32, TArray<uint8>> &Map) {
               return Map.Find(UserIndex);
           }) |
           UE::Optionals::Map([](const TArray<uint8> &) {
               return ESaveExistsResult::OK;
           }) |
           UE::Optionals::OrElse(ESaveExistsResult::DoesNotExist);
    // clang-format on
}

bool FInMemorySaveGameSystem::SaveGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex,
                                       const TArray<uint8> &Data) {
    UE_LOG(LogEnhancedSaveGame, Log, TEXT("Saving to save game '%s' slot %d"), Name, UserIndex)
    auto &Slots = SaveGames.FindOrAdd(Name);
    Slots.Emplace(UserIndex, Data);
    return true;
}

bool FInMemorySaveGameSystem::LoadGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex,
                                       TArray<uint8> &Data) {
    UE_LOG(LogEnhancedSaveGame, Log, TEXT("Loading save game '%s' slot %d"), Name, UserIndex)
    // clang-format off
    auto DataSlot = UE::Optionals::OfNullable(SaveGames.Find(Name)) |
                    UE::Optionals::Map([UserIndex](const TMap<int32, TArray<uint8>> &Map) {
                        return Map.Find(UserIndex);
                    });
    DataSlot | UE::Optionals::IfPresent([&Data](const TArray<uint8> &Loaded) {
        Data = Loaded;
    });
    // clang-format on

    return DataSlot.IsSet();

}

bool FInMemorySaveGameSystem::DeleteGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex) {
    // clang-format off
    return UE::Optionals::OfNullable(SaveGames.Find(Name)) |
        UE::Optionals::Map([UserIndex](TMap<int32, TArray<uint8>> &Map) { return Map.Remove(UserIndex); }) |
        UE::Optionals::Map([](int32 Num) { return Num > 0; })|
        UE::Optionals::OrElse(false);
    // clang-format on
}