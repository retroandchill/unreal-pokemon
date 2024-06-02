// "Unreal Pokémon" created by Retro & Chill.


#include "Details/PocketKeyCustomization.h"
#include "DetailWidgetRow.h"

TSharedRef<IPropertyTypeCustomization> FPocketKeyCustomization::MakeInstance() {
    return MakeShared<FPocketKeyCustomization>();
}

void FPocketKeyCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
    IPropertyTypeCustomizationUtils &CustomizationUtils) {
    HeaderRow
        .NameContent()[PropertyHandle->CreatePropertyNameWidget()]
        .ValueContent()[PropertyHandle->GetChildHandle(TEXT("PocketName"))->CreatePropertyValueWidget()];
}

void FPocketKeyCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
    IDetailChildrenBuilder &ChildBuilder, IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // We don't need to display anything here
}