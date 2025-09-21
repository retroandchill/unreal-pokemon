#include "GameDataAccessToolsEditor.h"
#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"
#include "Handles/DataHandleCustomization.h"
#include "Handles/DataHandlePinFactory.h"
#include "Handles/DataHandlePropertyIdentifier.h"
#include "PropertyEditorModule.h"
#include "Repositories/GameDataEntryDetailsCustomization.h"
#include "Repositories/GameDataRepositoryActions.h"

class FGameDataAccessToolsEditorModule final : public IGameDataAccessToolsEditorModule
{
  public:
    void StartupModule() override
    {
        FEdGraphUtilities::RegisterVisualPinFactory(MakeShared<FDataHandlePinFactory>());

        // Register the asset type actions
        auto &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

        GameDataRepositoryActions = MakeShared<FGameDataRepositoryActions>();
        AssetTools.RegisterAssetTypeActions(GameDataRepositoryActions.ToSharedRef());

        auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.RegisterCustomClassLayout(
            "GameDataEntry",
            FOnGetDetailCustomizationInstance::CreateStatic(&FGameDataEntryDetailsCustomization::MakeInstance));

        PropertyModule.RegisterCustomPropertyTypeLayout(
            "StructProperty",
            FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataHandleCustomization::MakeInstance),
            MakeShared<FDataHandlePropertyIdentifier>());
    }

    void ShutdownModule() override
    {
        if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
        {
            IAssetTools &AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
            if (GameDataRepositoryActions.IsValid())
            {
                AssetTools.UnregisterAssetTypeActions(GameDataRepositoryActions.ToSharedRef());
            }
        }
    }

    TArray<TObjectPtr<const UGameDataEntrySerializer>> GetAvailableSerializers(
        TSubclassOf<UAssetGameDataRepository> RepositoryClass) const override
    {
        TArray<TObjectPtr<const UGameDataEntrySerializer>> Serializers;
        for (TObjectIterator<UClass> It; It; ++It)
        {
            if (It->HasAnyClassFlags(CLASS_Abstract) || !It->IsChildOf(UGameDataEntrySerializer::StaticClass()))
            {
                continue;
            }

            if (auto *CDO = CastChecked<UGameDataEntrySerializer>(It->GetDefaultObject());
                CDO->Supports(RepositoryClass))
            {
                Serializers.Emplace(CDO);
            }
        }

        return Serializers;
    }

  private:
    TSharedPtr<FGameDataRepositoryActions> GameDataRepositoryActions;
};

IGameDataAccessToolsEditorModule &IGameDataAccessToolsEditorModule::Get()
{
    return FModuleManager::LoadModuleChecked<FGameDataAccessToolsEditorModule>("GameDataAccessToolsEditor");
}

IMPLEMENT_MODULE(FGameDataAccessToolsEditorModule, GameDataAccessToolsEditor)
