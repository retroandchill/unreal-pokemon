// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"

#include "GameDataRepositoryFactory.generated.h"

/**
 * @brief A factory class responsible for creating new instances of game data repository assets.
 *
 * UGameDataRepositoryFactory allows users to configure and select a specific class of game data repository asset
 * and create new objects of the selected type.
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataRepositoryFactory : public UFactory
{
    GENERATED_BODY()

  public:
    /**
     * @brief Default constructor for UGameDataRepositoryFactory.
     *
     * Initializes the factory with default settings to enable creating and editing
     * new instances of UAssetGameDataRepository.
     */
    UGameDataRepositoryFactory();

    bool ConfigureProperties() override;

    UObject *FactoryCreateNew(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags, UObject *Context,
                              FFeedbackContext *Warn) override;

  private:
    /**
     * @brief Represents the class type of an asset to be created by the factory.
     *
     * AssetClass is utilized to determine the specific class type of game data repository
     * that the factory should create. This allows for dynamic asset creation based on the
     * selected class type during configuration.
     */
    UPROPERTY()
    TObjectPtr<UClass> AssetClass;
};
