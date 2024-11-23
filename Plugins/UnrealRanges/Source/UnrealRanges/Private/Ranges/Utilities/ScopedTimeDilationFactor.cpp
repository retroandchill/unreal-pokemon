// "Unreal Pokémon" created by Retro & Chill.

#include "Ranges/Utilities/ScopedTimeDilationFactor.h"
#include "Kismet/GameplayStatics.h"

using namespace UE::Ranges;

FScopedTimeDilationFactor::FScopedTimeDilationFactor(const UObject *WorldContextObject, float Factor) noexcept
    : WorldContextObject(WorldContextObject), Factor(Factor) {
    UGameplayStatics::SetGlobalTimeDilation(WorldContextObject,
                                            UGameplayStatics::GetGlobalTimeDilation(WorldContextObject) * Factor);
}

FScopedTimeDilationFactor::~FScopedTimeDilationFactor() {
    UGameplayStatics::SetGlobalTimeDilation(WorldContextObject,
                                            UGameplayStatics::GetGlobalTimeDilation(WorldContextObject) / Factor);
}

void FScopedTimeDilationFactor::AddReferencedObjects(FReferenceCollector &Collector) {
    Collector.AddReferencedObject(WorldContextObject);
}

FString FScopedTimeDilationFactor::GetReferencerName() const {
    return TEXT("FScopedTimeDilationFactor");
}