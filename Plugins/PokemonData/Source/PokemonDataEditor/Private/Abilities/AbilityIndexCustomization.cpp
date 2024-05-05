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
            SNew(SComboBox<TSharedPtr<FText>>)
                .OptionsSource(&Options)
                .OnGenerateWidget(this, &FAbilityIndexCustomization::OnGenerateComboWidget)
                .OnSelectionChanged(this, &FAbilityIndexCustomization::OnComboSelectionChanged, IndexHandle.ToWeakPtr())
                .InitiallySelectedItem(Options[StartingIndex])
            [
                SNew(STextBlock)
                .Font(IPropertyTypeCustomizationUtils::GetRegularFont())
                .Text_Lambda([this, IndexHandle]() {
                    int32 CurrentIndex;
                    IndexHandle->GetValue(CurrentIndex);
                    return *Options[CurrentIndex];
                })
            ]
        ]
    ];
}

void FAbilityIndexCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
    IDetailChildrenBuilder &ChildBuilder, IPropertyTypeCustomizationUtils &CustomizationUtils) {
	// Don't display children, as editing them directly can break the constraints
}

TSharedRef<SWidget> FAbilityIndexCustomization::OnGenerateComboWidget(TSharedPtr<FText> Option) {
    return SNew(STextBlock).Text(*Option);
}

void FAbilityIndexCustomization::OnComboSelectionChanged(TSharedPtr<FText> InSelectedItem, ESelectInfo::Type SelectInfo,
    TWeakPtr<IPropertyHandle> HandleWeakPtr) {
    int32 OptionIndex = Options.Find(InSelectedItem);

    if (auto PropertyHandle = HandleWeakPtr.Pin(); PropertyHandle != nullptr) {
        ensure(PropertyHandle->SetValue(OptionIndex) == FPropertyAccess::Success);
    }
}