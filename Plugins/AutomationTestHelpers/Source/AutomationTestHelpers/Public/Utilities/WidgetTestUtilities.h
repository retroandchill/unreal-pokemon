// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/WidgetTree.h"
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

    /**
     * Find all child widgets on a given widget with a particular type
     * @tparam T The type of widget we're filtering for
     * @param Parent The parent widget that contains the child
     * @return The filter list of widgets
     */
    template <typename T>
        requires std::is_base_of_v<UWidget, T>
    static TArray<T *> FindAllChildWidgetsOfType(UUserWidget *Parent) {
        TArray<UWidget *> AllChildren;
        UWidgetTree::GetChildWidgets(Parent->GetRootWidget(), AllChildren);

        TArray<T *> Ret;
        for (auto Child : AllChildren) {
            if (Child->IsA<T>()) {
                Ret.Add(CastChecked<T>(Child));
            }
        }
        return Ret;
    }

    /**
     * Create the context for a test that requires a world
     * @return A tuple containing the overlay for widgets, a pointer to the world, and a pointer to the game instance
     */
    static std::tuple<TSharedRef<SOverlay>, FWorldPtr, FGameInstancePtr> CreateTestWorld(bool bBeginPlay = true);

    /**
     * Find the first child of a given type in a slate hirearchy
     * @tparam T The type we're looking for
     * @param Widget The parent widget
     * @param TypeName The name of the actual type
     * @return The found widget
     */
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