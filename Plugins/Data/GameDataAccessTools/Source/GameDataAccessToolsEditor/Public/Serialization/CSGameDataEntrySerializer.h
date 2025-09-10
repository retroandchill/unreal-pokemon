// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/GameDataEntrySerializer.h"
#include "CSGameDataEntrySerializer.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UCSGameDataEntrySerializer : public UGameDataEntrySerializer
{
    GENERATED_BODY()

public:
    bool Supports(TSubclassOf<UAssetGameDataRepository> RepositoryClass) override;
    FString GetFileExtensionText() const override;
    FText GetFormatName() const override;
    bool Serialize(const FString &FilePath, const UAssetGameDataRepository *Repository,
                                  FString &ErrorMessage) const override;
    bool Deserialize(const FString &FilePath, UAssetGameDataRepository *Repository,
        FString &ErrorMessage) const override;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta=(ScriptName = Supports))
    bool K2_Supports(TSubclassOf<UAssetGameDataRepository> RepositoryClass);

    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta=(ScriptName = GetFormatName))
    FText K2_GetFormatName() const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta=(ScriptName = GetFileExtensionText))
    FString K2_GetFileExtensionText() const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta=(ScriptName = Serialize))
    bool K2_Serialize(const FString &FilePath, const UAssetGameDataRepository *Repository, FString &ErrorMessage) const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Serialization", meta=(ScriptName = Deserialize))
    bool K2_Deserialize(const FString &FilePath, UAssetGameDataRepository *Repository, FString &ErrorMessage) const;

};
