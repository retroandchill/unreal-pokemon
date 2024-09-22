// "Unreal Pokémon" created by Retro & Chill.

#include "Details/InjectionTargetCustomization.h"
#include "DetailWidgetRow.h"
#include "Lookup/InjectionTarget.h"

TSharedRef<IPropertyTypeCustomization> FInjectionTargetCustomization::MakeInstance() {
    return MakeShared<FInjectionTargetCustomization>();
}

void FInjectionTargetCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                                    FDetailWidgetRow &HeaderRow,
                                                    IPropertyTypeCustomizationUtils &CustomizationUtils) {
    UObject *InterfaceObject;
    PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInjectionTarget, TargetInterface))->GetValue(InterfaceObject);
    auto WrappedProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInjectionTarget, InjectedClass));
    if (InterfaceObject != nullptr) {
        WrappedProperty->GetProperty()->SetMetaData(TEXT("MustImplement"), InterfaceObject->GetName());
    }
    HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
        .ValueContent()[WrappedProperty->CreatePropertyValueWidget(false)];
    WrappedProperty->GetProperty()->RemoveMetaData(TEXT("MustImplement"));
}

void FInjectionTargetCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                      IDetailChildrenBuilder &ChildBuilder,
                                                      IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // No child customization
}