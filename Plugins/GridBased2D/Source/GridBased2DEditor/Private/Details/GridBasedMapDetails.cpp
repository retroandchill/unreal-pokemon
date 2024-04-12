// "Unreal Pokémon" created by Retro & Chill.

#include "Details/GridBasedMapDetails.h"

#include "DetailLayoutBuilder.h"

TSharedRef<IDetailCustomization> FGridBasedMapDetails::MakeInstance() { return MakeShared<FGridBasedMapDetails>(); }

void FGridBasedMapDetails::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) {
    DetailBuilder.EditCategory("Tile Map", FText::GetEmpty(), ECategoryPriority::Important);
    DetailBuilder.EditCategory("Audio", FText::GetEmpty(), ECategoryPriority::Important);
    DetailBuilder.EditCategory("Tiles", FText::GetEmpty(), ECategoryPriority::Important);
    DetailBuilder.EditCategory("Z-Sorting", FText::GetEmpty(), ECategoryPriority::Important);
    DetailBuilder.EditCategory("Connections", FText::GetEmpty(), ECategoryPriority::Important);
}
void FGridBasedMapDetails::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder> &DetailBuilder) {
    CustomizeDetails(*DetailBuilder);
}
