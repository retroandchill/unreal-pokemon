// "Unreal Pokémon" created by Retro & Chill.

#include "Details/WrappedKeyCustomization.h"
#include "DetailWidgetRow.h"

FWrappedKeyCustomization::FWrappedKeyCustomization(FName Property) : PropertyName(Property) {
}

TSharedRef<IPropertyTypeCustomization> FWrappedKeyCustomization::MakeInstance(FName PropertyName) {
    return MakeShared<FWrappedKeyCustomization>(PropertyName);
}

void FWrappedKeyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                                               IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // clang-format off
    HeaderRow.NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            PropertyHandle->GetChildHandle(PropertyName)->CreatePropertyValueWidget()
        ];
    // clang-format on
}

void FWrappedKeyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                 IDetailChildrenBuilder &ChildBuilder,
                                                 IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // We don't need to display anything here
}