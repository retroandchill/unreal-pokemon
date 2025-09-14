// Fill out your copyright notice in the Description page of Project Settings.

#include "Runner/CSharpTestLatentCommand.h"
#include "Interop/ManagedTestingCallbacks.h"

FCSharpTestLatentCommand::FCSharpTestLatentCommand(TWeakPtr<FCSharpAutomationTest> Owner,
                                                   const FGCHandleIntPtr TestTask)
    : Owner(MoveTemp(Owner)), TestTask(TestTask)
{
}

bool FCSharpTestLatentCommand::Update()
{
    return FManagedTestingCallbacks::Get().CheckTaskComplete(TestTask);
}
