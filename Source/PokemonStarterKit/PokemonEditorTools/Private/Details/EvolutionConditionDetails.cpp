// Fill out your copyright notice in the Description page of Project Settings.

#include "Details/EvolutionConditionDetails.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "InstancedStructDetails.h"
#include "Interop/PokemonDataManagedCallbacks.h"
#include "TypeGenerator/CSScriptStruct.h"

struct FGameplayTag;

bool FEvolutionConditionPropertyIdentifier::IsPropertyTypeCustomized(const IPropertyHandle &PropertyHandle) const
{
    static const FName EvolutionConditionName = TEXT("EvolutionCondition");
    const auto Property = CastFieldChecked<FStructProperty>(PropertyHandle.GetProperty());
    const auto ManagedStruct = Cast<UCSScriptStruct>(Property->Struct);
    if (ManagedStruct == nullptr)
    {
        return false;
    }

    return ManagedStruct->GetFName() == EvolutionConditionName;
}

TSharedRef<IPropertyTypeCustomization> FEvolutionConditionDetails::MakeInstance()
{
    return MakeShared<FEvolutionConditionDetails>();
}

void FEvolutionConditionDetails::CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle,
                                                 FDetailWidgetRow &HeaderRow,
                                                 IPropertyTypeCustomizationUtils &CustomizationUtils)
{
    // clang-format off
    HeaderRow.NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            PropertyHandle->CreatePropertyValueWidget()
        ];
    // clang-format on
}

void FEvolutionConditionDetails::CustomizeChildren(const TSharedRef<IPropertyHandle> PropertyHandle,
                                                   IDetailChildrenBuilder &ChildBuilder,
                                                   IPropertyTypeCustomizationUtils &CustomizationUtils)
{
    const auto StructProperty = CastFieldChecked<FStructProperty>(PropertyHandle->GetProperty());
    const UScriptStruct *ScriptStruct = StructProperty->Struct;

    for (auto CurrentProperty = ScriptStruct->PropertyLink; CurrentProperty != nullptr;
         CurrentProperty = CastField<FProperty>(CurrentProperty->Next))
    {
        if (CurrentProperty->GetFName() == TEXT("Method"))
        {
            auto MethodHandle = PropertyHandle->GetChildHandle(CurrentProperty->GetFName()).ToSharedRef();

            // Set up the callback for when the value changes
            MethodHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateStatic(
                &FEvolutionConditionDetails::OnMethodChanged, PropertyHandle, MethodHandle));

            ChildBuilder.AddProperty(MethodHandle);
            continue;
        }

        // Skip the 'Parameters' property as we want to display its properties inline
        if (CurrentProperty->GetFName() != TEXT("Parameters"))
        {
            // Add all other properties normally
            const auto ChildHandle = PropertyHandle->GetChildHandle(CurrentProperty->GetFName()).ToSharedRef();
            ChildBuilder.AddProperty(ChildHandle);
            continue;
        }

        // Get the Parameters property handle
        auto ParametersHandle = PropertyHandle->GetChildHandle(CurrentProperty->GetFName());
        if (ParametersHandle == nullptr)
            continue;

        // Add the struct children
        auto InstancedStructDetails = MakeShared<FInstancedStructDataDetails>(ParametersHandle);
        ChildBuilder.AddCustomBuilder(InstancedStructDetails);
    }
}

void FEvolutionConditionDetails::OnMethodChanged(TSharedRef<IPropertyHandle> ParentProperty,
                                                 TSharedRef<IPropertyHandle> MethodProperty)
{
    void *RawValue;
    MethodProperty->GetValueData(RawValue);
    const auto *Handle = static_cast<uint8 *>(RawValue);

    auto CreatedObject = FPokemonDataManagedCallbacks::Get().GetEvolutionConditionClass(Handle);

    if (!CreatedObject.has_value())
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *CreatedObject.error());
        return;
    }

    const auto DataProperty = ParentProperty->GetChildHandle(TEXT("Parameters"));
    check(DataProperty != nullptr);

    DataProperty->NotifyPreChange();
    TArray<void *> Objects;
    DataProperty->AccessRawData(Objects);
    const auto TargetStruct = CreatedObject.value();
    for (const auto Object : Objects)
    {
        auto &AsInstancedStruct = *static_cast<FInstancedStruct *>(Object);
        if (AsInstancedStruct.GetScriptStruct() == TargetStruct)
        {
            continue;
        }

        if (TargetStruct != nullptr)
        {
            AsInstancedStruct.InitializeAs(TargetStruct);
        }
        else
        {
            AsInstancedStruct.Reset();
        }
    }
    DataProperty->NotifyPostChange(EPropertyChangeType::ValueSet);
}
