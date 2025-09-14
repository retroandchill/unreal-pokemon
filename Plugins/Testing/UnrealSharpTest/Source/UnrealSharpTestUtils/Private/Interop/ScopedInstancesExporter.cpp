// Fill out your copyright notice in the Description page of Project Settings.

#include "Interop/ScopedInstancesExporter.h"

void UScopedInstancesExporter::RemovePlayer(ULocalPlayer *Player)
{
    Player->PlayerRemoved();
}
