#include "RetroLib/Async/Saving.h"
#include "PlatformFeatures.h"
#include "SaveGameSystem.h"
#include "Kismet/GameplayStatics.h"

namespace Retro {
    UE5Coro::TCoroutine<bool> SaveGameToSlotAsync(USaveGame* SaveGameObject, const FString& SlotName, const int32 UserIndex) {
        ISaveGameSystem* SaveSystem = IPlatformFeaturesModule::Get().GetSaveGameSystem();

        if (auto ObjectBytes = MakeShared<TArray<uint8>>(); SaveSystem && (SlotName.Len() > 0) && 
                                                            UGameplayStatics::SaveGameToMemory(SaveGameObject, *ObjectBytes) && (ObjectBytes->Num() > 0) )
        {
            FPlatformUserId PlatformUserId = FPlatformMisc::GetPlatformUserForUserIndex(UserIndex);

            TDelegate<void(bool)> SavedDelegate;
            SaveSystem->SaveGameAsync(false, *SlotName, PlatformUserId, ObjectBytes, 
                [SavedDelegate](const FString&, FPlatformUserId, bool bSuccess)
                {
                    check(IsInGameThread());
                    (void)SavedDelegate.ExecuteIfBound(bSuccess);
                }
            );
            auto [Result] = co_await SavedDelegate;
            co_return Result;
        }
        
        co_return false;
    }
}