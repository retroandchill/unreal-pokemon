// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#include "Notifies/PaperZDAnimNotify_NiagaraEffect.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNotify_NiagaraEffect)
#endif

UPaperZDAnimNotify_NiagaraEffect::UPaperZDAnimNotify_NiagaraEffect()
{
	bAttached = true;
	Scale = FVector(1.f);

#if WITH_EDITORONLY_DATA
	Color = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA
}

void UPaperZDAnimNotify_NiagaraEffect::PostLoad()
{
	Super::PostLoad();

	RotationOffsetQuat = FQuat(RotationOffset);
}

#if WITH_EDITOR
void UPaperZDAnimNotify_NiagaraEffect::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UPaperZDAnimNotify_NiagaraEffect, RotationOffset))
	{
		RotationOffsetQuat = FQuat(RotationOffset);
	}
}
#endif

FName UPaperZDAnimNotify_NiagaraEffect::GetDisplayName_Implementation() const
{
	if (PSTemplate)
	{
		return FName(*PSTemplate->GetName());
	}
	else
	{
		return Super::GetDisplayName_Implementation();
	}
}

void UPaperZDAnimNotify_NiagaraEffect::OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance /* = nullptr */) const
{
	if (PSTemplate && SequenceRenderComponent.IsValid())
	{
		if (PSTemplate->IsLooping())
		{
			UObject* AnimSequencePkg = GetContainingAsset();
			UE_LOG(LogTemp, Warning, TEXT("Particle Notify: Particle Notify : Anim '%s' tried to spawn infinitely looping particle system '%s'. Spawning suppressed."), *GetNameSafe(AnimSequencePkg), *GetNameSafe(PSTemplate));
			return;
		}

		if (bAttached)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(PSTemplate, SequenceRenderComponent.Get(), SocketName, LocationOffset, RotationOffset, Scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);
		}
		else
		{
			const FTransform Transform = SequenceRenderComponent->GetSocketTransform(SocketName);
			UFXSystemComponent* System = UNiagaraFunctionLibrary::SpawnSystemAtLocation(SequenceRenderComponent.Get(), PSTemplate, Transform.TransformPosition(LocationOffset), (Transform.GetRotation() * RotationOffsetQuat).Rotator(), Scale, true);
		}
	}
	else if (!PSTemplate)
	{
		UObject* AnimSequencePkg = GetContainingAsset();
		UE_LOG(LogTemp, Warning, TEXT("Particle Notify: Particle system is null for particle notify '%s' in anim: '%s'"), *(GetDisplayName().ToString()), *GetPathNameSafe(AnimSequencePkg));
	}
}

