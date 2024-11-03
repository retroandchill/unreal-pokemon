// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Views/AnyView.h"

class UService;

namespace UnrealInjector {

    /**
     * Retrieves a unique view of all available service classes.
     *
     * This method fetches and returns a list of all the services
     * that are currently available in the system.
     *
     * @return A list of classes, where each string represents the name of a service.
     */
    UNREALINJECTOR_API UE::Ranges::TAnyView<TSubclassOf<UService>> GetAllServices(); 
    
}
