// "Unreal Pokémon" created by Retro & Chill.

#include "Details/InjectionTargetCustomization.h"
#include "DetailWidgetRow.h"
#include "EditorClassUtils.h"
#include "PropertyCustomizationHelpers.h"
#include "Lookup/InjectionTarget.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Optional/OrElse.h"

TSharedRef<IPropertyTypeCustomization> FInjectionTargetCustomization::MakeInstance() {
    return MakeShared<FInjectionTargetCustomization>();
}

void FInjectionTargetCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle,
                                                    FDetailWidgetRow &HeaderRow,
                                                    IPropertyTypeCustomizationUtils &CustomizationUtils) {
    PropertyHandle = InPropertyHandle;
    
    const bool bShowTreeView = PropertyHandle->HasMetaData("ShowTreeView");
    const bool bHideViewOptions = PropertyHandle->HasMetaData("HideViewOptions");
    const bool bShowDisplayNames = PropertyHandle->HasMetaData("ShowDisplayNames");

    UObject *InterfaceObject;
    PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInjectionTarget, TargetInterface))->GetValue(InterfaceObject);
    TSubclassOf<UObject> FoundClass = Cast<UClass>(InterfaceObject);
    auto MetaClass = UE::Optionals::OfNullable(FoundClass) |
        UE::Optionals::Filter([](const UClass& Class) { return !Class.IsChildOf<UInterface>(); }) |
        UE::Optionals::OrElse(UObject::StaticClass());
    auto RequiredInterface = UE::Optionals::OfNullable(FoundClass) |
        UE::Optionals::Filter([](const UClass& Class) { return Class.IsChildOf<UInterface>(); }) |
        UE::Optionals::GetPtrOrNull;
        

    HeaderRow
    .NameContent()
    [
        PropertyHandle->CreatePropertyNameWidget()
    ]
    .ValueContent()
    .MinDesiredWidth(350.0f)
    .MaxDesiredWidth(0.0f)
    [
        SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .VAlign(VAlign_Center)
            .HAlign(HAlign_Left)
            [
                SNew(STextBlock)
                .Text(UE::Optionals::OfNullable(FoundClass) |
                    UE::Optionals::Map([](const UClass& Class) { return Class.GetDisplayNameText(); }) |
                    UE::Optionals::OrElse(FText::FromStringView(TEXT("Invalid"))))
            ]
            + SHorizontalBox::Slot()
                .VAlign(VAlign_Center)
                .HAlign(HAlign_Left)
            [
                // Add a class entry box.  Even though this isn't an class entry, we will simulate one
            SNew(SClassPropertyEntryBox)
                .MetaClass(MetaClass)
                .RequiredInterface(RequiredInterface)
                .AllowAbstract(false)
                .AllowNone(false)
                .ShowTreeView(bShowTreeView)
                .HideViewOptions(bHideViewOptions)
                .ShowDisplayNames(bShowDisplayNames)
                .SelectedClass(this, &FInjectionTargetCustomization::OnGetClass)
                .OnSetClass(this, &FInjectionTargetCustomization::OnSetClass)
            ]
    ];
}

void FInjectionTargetCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
                                                      IDetailChildrenBuilder &ChildBuilder,
                                                      IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // No child customization
}

const UClass * FInjectionTargetCustomization::OnGetClass() const {
    auto WrappedProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInjectionTarget, InjectedClass));
    FString ClassName;
    WrappedProperty->GetValueAsFormattedString(ClassName);

    // Do we have a valid cached class pointer?
    const UClass* Class = CachedClassPtr.Get();
    if(!Class || Class->GetPathName() != ClassName)
    {
        Class = FEditorClassUtils::GetClassFromString(ClassName);
        CachedClassPtr = MakeWeakObjectPtr(const_cast<UClass*>(Class));
    }
    return Class;
}

void FInjectionTargetCustomization::OnSetClass(const UClass *NewClass) const {
    if (auto WrappedProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInjectionTarget, InjectedClass)); WrappedProperty->SetValueFromFormattedString((NewClass) ? NewClass->GetPathName() : "None") == FPropertyAccess::Result::Success) {
        CachedClassPtr = MakeWeakObjectPtr(const_cast<UClass*>(NewClass));
    }
}