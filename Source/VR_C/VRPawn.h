// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionControllerComponent.h"
#include "GrabComponent.h"
#include "Menu.h"
#include "VRPawn.generated.h"

UCLASS()
class VR_C_API AVRPawn : public ACharacter
{
	GENERATED_BODY()

		/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* AnimationMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GrabLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* GrabRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Loccomotion, meta = (AllowPrivateAccess = "true"))
	float SnapTurnDegrees;

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& Value);
	void SnapTurn(const FInputActionValue& Value);
	void GrabLeft(const FInputActionValue& Value);
	void ReleaseLeft(const FInputActionValue& Value);
	void GrabRight(const FInputActionValue& Value);
	void ReleaseRight(const FInputActionValue& Value);
	void ToggleMenu();


	UCameraComponent* CameraComponent;
	UCapsuleComponent* CapsuleComponent;
	USceneComponent* VROriginComponent;
	UMotionControllerComponent* MC_LeftComponent;
	UMotionControllerComponent* MC_RightComponent;
	USkeletalMeshComponent* HandLeft;


	AMenu* MenuActor;

	void UpdateCapsulePosition();
	void UpdateCapsuleHeight();
	void InitializeCapsulePosition();
	UGrabComponent* GetGrabComponentNearMotionController(UMotionControllerComponent* MotionController);

	FTimerHandle UpdateCapsuleTimer;

	float CapsuleHalfHeight;

	UPROPERTY(EditAnywhere)
	float GrabRadius;

	UGrabComponent* HeldComponentLeft;
	UGrabComponent* HeldComponentRight;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMenu> InteractiveMenu;
};
