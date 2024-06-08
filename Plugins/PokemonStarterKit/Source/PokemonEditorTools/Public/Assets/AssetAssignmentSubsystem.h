// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"

#include "AssetAssignmentSubsystem.generated.h"

class IAssetRepository;
class UTextureRepository;
/**
 * Editor subsystem used to categorize assets into their appropriate bins.
 */
UCLASS()
class POKEMONEDITORTOOLS_API UAssetAssignmentSubsystem : public UEditorSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

  private:
    void HandleAssetAdded(const FAssetData &AssetData) const;
    void HandleAssetRenamed(const FAssetData &AssetData, const FString &OldAssetName) const;
    void HandleAssetRemoved(const FAssetData &AssetData) const;

    UPROPERTY()
    TMap<FName, TScriptInterface<IAssetRepository>> AssetRepositories;
};
