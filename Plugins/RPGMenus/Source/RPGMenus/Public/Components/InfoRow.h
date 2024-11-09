// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InfoRow.generated.h"

/**
 * @brief An abstract class for displaying information in a row format.
 *
 * UInfoRow is a subclass of UCommonUserWidget that provides functionality for displaying information
 * using common UI components. It manages the visibility of the row based on certain initial settings
 * and delegates.
 */
UCLASS(Abstract)
class RPGMENUS_API UInfoRow : public UCommonUserWidget {
    GENERATED_BODY()

protected:
    void NativePreConstruct() override;

public:
    /**
     * @brief Updates the visibility of the UInfoRow widget based on its initial visibility settings.
     *
     * This function determines the appropriate visibility state for the widget. During design time in the editor, it directly
     * sets the visibility to the initial value. At runtime, it uses the InitialVisibilityDelegate to determine the visibility
     * if the delegate is bound and valid. If the delegate is not bound or uninitialized, it defaults to the InitialVisibility value.
     *
     * @note This function is automatically called during the Native Pre-Construct phase of widget initialization.
     */
    UFUNCTION(BlueprintCallable, Category = Visibility)
    void UpdateVisibility();

private:
    /**
     * @brief Specifies the initial visibility state of the UInfoRow widget.
     *
     * The InitialVisibility variable determines the default visibility of the UInfoRow widget
     * when it is first constructed. This value is used both during design time in the editor and
     * at runtime if no delegate is bound to dynamically determine the widget's visibility.
     *
     * @see UpdateVisibility
     */
    UPROPERTY(EditAnywhere, Category = Visibility)
    ESlateVisibility InitialVisibility = ESlateVisibility::SelfHitTestInvisible;

    UPROPERTY()
    FGetSlateVisibility InitialVisibilityDelegate;

};
