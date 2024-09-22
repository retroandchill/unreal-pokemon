// "Unreal Pokémon" created by Retro & Chill.

#include "Details/BlueprintNodeTargetCustomization.h"
#include "DetailWidgetRow.h"
#include "Lookup/BlueprintNodeTarget.h"

TSharedRef<IPropertyTypeCustomization> FBlueprintNodeTargetCustomization::MakeInstance() {
    return MakeShared<FBlueprintNodeTargetCustomization>();
}

void FBlueprintNodeTargetCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                                    FDetailWidgetRow &HeaderRow,
                                                    IPropertyTypeCustomizationUtils &CustomizationUtils) {
    UObject *InterfaceObject;
    PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBlueprintNodeTarget, TargetClass))->GetValue(InterfaceObject);
    auto WrappedProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FBlueprintNodeTarget, InjectedClass));
    if (InterfaceObject != nullptr) {
        WrappedProperty->GetProperty()->SetMetaData(TEXT("MetaClass"), InterfaceObject->GetName());
    }
    HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
        .ValueContent()[WrappedProperty->CreatePropertyValueWidget(false)];
    WrappedProperty->GetProperty()->RemoveMetaData(TEXT("MetaClass"));
}

void FBlueprintNodeTargetCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                      IDetailChildrenBuilder &ChildBuilder,
                                                      IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // No child customization
}