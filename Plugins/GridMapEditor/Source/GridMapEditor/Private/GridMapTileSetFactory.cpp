// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMapTileSetFactory.h"
#include "TileSet.h"

#define LOCTEXT_NAMESPACE "GridMapEditor"

UGridMapTileSetFactory::UGridMapTileSetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UGridMapTileSet::StaticClass();
}

UObject* UGridMapTileSetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UGridMapTileSet* NewTileSet = NewObject<UGridMapTileSet>(InParent, Class, Name, Flags | RF_Transactional);
	NewTileSet->PostEditChange();
	return NewTileSet;
}

#undef LOCTEXT_NAMESPACE
