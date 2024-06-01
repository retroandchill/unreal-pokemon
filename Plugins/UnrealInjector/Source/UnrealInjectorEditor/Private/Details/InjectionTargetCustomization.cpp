// "Unreal Pokémon" created by Retro & Chill.

#include "Details/InjectionTargetCustomization.h"
#include "DetailWidgetRow.h"

TSharedRef<IPropertyTypeCustomization> FInjectionTargetCustomization::MakeInstance() {
    return MakeShared<FInjectionTargetCustomization>();
}

void FInjectionTargetCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                                    FDetailWidgetRow &HeaderRow,
                                                    IPropertyTypeCustomizationUtils &CustomizationUtils) {
    UObject *InterfaceObject;
    PropertyHandle->GetChildHandle(TEXT("TargetInterface"))->GetValue(InterfaceObject);
    auto WrappedProperty = PropertyHandle->GetChildHandle(TEXT("InjectedClass"));
    WrappedProperty->GetProperty()->SetMetaData(TEXT("MustImplement"), InterfaceObject->GetName());
    HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
        .ValueContent()[WrappedProperty->CreatePropertyValueWidget(false)];
    WrappedProperty->GetProperty()->RemoveMetaData(TEXT("MustImplement"));
}

void FInjectionTargetCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                      IDetailChildrenBuilder &ChildBuilder,
                                                      IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // No child customization
}