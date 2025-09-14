// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "SaveSerializationUtils.h"

#include "EnhancedSaveGame.generated.h"

class ISerializable;

/**
 *
 */
UCLASS()
class ENHANCEDSAVEGAME_API UEnhancedSaveGame : public USaveGame
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Saving, meta = (AutoCreateRefTerm = Tag))
    bool AddObjectToSaveGame(const FGameplayTag &Tag, const TScriptInterface<ISerializable> &Object);

    template <typename T>
        requires std::derived_from<T, ISerializable> && std::derived_from<T, UObject>
    TOptional<T &> LoadObjectFromSaveGame(const FGameplayTag &Tag) const
    {
        return static_cast<T *>(LoadObjectFromSaveGame(T::StaticClass(), Tag));
    }

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Saving,
              meta = (AutoCreateRefTerm = "Class,Tag", DeterminesOutputType = Class))
    UObject *LoadObjectFromSaveGame(UPARAM(meta = (MustImplement = Serializable)) const TSubclassOf<UObject> &Class,
                                    const FGameplayTag &Tag) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Saving, meta = (AutoCreateRefTerm = Tag))
    bool LoadDataIntoObject(const FGameplayTag &Tag, const TScriptInterface<ISerializable> &TargetObject) const;

  private:
    UPROPERTY(SaveGame)
    TMap<FGameplayTag, FObjectData> Data;
};
