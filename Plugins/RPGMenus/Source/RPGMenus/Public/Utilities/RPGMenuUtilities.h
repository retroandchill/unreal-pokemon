// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PrimaryGameLayout.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Lookup/InjectionUtilities.h"
#include "Screens/Screen.h"

#include "RPGMenuUtilities.generated.h"

struct FGameplayTag;
class UScreen;
/**
 *
 */
UCLASS()
class RPGMENUS_API URPGMenuUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    template <typename T, typename... A>
        requires RPG::Menus::InjectableScreen<T>
    static TOptional<T&> InjectScreenToLayer(const UObject* WorldContextObject, const FGameplayTag& LayerTag, A&&... Args) {
        auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
        
        if (auto Layer = Layout->GetLayerWidget(LayerTag); Layer != nullptr) {
            auto &Injection = UnrealInjector::TInjectionSettings<T>::Get();
            auto Widget = UnrealInjector::NewInjectedDependency<T, A...>(Layer, Forward<A>(Args)...);
            Layer->AddWidgetInstance(*Widget);
            return Widget;
        }

        return nullptr;
    }
    
    template <typename T, typename... A>
        requires RPG::Menus::InjectableScreen<T>
    static TOptional<T&> InjectScreenToStack(const UObject* WorldContextObject, A&&... Args) {
        return InjectScreenToLayer<T, A...>(WorldContextObject, RPG::Menus::PrimaryMenuLayerTag, Forward<A>(Args)...);
    }

    template <typename T, typename... A>
        requires RPG::Menus::InjectableScreen<T>
    static TOptional<T&> InjectScreenToOverlay(const UObject* WorldContextObject, A&&... Args) {
        return InjectScreenToLayer<T, A...>(WorldContextObject, RPG::Menus::OverlayMenuLayerTag, Forward<A>(Args)...);
    }
    
    /**
     * Helper function used to create a screen on the stack from Blueprints
     * @param WorldContextObject The world context object needed to get the subsystem
     * @param ScreenType The screen class to spawn
     * @return The created screen.
     */
    UFUNCTION(BlueprintCallable, Category = Screens,
              meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = ScreenType))
    static UScreen *PushScreenToStack(const UObject *WorldContextObject, TSubclassOf<UScreen> ScreenType);

    /**
     * Helper function used to create a screen on the stack from Blueprints
     * @param WorldContextObject The world context object needed to get the subsystem
     * @param ScreenType The screen class to spawn
     * @return The created screen.
     */
    UFUNCTION(BlueprintCallable, Category = Screens,
              meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = ScreenType))
    static UScreen *LoadAndPushScreenToStack(const UObject *WorldContextObject,
                                             const TSoftClassPtr<UScreen> &ScreenType);

    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = "WorldContextObject"))
    static UScreen *RemoveTopScreenFromStackLayer(const UObject *WorldContextObject, FGameplayTag Tag);

    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = "WorldContextObject"))
    static UScreen *RemoveTopScreenFromStack(const UObject *WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = "WorldContextObject"))
    static UScreen *RemoveTopScreenFromOverlay(const UObject *WorldContextObject);
};
