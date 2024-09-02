// "Unreal Pokémon" created by Retro & Chill.


#include "Details/DataHandleCustomization.h"
#include "DetailWidgetRow.h"
#include "Data/DataStructHandleNode.h"
#include "DataRetrieval/DataStructHandle.h"
#include "Ranges/Algorithm/ToSet.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/ContainerView.h"


TSharedRef<IPropertyTypeCustomization> FDataHandleCustomization::MakeInstance() {
    return MakeShared<FDataHandleCustomization>();
}

void FDataHandleCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
    IPropertyTypeCustomizationUtils &CustomizationUtils) {
    TArray<UObject*> OuterObjects;
    PropertyHandle->GetOuterObjects(OuterObjects);
    // clang-format off
    auto StructTypes = OuterObjects |
        UE::Ranges::Filter(&UObject::Implements<UDataStructHandleNode>) |
        UE::Ranges::CastType<IDataStructHandleNode> |
        UE::Ranges::Map(&IDataStructHandleNode::GetStructType) |
        UE::Ranges::Filter(&Pokemon::Data::IsValidDataTableStruct) |
        UE::Ranges::ToSet;
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
    IDetailChildrenBuilder &ChildBuilder, IPropertyTypeCustomizationUtils &CustomizationUtils) {
    // No child customization
}