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
    bool Supports_Implementation(TSubclassOf<UAssetGameDataRepository> RepositoryClass) override;
    FString GetFileExtensionText_Implementation() const override;
    FText GetFormatName_Implementation() const override;
    bool Serialize_Implementation(const FString &FilePath, const UAssetGameDataRepository *Repository,
                                  FString &ErrorMessage) const override;
    bool Deserialize_Implementation(const FString &FilePath, UAssetGameDataRepository *Repository,
                                    FString &ErrorMessage) const override;
};
