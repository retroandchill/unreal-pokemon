// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameDataEntrySerializer.h"
#include "UObject/Object.h"

#include "GameDataEntryJsonSerializer.generated.h"

/**
 *
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataEntryJsonSerializer : public UGameDataEntrySerializer
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
};
