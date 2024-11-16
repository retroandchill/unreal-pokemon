// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDAnimNotify_ParticleEffect.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "ParticleHelper.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimNotify_ParticleEffect)
#endif

UPaperZDAnimNotify_ParticleEffect::UPaperZDAnimNotify_ParticleEffect()
{
	bAttached = true;
	Scale = FVector(1.f);

#if WITH_EDITORONLY_DATA
	Color = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA
}

void UPaperZDAnimNotify_ParticleEffect::PostLoad()
{
	Super::PostLoad();

	RotationOffsetQuat = FQuat(RotationOffset);
}

#if WITH_EDITOR
void UPaperZDAnimNotify_ParticleEffect::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UPaperZDAnimNotify_ParticleEffect, RotationOffset))
	{
		RotationOffsetQuat = FQuat(RotationOffset);
	}
}
#endif

FName UPaperZDAnimNotify_ParticleEffect::GetDisplayName_Implementation() const
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

void UPaperZDAnimNotify_ParticleEffect::OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance /* = nullptr */) const
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
			UGameplayStatics::SpawnEmitterAttached(PSTemplate, SequenceRenderComponent.Get(), SocketName, LocationOffset, RotationOffset, Scale);
		}
		else
		{
			const FTransform Transform = SequenceRenderComponent->GetSocketTransform(SocketName);
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(Transform.TransformPosition(LocationOffset));
			SpawnTransform.SetRotation(Transform.GetRotation() * RotationOffsetQuat);
			SpawnTransform.SetScale3D(Scale);
			UParticleSystemComponent* System = UGameplayStatics::SpawnEmitterAtLocation(SequenceRenderComponent->GetWorld(), PSTemplate, SpawnTransform);
		}
	}
	else if (!PSTemplate)
	{
		UObject* AnimSequencePkg = GetContainingAsset();
		UE_LOG(LogTemp, Warning, TEXT("Particle Notify: Particle system is null for particle notify '%s' in anim: '%s'"), *(GetDisplayName().ToString()), *GetPathNameSafe(AnimSequencePkg));
	}
}

