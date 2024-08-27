// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMapStaticMeshActor.h"

AGridMapStaticMeshActor::AGridMapStaticMeshActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
}