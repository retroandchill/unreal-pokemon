// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Lookup/InjectionUtilities.h"
#include "PrimaryGameLayout.h"
#include "Screens/Screen.h"

#include "RPGMenuUtilities.generated.h"

struct FGameplayTag;
class UScreen;
/**
 *
 */
UCLASS()
class RPGMENUS_API URPGMenuUtilities : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    template <typename T, typename... A>
        requires RPG::Menus::InjectableScreen<T>
    static TOptional<T &> InjectScreenToLayer(const UObject *WorldContextObject, const FGameplayTag &LayerTag,
                                              A &&...Args)
    {
        auto Layout = UPrimaryGameLayout::Get(WorldContextObject);

        if (auto Layer = Layout->GetLayerWidget(LayerTag); Layer != nullptr)
        {
            auto Widget = UnrealInjector::NewInjectedDependency<T, A...>(Layer, std::forward<A>(Args)...);
            Layer->AddWidgetInstance(*Widget);
            return Widget;
        }

        return nullptr;
    }

    template <typename T, typename... A>
        requires RPG::Menus::InjectableScreen<T>
    static TOptional<T &> InjectScreenToStack(const UObject *WorldContextObject, A &&...Args)
    {
        return InjectScreenToLayer<T, A...>(WorldContextObject, RPG::Menus::PrimaryMenuLayerTag,
                                            std::forward<A>(Args)...);
    }

    template <typename T, typename... A>
        requires RPG::Menus::InjectableScreen<T>
    static TOptional<T &> InjectScreenToOverlay(const UObject *WorldContextObject, A &&...Args)
    {
        return InjectScreenToLayer<T, A...>(WorldContextObject, RPG::Menus::OverlayMenuLayerTag,
                                            std::forward<A>(Args)...);
    }

    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = "WorldContextObject"))
    static UScreen *RemoveTopScreenFromStackLayer(const UObject *WorldContextObject, FGameplayTag Tag);

    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = "WorldContextObject"))
    static UScreen *RemoveTopScreenFromStack(const UObject *WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = "WorldContextObject"))
    static UScreen *RemoveTopScreenFromOverlay(const UObject *WorldContextObject);
};
