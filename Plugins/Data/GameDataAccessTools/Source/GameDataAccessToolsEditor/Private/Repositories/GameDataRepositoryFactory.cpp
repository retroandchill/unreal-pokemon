// Fill out your copyright notice in the Description page of Project Settings.

#include "Repositories/GameDataRepositoryFactory.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "GameDataRepository.h"
#include "Kismet2/SClassPickerDialog.h"

class FGameDataRepositoryFilter final : public IClassViewerFilter
{
  public:
    bool IsClassAllowed(const FClassViewerInitializationOptions &InInitOptions, const UClass *InClass,
                        const TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
    {
        if (!InClass->IsChildOf<UGameDataRepository>() || InClass->HasAnyClassFlags(CLASS_Abstract))
        {
            return false;
        }

        const auto DataEntriesProperty = CastField<FArrayProperty>(InClass->FindPropertyByName(TEXT("DataEntries")));
        if (DataEntriesProperty == nullptr)
        {
            return false;
        }

        const auto InnerProperty = CastField<FStructProperty>(DataEntriesProperty->Inner);
        return InnerProperty != nullptr;
    }

    bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions &InInitOptions,
                                const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
                                const TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
    {
        return false;
    }
};

UGameDataRepositoryFactory::UGameDataRepositoryFactory()
{
    bCreateNew = true;
    bEditAfterNew = true;
    SupportedClass = UGameDataRepository::StaticClass();
}

bool UGameDataRepositoryFactory::ConfigureProperties()
{
    // Configure the class viewer
    FClassViewerInitializationOptions Options;
    Options.Mode = EClassViewerMode::ClassPicker;
    Options.DisplayMode = EClassViewerDisplayMode::DefaultView;
    Options.bShowObjectRootClass = false;

    // Only show classes that are non-abstract and inherit from our base class
    const auto Filter = MakeShared<FGameDataRepositoryFilter>();
    Options.ClassFilters.Add(Filter);

    const FText TitleText =
        NSLOCTEXT("GameDataRepositoryFactory", "CreateGameDataRepositoryOptions", "Pick Game Data Asset Class");
    if (UClass *ChosenClass = nullptr;
        SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UGameDataRepository::StaticClass()))
    {
        AssetClass = ChosenClass;
        return true;
    }

    return false;
}

UObject *UGameDataRepositoryFactory::FactoryCreateNew(UClass *InClass, UObject *InParent, const FName InName,
                                                      const EObjectFlags Flags, UObject *Context,
                                                      FFeedbackContext *Warn)
{
    return NewObject<UGameDataRepository>(InParent, AssetClass, InName, Flags);
}
