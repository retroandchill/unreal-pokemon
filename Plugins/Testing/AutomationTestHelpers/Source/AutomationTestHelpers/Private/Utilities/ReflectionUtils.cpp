// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/ReflectionUtils.h"
#include "AutomationTestModule.h"
#include "Editor/PropertyEditor/Private/DetailCategoryBuilderImpl.h"
#include "Engine/ObjectLibrary.h"
#include "IDetailTreeNode.h"
#include "PropertyPath.h"
#include "Utilities/WidgetTestUtilities.h"

TArray<FString> UReflectionUtils::GetPropertyCategories(IDetailsView &DetailsView) {
    TArray<FString> Categories;
    auto DetailTree = UWidgetTestUtilities::FindFirstChildOfType<STreeView<TSharedRef<FDetailTreeNode>>>(
        DetailsView, TEXT("SDetailTree"));
    if (DetailTree == nullptr) {
        return Categories;
    }

    for (auto RootItems = DetailTree->GetRootItems(); auto &DetailNode : RootItems) {
        if (DetailNode->GetNodeType() != EDetailNodeType::Category) {
            continue;
        }

        auto &Category = static_cast<FDetailCategoryImpl &>(DetailNode.Get());
        if (auto &CategoryName = Category.GetDisplayName().ToString(); !Categories.Contains(CategoryName)) {
            Categories.Emplace(CategoryName);
        }
    }

    return Categories;
}

TStrongObjectPtr<UObjectLibrary> UReflectionUtils::LoadBlueprints() {
    TStrongObjectPtr ClassLibrary(
        UObjectLibrary::CreateLibrary(UBlueprintGeneratedClass::StaticClass(), true, GIsEditor));
    ClassLibrary->LoadBlueprintsFromPath(TEXT("/Game"));
    return ClassLibrary;
}