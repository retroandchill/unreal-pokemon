// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameDataRepository.generated.h"

/**
 *
 */
UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UGameDataRepository : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "GameDataRepository")
    const UScriptStruct* GetEntryStruct() const;
    
protected:
    void PostLoad() override;

    
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

    UFUNCTION(BlueprintImplementableEvent, Category = "GameDataRepository")
    void RebuildIndices();

private:
#if WITH_EDITOR
    friend class FGameDataRepositoryDataAccessor;
#endif
};
