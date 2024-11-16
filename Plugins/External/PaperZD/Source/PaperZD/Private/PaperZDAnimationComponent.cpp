// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"
#include "Components/PrimitiveComponent.h"
#include "PaperZDCustomVersion.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimationComponent)
#endif

// Sets default values for this component's properties
UPaperZDAnimationComponent::UPaperZDAnimationComponent()
	: AnimInstanceClass(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UPaperZDAnimationComponent::PostLoad()
{
	Super::PostLoad();

	//Apply the patches to different versions.
	const int32 ZDVersion = GetLinkerCustomVersion(FPaperZDCustomVersion::GUID);
	if (ZDVersion < FPaperZDCustomVersion::AddedCustomComponentPicker)
	{
		//Use the new component ref type which uses the new and improved picker
		RenderComponentRef.ComponentProperty = RenderComponent_DEPRECATED.ComponentProperty;
		RenderComponentRef.PathToComponent = RenderComponent_DEPRECATED.PathToComponent;
		RenderComponentRef.OtherActor = nullptr;
		RenderComponentRef.OverrideComponent = nullptr;
	}
}

void UPaperZDAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	//Create a fresh AnimInstance object
	CreateAnimInstance();
}

void UPaperZDAnimationComponent::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	Ar.UsingCustomVersion(FPaperZDCustomVersion::GUID);
}

void UPaperZDAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Need to update the AnimInstance
	if (AnimInstance)
	{
		AnimInstance->Tick(DeltaTime);
	}
}

AActor* UPaperZDAnimationComponent::GetOwningActor() const
{
	return GetOwner();
}

UPrimitiveComponent* UPaperZDAnimationComponent::GetRenderComponent() const
{
	return Cast<UPrimitiveComponent>(RenderComponentRef.GetComponent(GetOwner()));
}

TSubclassOf<UPaperZDAnimInstance> UPaperZDAnimationComponent::GetSequencerAnimInstanceClass() const
{
	return AnimInstanceClass;
}

UPaperZDAnimInstance* UPaperZDAnimationComponent::GetSequencerAnimInstance()
{
	return GetOrCreateAnimInstance();
}

UPaperZDAnimInstance* UPaperZDAnimationComponent::GetOrCreateAnimInstance()
{
	if (!AnimInstance || !AnimInstance->GetPlayer())
	{
		CreateAnimInstance();
	}

	return AnimInstance;
}

void UPaperZDAnimationComponent::CreateAnimInstance()
{
	if (AnimInstanceClass)
	{
		AnimInstance = NewObject<UPaperZDAnimInstance>(this, AnimInstanceClass);
		AnimInstance->Init(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No animation class defined on '%s', cannot create instance."), *GetName());
	}
}

void UPaperZDAnimationComponent::InitRenderComponent(UPrimitiveComponent* InRenderComponent)
{
	const FString PathToComponent = *InRenderComponent->GetPathName(GetOwner());
	RenderComponentRef.PathToComponent = PathToComponent;

	//We need to also set the old deprecated component, because if this call was used on a constructor to set the correct path
	//the PostLoad implementation will then override the new RenderComponent data with the old one, during the version upgrade process.
	//If this hasn't been setup by that point, it will effectively clear the RenderComponentRef during upgrade.
	RenderComponent_DEPRECATED.PathToComponent = PathToComponent;
}

void UPaperZDAnimationComponent::SetAnimInstanceClass(TSubclassOf<UPaperZDAnimInstance> InAnimInstanceClass)
{
	AnimInstanceClass = InAnimInstanceClass;

	//Potentially re-create the anim instance, only if we already initialized the AnimInstance
	AnimInstance = nullptr;
	CreateAnimInstance();
}

void UPaperZDAnimationComponent::InitAnimInstanceClass(TSubclassOf<UPaperZDAnimInstance> InAnimInstanceClass)
{
	AnimInstanceClass = InAnimInstanceClass;
	AnimInstance = nullptr;
}
