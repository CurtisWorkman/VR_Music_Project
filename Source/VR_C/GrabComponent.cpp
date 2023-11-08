// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();
	UPrimitiveComponent* Parent = Cast<UPrimitiveComponent>(GetAttachParent());
	SetShouldSimulateOnDrop(Parent);
	Parent->SetCollisionProfileName(TEXT("VRInteractable"));
	
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UGrabComponent::TryGrab(UMotionControllerComponent* MotionController)
{
	UPrimitiveComponent* Parent = Cast<UPrimitiveComponent>(GetAttachParent());
//	UE_LOG(LogTemp, Warning, TEXT("GrabType %s") );
	switch (GrabType)
	{
	case None:
		break;
	case Free:
		SetPrimitiveComponentPhysics(false, Parent);
		if (AttachParentToMotionController(MotionController, Parent))
		{
			bIsHeld = true;
		}
		break;

	case Snap:
		SetPrimitiveComponentPhysics(false, Parent);
		if (AttachParentToMotionController(MotionController, Parent))
		{
			bIsHeld = true;
		}
		SnapToMotionController(MotionController, Parent);
		break;

	case SnapWithCollision:
		SetPrimitiveComponentPhysics(false, Parent);
		AttachParentToMotionController(MotionController, Parent);		
		SnapToMotionController(MotionController, Parent);
		Parent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		UPhysicsConstraintComponent* PhysicsConstraint = Cast<UPhysicsConstraintComponent>(MotionController->GetChildComponent(1));
		if (PhysicsConstraint != nullptr)
		{
			USkeletalMeshComponent* Hand = Cast<USkeletalMeshComponent>(MotionController->GetChildComponent(0));
			if (Hand != nullptr)
			{
				PhysicsConstraint->SetConstrainedComponents(Hand, NAME_None, Parent, NAME_None);
				bIsHeld = true;
			}
		}
		SetPrimitiveComponentPhysics(true, Parent);
//		UE_LOG(LogTemp, Warning, TEXT("snap with collision"));
			break;
	}
	
	if (bIsHeld)
	{
		MotionControllerRef = MotionController;

		//Call On Pickup
		OnGrabbed.ExecuteIfBound();

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (OnGrabHapticEffect != nullptr)
		{
			PlayerController->PlayHapticEffect(OnGrabHapticEffect, GetHeldByHand());
		}
//		Parent->SetCollisionProfileName(TEXT("VRInteractable"));
		return true;
	}
	else
	{
		return false;
	}
}

bool UGrabComponent::TryRelease(UMotionControllerComponent* MotionController)
{
	UPrimitiveComponent* Parent = Cast<UPrimitiveComponent>(GetAttachParent());
	switch (GrabType)
	{
	case None:
		break;
	case Free:
	case Snap:
//		UE_LOG(LogTemp, Warning, TEXT("Free Snap"))
		if (SimulateOnDrop)
		{
			SetPrimitiveComponentPhysics(true, Parent);
			
		}
		else
		{
			Parent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		}
		bIsHeld = false;
		break;
	case SnapWithCollision:
		UPhysicsConstraintComponent* PhysicsConstraint = Cast<UPhysicsConstraintComponent>(MotionController->GetChildComponent(1));
		if (PhysicsConstraint != nullptr)
		{
			PhysicsConstraint->BreakConstraint();
			bIsHeld = false;
		}	
		break;
	}
	if (bIsHeld)
	{
		return false;
	}
	else
	{
//		Parent->SetCollisionProfileName(TEXT("PhysicsActor"));
		//Call On Dropped
		OnReleased.ExecuteIfBound();
		MotionControllerRef = nullptr;
		return true;
	}
}

void UGrabComponent::SetShouldSimulateOnDrop(UPrimitiveComponent* Parent)
{
	
	if (Parent != nullptr)
	{
		if (Parent->IsAnySimulatingPhysics())
		{
			SimulateOnDrop = true;
		}
	}
}

void UGrabComponent::SetPrimitiveComponentPhysics(bool Simulate, UPrimitiveComponent* Parent)
{
	Parent->SetSimulatePhysics(Simulate);
}

bool UGrabComponent::AttachParentToMotionController(UMotionControllerComponent* MotionController, UPrimitiveComponent* Parent)
{
	return Parent->AttachToComponent(MotionController, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
}

EControllerHand UGrabComponent::GetHeldByHand()
{
	if (MotionControllerRef != nullptr)
	{
//		UE_LOG(LogTemp, Warning, TEXT("Held"))
		return MotionControllerRef->GetTrackingSource();
	}
	return EControllerHand::Special;
}

void UGrabComponent::SnapToMotionController(UMotionControllerComponent* MotionController, UPrimitiveComponent* Parent)
{
	FRotator NewRotation =  (this->GetRelativeRotation())*-1;
	Parent->SetRelativeRotation(NewRotation, false, nullptr, ETeleportType::TeleportPhysics);
	FVector NewLocation = MotionController->GetComponentLocation() + ((this->GetComponentLocation() - Parent->GetComponentLocation()) * -1);
	Parent->SetWorldLocation(NewLocation);
}

