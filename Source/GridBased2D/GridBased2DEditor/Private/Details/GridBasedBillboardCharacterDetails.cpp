// "Unreal Pokémon" created by Retro & Chill.


#include "Details/GridBasedBillboardCharacterDetails.h"

#include "DetailLayoutBuilder.h"

TSharedRef<IDetailCustomization> FGridBasedBillboardCharacterDetails::MakeInstance() {
	return MakeShared<FGridBasedBillboardCharacterDetails>();
}

void FGridBasedBillboardCharacterDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {
	DetailBuilder.EditCategory("Rendering", FText::GetEmpty(), ECategoryPriority::Important);
}
