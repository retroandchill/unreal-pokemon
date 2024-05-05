// "Unreal Pokémon" created by Retro & Chill.


#include "Abilities/AbilityIndexCustomization.h"
#include "DetailWidgetRow.h"
#include "Utility/AbilityIndex.h"

TSharedRef<IPropertyTypeCustomization> FAbilityIndexCustomization::MakeInstance() {
    return MakeShared<FAbilityIndexCustomization>();
}

void FAbilityIndexCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
    FDetailWidgetRow &HeaderRow, IPropertyTypeCustomizationUtils &CustomizationUtils) {
    Options.Empty();
    Options.Add(MakeShared<FText>(FText::FromStringView(TEXT("Option 1"))));
    Options.Add(MakeShared<FText>(FText::FromStringView(TEXT("Option 2"))));

    void* IndexData;
    PropertyHandle->GetValueData(IndexData);
    auto &AbilityIndex = *static_cast<const FAbilityIndex*>(IndexData);
    
	auto IndexHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FAbilityIndex, Index));
    int32 StartingIndex;
    IndexHandle->GetValue(StartingIndex);

    HeaderRow.NameContent()
    [
        PropertyHandle->CreatePropertyNameWidget()
    ]
    .ValueContent()
    [
        SNew(SVerticalBox)
        +SVerticalBox::Slot()
        .Padding(FMargin(0.0f, 3.0f, 0.0f, 2.0f))
        [
            SNew(SComboBox<TSharedPtr<FIntTextPair>>)
                .OptionsSource(&AbilityIndex.TextOptions)
                .OnGenerateWidget(this, &FAbilityIndexCustomization::OnGenerateComboWidget)
                .OnSelectionChanged(this, &FAbilityIndexCustomization::OnComboSelectionChanged, IndexHandle.ToWeakPtr())
                .InitiallySelectedItem(AbilityIndex.TextOptions[StartingIndex])
            [
                SNew(STextBlock)
                .Font(IPropertyTypeCustomizationUtils::GetRegularFont())
                .Text_Lambda([this, IndexHandle, PropertyHandle]() {
                    void* WrapperData;
                    PropertyHandle->GetValueData(WrapperData);
                    auto &Wrapper = *static_cast<const FAbilityIndex*>(WrapperData);
                    
                    int32 CurrentIndex;
                    IndexHandle->GetValue(CurrentIndex);
                    return Wrapper.TextOptions[CurrentIndex]->Value;
                })
            ]
        ]
    ];
}

void FAbilityIndexCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
    IDetailChildrenBuilder &ChildBuilder, IPropertyTypeCustomizationUtils &CustomizationUtils) {
	// Don't display children, as editing them directly can break the constraints
}

TSharedRef<SWidget> FAbilityIndexCustomization::OnGenerateComboWidget(TSharedPtr<FIntTextPair> Option) {
    return SNew(STextBlock).Text(Option->Value);
}

void FAbilityIndexCustomization::OnComboSelectionChanged(TSharedPtr<FIntTextPair> InSelectedItem, ESelectInfo::Type SelectInfo,
    TWeakPtr<IPropertyHandle> HandleWeakPtr) {
    if (auto PropertyHandle = HandleWeakPtr.Pin(); PropertyHandle != nullptr && InSelectedItem != nullptr) {
        ensure(PropertyHandle->SetValue(InSelectedItem->Key) == FPropertyAccess::Success);
    }
}