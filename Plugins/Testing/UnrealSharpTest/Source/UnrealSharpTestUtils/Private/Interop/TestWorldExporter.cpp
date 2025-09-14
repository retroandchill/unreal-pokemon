// Fill out your copyright notice in the Description page of Project Settings.

#include "Interop/TestWorldExporter.h"

void UTestWorldExporter::InitializeTestWorld(FTestWorld &World, bool BeginPlay)
{
    std::construct_at(&World, BeginPlay);
}

void UTestWorldExporter::FinalizeTestWorld(FTestWorld &World)
{
    std::destroy_at(&World);
}

void UTestWorldExporter::BeginPlay(FTestWorld &World)
{
    World.BeginPlay();
}
