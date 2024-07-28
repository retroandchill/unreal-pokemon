// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"

#include "HPBar.generated.h"

/**
 * The style information for the HP bar
 */
USTRUCT(BlueprintType, DisplayName = "HP Bar Style")
struct FHPBarStyle {
    GENERATED_BODY()

    /**
     * The threshold that the percent needs to be under to use this style
     */
    UPROPERTY(EditAnywhere, Category = "Style|Fill", meta = (ClampMin = 0.f, UIMIn = 0.f, ClampMax = 1.f, UIMax = 1.f))
    float Threshold;

    /**
     * The image to use for the bar
     */
    UPROPERTY(EditAnywhere, Category = "Style|Fill")
    FSlateBrush Style;
};

/**
 * Progress Bar Widget that uses a special material instance to change the color of the bar based on the percent.
 */
UCLASS()
class POKEMONUI_API UHPBar : public UProgressBar, public FTickableGameObject {
    GENERATED_BODY()

  protected:
    TSharedRef<SWidget> RebuildWidget() override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

    void SynchronizeProperties() override;

  public:
    void Tick(float DeltaTime) override;
    TStatId GetStatId() const override;

  private:
    /**
     * Update the bar material to the new one
     */
    void UpdateBarMaterial();

    /**
     * The name of the parameter on the base material to substitute in for the state
     */
    UPROPERTY(EditAnywhere, Category = "Style|Fill")
    FName StateParameterName = TEXT("State");

    /**
     * The default style to use if none of the thresholds are met
     */
    UPROPERTY(EditAnywhere, Category = "Style|Fill")
    FSlateBrush DefaultStyle;

    /**
     * The thresholds used to drop the state down to a lower number
     */
    UPROPERTY(EditAnywhere, Category = "Style|Fill")
    TArray<FHPBarStyle> PercentThresholds;

    /**
     * The current state of the menu
     */
    TOptional<int32> CurrentState;

    /**
     * Get the previous percentage value for this object
     */
    float PreviousPercent = 0.f;

    /**
     * Flag that tells the game that the widget has been set up and can now tick
     */
    bool bSetUp = false;
};
