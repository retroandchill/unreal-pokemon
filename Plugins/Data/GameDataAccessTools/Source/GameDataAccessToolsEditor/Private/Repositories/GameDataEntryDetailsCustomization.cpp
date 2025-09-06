// Fill out your copyright notice in the Description page of Project Settings.

#include "Repositories/GameDataEntryDetailsCustomization.h"
#include "DetailLayoutBuilder.h"

TSharedRef<IDetailCustomization> FGameDataEntryDetailsCustomization::MakeInstance()
{
    return MakeShared<FGameDataEntryDetailsCustomization>();
}

void FGameDataEntryDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder &DetailBuilder)
{
    DetailBuilder.EditCategory("EntryInformation", NSLOCTEXT("GameDataEntry", "EntryInformation", "Entry Information"),
                               ECategoryPriority::Important);
}
