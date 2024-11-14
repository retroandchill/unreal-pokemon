// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#include "Notifies/PaperZDNotifyState_ParticleEffect.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystemComponent.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDNotifyState_ParticleEffect)
#endif

UPaperZDNotifyState_ParticleEffect::UPaperZDNotifyState_ParticleEffect()
{
	bDestroyAtEnd = false;
	Scale = FVector(1.f);

#if WITH_EDITORONLY_DATA
	Color = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA
}

void UPaperZDNotifyState_ParticleEffect::PostLoad()
{
	Super::PostLoad();

	RotationOffsetQuat = FQuat(RotationOffset);
}

#if WITH_EDITOR
void UPaperZDNotifyState_ParticleEffect::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UPaperZDNotifyState_ParticleEffect, RotationOffset))
	{
		RotationOffsetQuat = FQuat(RotationOffset);
	}
}
#endif

FName UPaperZDNotifyState_ParticleEffect::GetDisplayName_Implementation() const
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

void UPaperZDNotifyState_ParticleEffect::OnNotifyBegin_Implementation(UPaperZDAnimInstance* OwningInstance /* = nullptr */) const
{
	if (PSTemplate && SequenceRenderComponent.IsValid())
	{
		UParticleSystemComponent* Component = UGameplayStatics::SpawnEmitterAttached(PSTemplate, SequenceRenderComponent.Get(), SocketName, LocationOffset, RotationOffset, Scale, EAttachLocation::KeepRelativeOffset, !bDestroyAtEnd);
		if (Component)
		{
			Component->ComponentTags.AddUnique(GetSpawnedComponentTag());
		}
	}
	else if (!PSTemplate)
	{
		UObject* AnimSequencePkg = GetContainingAsset();
		UE_LOG(LogTemp, Warning, TEXT("Particle Notify: Particle system is null for particle notify '%s' in anim: '%s'"), *(GetDisplayName().ToString()), *GetPathNameSafe(AnimSequencePkg));
	}
}

void UPaperZDNotifyState_ParticleEffect::OnNotifyEnd_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	if (SequenceRenderComponent.IsValid())
	{
		TArray<USceneComponent*> Children;
		SequenceRenderComponent->GetChildrenComponents(false, Children);
		for (USceneComponent* Component : Children)
		{
			if (UParticleSystemComponent* ParticleComponent = Cast<UParticleSystemComponent>(Component))
			{
				if (ParticleComponent->ComponentHasTag(GetSpawnedComponentTag()))
				{
					// untag the component
					ParticleComponent->ComponentTags.Remove(GetSpawnedComponentTag());

					// Either destroy the component or deactivate it to have it's active particles finish.
					// The component will auto destroy once all particle are gone.
					if (bDestroyAtEnd)
					{
						ParticleComponent->DestroyComponent();
					}
					else
					{
						ParticleComponent->DeactivateSystem();
					}
				}
			}
		}
	}
}
