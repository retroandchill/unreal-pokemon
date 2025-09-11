// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GameDataEntrySerializer.generated.h"

class UAssetGameDataRepository;

/**
 *
 */
UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataEntrySerializer : public UObject
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual bool Supports(TSubclassOf<UAssetGameDataRepository> RepositoryClass);
    virtual FText GetFormatName() const;
    virtual FString GetFileExtensionText() const;
    virtual bool Serialize(const FString &FilePath, const UAssetGameDataRepository *Repository,
                           FString &ErrorMessage) const;
    virtual bool Deserialize(const FString &FilePath, UAssetGameDataRepository *Repository,
                             FString &ErrorMessage) const;

  protected:
    static FScriptArrayHelper &GetGameDataEntries(const UAssetGameDataRepository *Repository);
    static FArrayProperty *GetGameDataEntriesProperty(const UAssetGameDataRepository *Repository);
    static void MoveEntries(UAssetGameDataRepository *Repository, FScriptArray &Entries);

  private:
    friend class UGameDataEntrySerializerExporter;
};