// "Unreal Pokémon" created by Retro & Chill.


#include "Pokemon/PokemonDTODetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Pokemon/PokemonDTO.h"

TSharedRef<IPropertyTypeCustomization> FPokemonDTODetails::MakeInstance() {
    return MakeShared<FPokemonDTODetails>();
}

void FPokemonDTODetails::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow &HeaderRow,
                                         IPropertyTypeCustomizationUtils &CustomizationUtils) {

    auto SpeciesHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPokemonDTO, Species));
    auto LevelHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPokemonDTO, Level));

    HeaderRow.NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        .MinDesiredWidth(200.0f)
        .MaxDesiredWidth(200.0f)
        [
            SNew(SGridPanel)
            + SGridPanel::Slot(0, 0)
            .Padding(FMargin(0.0f, 0.0f, 6.0f, 0.0f))
            .VAlign(VAlign_Center)
            [
                SpeciesHandle->CreatePropertyNameWidget()
            ]
            + SGridPanel::Slot(1, 0)
            .Padding(FMargin(6.0f, 0.0f, 3.0f, 0.0f))
            .VAlign(VAlign_Center)
            [
                SpeciesHandle->CreatePropertyValueWidget()
            ]
            + SGridPanel::Slot(0, 1)
            .Padding(FMargin(0.0f, 0.0f, 6.0f, 0.0f))
            .VAlign(VAlign_Center)
            [
                LevelHandle->CreatePropertyNameWidget()
            ]
            + SGridPanel::Slot(1, 1)
            .Padding(FMargin(6.0f, 0.0f, 3.0f, 0.0f))
            .VAlign(VAlign_Center)
            [
                LevelHandle->CreatePropertyValueWidget()
            ]
        ];
}

void FPokemonDTODetails::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                           IDetailChildrenBuilder &ChildBuilder,
                                           IPropertyTypeCustomizationUtils &CustomizationUtils) {
    auto SpeciesHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPokemonDTO, Species));
    auto LevelHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPokemonDTO, Level));

    uint32 ChildCount;
    PropertyHandle->GetNumChildren(ChildCount);
    for (uint32 i = 0; i < ChildCount; i++) {
        auto ChildHandle = PropertyHandle->GetChildHandle(i);
        if (ChildHandle->IsSamePropertyNode(SpeciesHandle) || ChildHandle->IsSamePropertyNode(LevelHandle)) {
            continue;
        }
        
        ChildBuilder.AddProperty(ChildHandle.ToSharedRef());
    }
}