#pragma once

#include "AssetTypeActions_Base.h"
#include "CSManagedGCHandle.h"
#include "Serialization/GameDataEntrySerializer.h"


class FGameDataEntrySerializerPtr;
class UGameDataRepository;
class IGameDataEntrySerializer;
DECLARE_DELEGATE_OneParam(FOnFileAction, const FString&)

struct FImportExportAction
{
    FSharedGCHandle ActionHandle;
    FText FormatName;
    FOnFileAction ImportAction;
    FOnFileAction ExportAction;
};

class FGameDataRepositoryActions final : public FAssetTypeActions_Base
{
public:
    FText GetName() const override;
    FColor GetTypeColor() const override;
    UClass* GetSupportedClass() const override;
    uint32 GetCategories() override;

    void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;

    void OpenAssetEditor(const TArray<UObject*>& InObjects,
                         const TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;

private:
    static void AddSerializationActions(FMenuBuilder& SubMenuBuilder, const UObject* InObject);
    static void ExportAsset(const UGameDataRepository* Repository, const UGameDataEntrySerializer* Serializer);
};
