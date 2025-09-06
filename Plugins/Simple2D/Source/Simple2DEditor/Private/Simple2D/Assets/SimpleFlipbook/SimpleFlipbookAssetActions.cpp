// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookAssetActions.h"
#include "Paper2DEditorModule.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditor.h"

namespace Simple2D
{
    FText FSimpleFlipbookAssetActions::GetName() const
    {
        return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_SimpleFlipbook", "Simple Flipbook");
    }

    UClass *FSimpleFlipbookAssetActions::GetSupportedClass() const
    {
        return USimpleFlipbook::StaticClass();
    }

    FColor FSimpleFlipbookAssetActions::GetTypeColor() const
    {
        return FColor(129, 196, 115);
    }

    uint32 FSimpleFlipbookAssetActions::GetCategories()
    {
        return FModuleManager::GetModuleChecked<IPaper2DEditorModule>("Paper2DEditor").GetPaper2DAssetCategory();
    }

    void FSimpleFlipbookAssetActions::OpenAssetEditor(const TArray<UObject *> &InObjects,
                                                      TSharedPtr<IToolkitHost> EditWithinLevelEditor)
    {
        auto Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
        for (auto Obj : InObjects)
        {
            auto *Flipbook = Cast<USimpleFlipbook>(Obj);
            if (Flipbook == nullptr)
            {
                continue;
            }

            auto NewFlipbookEditor = MakeShared<FSimpleFlipbookEditor>();
            NewFlipbookEditor->InitFlipbookEditor(Mode, EditWithinLevelEditor, Flipbook);
        }
    }
} // namespace Simple2D