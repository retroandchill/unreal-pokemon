// "Unreal Pokémon" created by Retro & Chill.


#include "Images/ImageAsset.h"

void FImageAsset::AddReferencedObjects(FReferenceCollector &Collector) {
    Collector.AddReferencedObject(GetObjectPtr());
}

FString FImageAsset::GetReferencerName() const {
    return TEXT("FImageAsset");
}

FImageAsset::FImageAsset(FIntrusiveUnsetOptionalState State) : TVariantObject(State) {
}

FImageAsset & FImageAsset::operator=(FIntrusiveUnsetOptionalState State) {
    TVariantObject::operator=(State);
    return *this;
}