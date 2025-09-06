// Fill out your copyright notice in the Description page of Project Settings.

#include "World/WorldContextExtensions.h"
#include "CSManager.h"

void UWorldContextExtensions::SetWorldContext(UObject *Object)
{
    UCSManager::Get().SetCurrentWorldContext(Object);
}
