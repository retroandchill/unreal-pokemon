// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Animations/ProgressBarAnimation.h"
#include "Components/ProgressBar.h"

#include "HPBar.generated.h"

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
    void UpdateBarMaterial();

    /**
     * The base material used to generate the HP Bar Dynamic Material
     */
    UPROPERTY(EditAnywhere, Category = "Style|Fill")
    TObjectPtr<UMaterialInterface> BaseMaterial;

    /**
     * The name of the parameter on the base material to substitute in for the state
     */
    UPROPERTY(EditAnywhere, Category = "Style|Fill")
    FName StateParameterName = TEXT("State");

    /**
     * The thresholds used to drop the state down to a lower number
     */
    UPROPERTY(EditAnywhere, Category = "Style|Fill")
    TArray<float> PercentThresholds = {0.5f, 0.25f};

    /**
     * Flag that tells the game that the widget has been set up and can now tick
     */
    bool bSetUp = false;

    /**
     * The actual material held by the widget. This is used to dynamically update it during runtime without needing
     * to create a new one on each change
     */
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> BarMaterial;
    
};
