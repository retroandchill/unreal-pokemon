#include "GameDataRepository.h"

void UGameDataRepository::PostLoad() {
    UObject::PostLoad();
    RebuildIndices();
}

#if WITH_EDITOR
void UGameDataRepository::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    UObject::PostEditChangeProperty(PropertyChangedEvent);
    RebuildIndices();
}
#endif
