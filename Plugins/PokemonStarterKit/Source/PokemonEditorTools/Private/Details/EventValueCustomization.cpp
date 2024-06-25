// "Unreal Pokémon" created by Retro & Chill.


#include "Details/EventValueCustomization.h"
#include "DetailWidgetRow.h"
#include "Battle/GameplayAbilities/Events/BattleEvent.h"
#include "Widgets/BattleEventPropertyWidget.h"

DECLARE_DELEGATE_OneParam(FOnAttributePicked, FProperty*);


TSharedRef<IPropertyTypeCustomization> FEventValueCustomization::MakeInstance(UScriptStruct *StructType) {
    return MakeShared<FEventValueCustomization>(StructType);
}

FEventValueCustomization::FEventValueCustomization(UScriptStruct *InStructType) : StructType(InStructType) {
}

void FEventValueCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                                               IPropertyTypeCustomizationUtils &CustomizationUtils) {
    MyProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEventValue, Attribute));
    OwnerProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEventValue, AttributeOwner));
    NameProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEventValue, AttributeName));

    PropertyOptions.Empty();
    PropertyOptions.Add(MakeShareable(new FString("None")));

    auto PropertiesToAdd = Pokemon::Battle::Events::GetAllViableProperties(StructType);

    for ( auto* Property : PropertiesToAdd ) {
        PropertyOptions.Add(MakeShareable(new FString(FString::Printf(TEXT("%s.%s"), *Property->GetOwnerVariant().GetName(), *Property->GetName()))));
    }

    FProperty* PropertyValue = nullptr;
    if (MyProperty.IsValid()) {
        FProperty *ObjPtr = nullptr;
        MyProperty->GetValue(ObjPtr);
        PropertyValue = ObjPtr;
    }

    HeaderRow.
        NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
    .ValueContent()
        .MinDesiredWidth(500)
        .MaxDesiredWidth(4096)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            //.FillWidth(1.0f)
            .HAlign(HAlign_Fill)
            .Padding(0.f, 0.f, 2.f, 0.f)
            [
                SNew(SBattleEventPropertyWidget)
                    .OnAttributeChanged(this, &FEventValueCustomization::OnAttributeChanged)
                    .StructType(StructType)
                    .AllProperties(MoveTemp(PropertiesToAdd))
                    .DefaultProperty(PropertyValue)
            ]
        ];
}

void FEventValueCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
    IDetailChildrenBuilder &ChildBuilder, IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // No child customization
}

TSharedPtr<FString> FEventValueCustomization::GetPropertyType() const {
    if (!MyProperty.IsValid())
        return PropertyOptions[0];

    FProperty *PropertyValue = nullptr;
    MyProperty->GetValue(PropertyValue);

    if (PropertyValue != nullptr) {
        for (int32 i = 0; i < PropertyOptions.Num(); ++i) {
            if (PropertyOptions[i].IsValid() && PropertyOptions[i].Get()->Equals(PropertyValue->GetName())) {
                return PropertyOptions[i];
            }
        }
    }

    return PropertyOptions[0]; // This should always be none
}

void FEventValueCustomization::OnChangeProperty(TSharedPtr<FString> ItemSelected, ESelectInfo::Type SelectInfo) {
    if (!ItemSelected.IsValid()) {
        return;
    }

    FString PropertyName = *ItemSelected.Get();

    for( TFieldIterator<FProperty> It(FBattleEventValues::StaticStruct(), EFieldIteratorFlags::ExcludeSuper) ; It ; ++It ) {
        if (auto Property = *It; PropertyName == Property->GetName()) {
            MyProperty->SetValue(Property);
            return;
        }
    }
}

void FEventValueCustomization::OnAttributeChanged(FProperty *SelectedAttribute) {
    if (MyProperty.IsValid()) {
        MyProperty->SetValue(SelectedAttribute);

        // When we set the attribute we should also set the owner and name info
        if (OwnerProperty.IsValid()) {
            OwnerProperty->SetValue(SelectedAttribute ? SelectedAttribute->GetOwnerStruct() : nullptr);
        }

        if (NameProperty.IsValid()) {
            FString AttributeName;
            if (SelectedAttribute) {
                SelectedAttribute->GetName(AttributeName);
            }
            
            NameProperty->SetValue(AttributeName);
        }
    }
}