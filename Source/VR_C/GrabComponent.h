// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MotionControllerComponent.h"
#include "GameFramework/PlayerController.h"
#include "GrabComponent.generated.h"

UENUM()
enum EGrabType
{
	None     UMETA(DisplayName = "None"),
	Free      UMETA(DisplayName = "Free"),
	Snap   UMETA(DisplayName = "Snap"),
	SnapWithCollision   UMETA(DisplayName = "SnapWithCollision"),
};

DECLARE_DELEGATE(FOnGrabbedDelegate);
DECLARE_DELEGATE(FOnReleasedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_C_API UGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool TryGrab(UMotionControllerComponent* MotionController);
	bool TryRelease(UMotionControllerComponent* MotionController);

//	UPROPERTY(BluePrintAssignable)
	FOnGrabbedDelegate OnGrabbed;
	FOnReleasedDelegate OnReleased;

	EControllerHand GetHeldByHand();

private:
	void SetShouldSimulateOnDrop(UPrimitiveComponent* AttachParent);
	void SetPrimitiveComponentPhysics(bool Simulate, UPrimitiveComponent* Parent);
	bool AttachParentToMotionController(UMotionControllerComponent* MotionController, UPrimitiveComponent* Parent);
	
	bool SimulateOnDrop = false;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EGrabType> GrabType;

	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* OnGrabHapticEffect;

	UMotionControllerComponent* MotionControllerRef;
	bool bIsHeld = false;

	void SnapToMotionController(UMotionControllerComponent* MotionController, UPrimitiveComponent* Parent);
		
};
