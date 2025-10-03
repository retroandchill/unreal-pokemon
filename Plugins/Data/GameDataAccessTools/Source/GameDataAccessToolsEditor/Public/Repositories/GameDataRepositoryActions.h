#pragma once

#include "AssetTypeActions_Base.h"
#include "CSManagedGCHandle.h"
#include "Serialization/GameDataEntrySerializer.h"

class FGameDataEntrySerializerPtr;
class UAssetGameDataRepository;
class IGameDataEntrySerializer;

/**
 * @class FGameDataRepositoryActions
 * @brief Implements asset type actions for Game Data Repository assets.
 *
 * This class provides custom actions and editor integration for Game Data Repository assets.
 * It overrides functionality to retrieve asset details, categorize, provide context menus with custom actions, and
 * handle editor opening for such assets.
 *
 * This class is responsible for the following:
 * - Defining the display name of the asset type.
 * - Specifying the asset type color.
 * - Indicating the associated supported class type.
 * - Defining the asset categories.
 * - Providing a menu with options related to serialization and exporting assets.
 * - Extending editor support for asset interaction.
 *
 * The class implements several static utility functions for handling menu building and exporting operations.
 */
class FGameDataRepositoryActions final : public FAssetTypeActions_Base
{
  public:
    FText GetName() const override;
    FColor GetTypeColor() const override;
    UClass *GetSupportedClass() const override;
    uint32 GetCategories() override;

    void GetActions(const TArray<UObject *> &InObjects, FMenuBuilder &MenuBuilder) override;

    void OpenAssetEditor(const TArray<UObject *> &InObjects,
                         const TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;

  private:
    static void AddSerializationActions(FMenuBuilder &SubMenuBuilder, const UObject *InObject);
    static void ExportAsset(const UAssetGameDataRepository *Repository, const UGameDataEntrySerializer *Serializer);
};
