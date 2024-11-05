﻿// "Unreal Pokémon" created by Retro & Chill.


#include "Services/GameServiceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Services/Service.h"
#include "Services/ServiceUtilities.h"

void UGameServiceSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    UnrealInjector::GetAllServices() | UE::Ranges::ForEach([this](const TSubclassOf<UService> &Class) {
            Services.Emplace(Class, NewObject<UService>(this, Class));
        });
        
}

void UGameServiceSubsystem::Deinitialize() {
    Super::Deinitialize();
    Services.Empty();
}

UService * UGameServiceSubsystem::StaticGetService(const UObject *WorldContext,
    const TSubclassOf<UService> &ServiceClass) {
    return &GetService(WorldContext, ServiceClass);
    
}