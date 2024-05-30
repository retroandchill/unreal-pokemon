// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MoveInfoWindow.generated.h"

struct FMoveData;
class UDisplayText;
class UImage;
class IMove;
enum class EMoveDamageCategory : uint8;
/**
 * Widget that displays information about a particular move.
 */
UCLASS(Abstract)
class POKEMONUI_API UMoveInfoWindow : public UUserWidget {
    GENERATED_BODY()

  public:
    /**
     * Refresh the information to the given move.
     * @param Move The move to display
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Display)
    void RefreshMove(const TScriptInterface<IMove> &Move);

  protected:
    /**
     * Get the damage text for the given move
     * @param MoveData The data of the move to examine.
     * @return The text to display
     */
    UFUNCTION(BlueprintNativeEvent, Category = Display)
    FText GetMoveDamageText(const FMoveData &MoveData) const;

    /**
     * Get the accuracy text for the given move
     * @param MoveData The data of the move to examine.
     * @return The text to display
     */
    UFUNCTION(BlueprintNativeEvent, Category = Display)
    FText GetMoveAccuracyText(const FMoveData &MoveData) const;

  private:
    /**
     * The graphics used to display the move category
     */
    UPROPERTY(
        EditAnywhere, Category = Display,
        meta = (AllowedClasses =
                    "/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
                DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
    TMap<EMoveDamageCategory, TObjectPtr<UObject>> CategoryGraphics;

    /**
     * The widget used for the category icon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> CategoryGraphic;

    /**
     * The widget used for the move's power
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PowerText;

    /**
     * The widget used for the move's accuracy
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> AccuracyText;

    /**
     * The widget used for the move's description
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> DescriptionText;
};
