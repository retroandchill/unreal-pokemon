// "Unreal Pokémon" created by Retro & Chill.

#include "Details/DataHandleCustomization.h"
#include "Data/DataStructHandleNode.h"
#include "DataRetrieval/DataStructHandle.h"
#include "DetailWidgetRow.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/NameAliases.h"

TSharedRef<IPropertyTypeCustomization> FDataHandleCustomization::MakeInstance() {
    return MakeShared<FDataHandleCustomization>();
}

void FDataHandleCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                                               IPropertyTypeCustomizationUtils &CustomizationUtils) {
    TArray<UObject *> OuterObjects;
    PropertyHandle->GetOuterObjects(OuterObjects);
    // clang-format off
    auto StructTypes = OuterObjects |
        Retro::Ranges::Views::Filter(&UObject::Implements<UDataStructHandleNode>) |
        Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<IDataStructHandleNode>) |
        Retro::Ranges::Views::Transform(&IDataStructHandleNode::GetStructType) |
        Retro::Ranges::Views::Filter(&Pokemon::Data::IsValidDataTableStruct) |
        Retro::Ranges::To<TSet>();
    // clang-format on

    auto WrappedProperty = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FDataStructHandle, RowID));
    if (StructTypes.Num() == 1) {
        auto Struct = *StructTypes.begin();
        auto RowProperty = Struct->FindPropertyByName(Pokemon::Data::DataStructRowID);
        WrappedProperty->GetProperty()->SetMetaData(TEXT("GetOptions"), *RowProperty->GetMetaData(TEXT("GetOptions")));
    } else {
        WrappedProperty->GetProperty()->RemoveMetaData(TEXT("GetOptions"));
    }

    // clang-format off
    HeaderRow.NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            WrappedProperty->CreatePropertyValueWidget()
        ];
    // clang-format on
}

void FDataHandleCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                 IDetailChildrenBuilder &ChildBuilder,
                                                 IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // No child customization
}