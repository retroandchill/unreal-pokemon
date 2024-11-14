// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Notifies/PaperZDAnimNotify_Base.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "PaperZDAnimInstance.h"
#include "UObject/Package.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNotify_Base)
#endif

UPaperZDAnimNotify_Base::UPaperZDAnimNotify_Base(const FObjectInitializer& ObjectInitializer)
	: Super()
{
	Name = FName(TEXT("BaseNotify"));
	Color = FLinearColor::Red;
	
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif
}

UWorld* UPaperZDAnimNotify_Base::GetWorld() const
{
	return SequenceRenderComponent.IsValid() ? SequenceRenderComponent->GetWorld() : NULL;
}

FName UPaperZDAnimNotify_Base::GetDisplayName_Implementation() const
{
	return Name;
}

void UPaperZDAnimNotify_Base::TickNotify(float DeltaTime, float Playtime, float LastPlaybackTime, UPrimitiveComponent* AnimRenderComponent, bool& bPersistentActiveState, UPaperZDAnimInstance* OwningInstance /* = nullptr */) const
{
	SequenceRenderComponent = AnimRenderComponent;
}

UObject* UPaperZDAnimNotify_Base::GetContainingAsset() const
{
	UObject* ContainingAsset = GetTypedOuter<UPaperZDAnimSequence>();
	if (ContainingAsset == nullptr)
	{
		ContainingAsset = GetOutermost();
	}
	return ContainingAsset;
}
