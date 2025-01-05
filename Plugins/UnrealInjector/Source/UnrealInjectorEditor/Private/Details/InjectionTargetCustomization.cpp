// "Unreal Pokémon" created by Retro & Chill.

#include "Details/InjectionTargetCustomization.h"
#include "DetailWidgetRow.h"
#include "EditorClassUtils.h"
#include "Lookup/InjectionTarget.h"
#include "PropertyCustomizationHelpers.h"
#include "RetroLib/Optionals/Filter.h"
#include "RetroLib/Optionals/OrElseValue.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"

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
    PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInjectionTarget, TargetInterface))
        ->GetValue(InterfaceObject);
    TSubclassOf<UObject> FoundClass = Cast<UClass>(InterfaceObject);
    auto MetaClass = Retro::Optionals::OfNullable(FoundClass) |
                     Retro::Optionals::Filter([](const UClass *Class) { return !Class->IsChildOf<UInterface>(); }) |
                     Retro::Optionals::OrElseValue(UObject::StaticClass());
    auto RequiredInterface =
        Retro::Optionals::OfNullable(FoundClass) |
        Retro::Optionals::Filter([](const UClass *Class) { return Class->IsChildOf<UInterface>(); }) |
        Retro::Optionals::PtrOrNull;

    HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
        .ValueContent()
        .MinDesiredWidth(350.0f)
        .MaxDesiredWidth(
            0.0f)[SNew(SHorizontalBox) +
                  SHorizontalBox::Slot()
                      .VAlign(VAlign_Center)
                      .HAlign(HAlign_Left)[SNew(STextBlock)
                                               .Text(Retro::Optionals::OfNullable(FoundClass) |
                                                     Retro::Optionals::Transform([](const UClass *Class) {
                                                         return Class->GetDisplayNameText();
                                                     }) |
                                                     Retro::Optionals::OrElseValue(
                                                         FText::FromStringView(TEXT("Invalid"))))] +
                  SHorizontalBox::Slot()
                      .VAlign(VAlign_Center)
                      .HAlign(HAlign_Left)[
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
                              .OnSetClass(this, &FInjectionTargetCustomization::OnSetClass)]];
}

void FInjectionTargetCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
                                                      IDetailChildrenBuilder &ChildBuilder,
                                                      IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // No child customization
}

const UClass *FInjectionTargetCustomization::OnGetClass() const {
    auto WrappedProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInjectionTarget, InjectedClass));
    FString ClassName;
    WrappedProperty->GetValueAsFormattedString(ClassName);

    // Do we have a valid cached class pointer?
    auto Class = CachedClassPtr.Get();
    if (!Class || Class->GetPathName() != ClassName) {
        Class = FEditorClassUtils::GetClassFromString(ClassName);
        CachedClassPtr = MakeWeakObjectPtr(Class);
    }
    return Class;
}

void FInjectionTargetCustomization::OnSetClass(const UClass *NewClass) const {
    using enum FPropertyAccess::Result;
    if (auto WrappedProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FInjectionTarget, InjectedClass));
        WrappedProperty->SetValueFromFormattedString(NewClass ? NewClass->GetPathName() : "None") == Success) {
        CachedClassPtr = MakeWeakObjectPtr(NewClass);
        void *Struct;
        PropertyHandle->GetValueData(Struct);
        auto AsInjectionTarget = static_cast<FInjectionTarget *>(Struct);
        AsInjectionTarget->OnInjectedClassEdited.Broadcast(AsInjectionTarget->InjectedClass);
    }
}