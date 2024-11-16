// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SaveGameSystem.h"

class USaveGame;

#if WITH_EDITOR
namespace EnhancedSaveGame {
    /**
     * Subsystem that holds save games in memory, primarily for usage in testing.
     */
    class ENHANCEDSAVEGAME_API FInMemorySaveGameSystem : public ISaveGameSystem {
      public:
        bool PlatformHasNativeUI() override;
        bool DoesSaveSystemSupportMultipleUsers() override;
        bool DoesSaveGameExist(const TCHAR *Name, const int32 UserIndex) override;
        ESaveExistsResult DoesSaveGameExistWithResult(const TCHAR *Name, const int32 UserIndex) override;
        bool SaveGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex,
                      const TArray<uint8> &Data) override;
        bool LoadGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex, TArray<uint8> &Data) override;
        bool DeleteGame(bool bAttemptToUseUI, const TCHAR *Name, const int32 UserIndex) override;

      private:
        TMap<FName, TMap<int32, TArray<uint8>>> SaveGames;
    };
} // namespace EnhancedSaveGame
#endif