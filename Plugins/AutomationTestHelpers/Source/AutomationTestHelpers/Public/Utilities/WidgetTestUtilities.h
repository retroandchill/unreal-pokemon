// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RAII.h"

#include "WidgetTestUtilities.generated.h"

class UWidget;
/**
 * Blueprint functions for getting widget children
 */
UCLASS()
class AUTOMATIONTESTHELPERS_API UWidgetTestUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Find a child widget by its name
     * @param Parent The parent widget
     * @param WidgetName The name of said widget
     * @return The found widget
     */
    UFUNCTION(BlueprintPure, Category = Widgets)
    static UWidget *FindChildWidget(UUserWidget *Parent, FName WidgetName);

    static std::tuple<TSharedRef<SOverlay>, FWorldPtr, FGameInstancePtr> CreateTestWorld();

    template <typename T>
        requires std::is_base_of_v<SWidget, T>
    static TSharedPtr<T> FindFirstChildOfType(SWidget &Widget, FName TypeName) {
        auto Children = Widget.GetChildren();
        if (Children == nullptr) {
            return nullptr;
        }

        for (int32 i = 0; i < Children->Num(); i++) {
            auto Child = Children->GetChildAt(i);
            if (Child->GetType() == TypeName) {
                return StaticCastSharedRef<T>(Child);
            }

            auto FoundChild = FindFirstChildOfType<T>(*Child, TypeName);
            if (FoundChild != nullptr) {
                return FoundChild;
            }
        }

        return nullptr;
    }
};

#define FIND_CHILD_WIDGET(Parent, Type, WidgetName)                                                                    \
    auto WidgetName = Cast<Type>(UWidgetTestUtilities::FindChildWidget(Parent, TEXT(#WidgetName)))

#define FIND_FIRST_CHILD_WIDGET_OF_TYPE(Parent, Type)                                                                  \
    UWidgetTestUtilities::FindFirstChildOfType<Type>(Parent, TEXT(#Type))