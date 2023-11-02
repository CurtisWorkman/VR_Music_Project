// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	bool HMDEnabled = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
	if (HMDEnabled) {
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);
		UE_LOG(LogTemp, Warning, TEXT("Enabled"))
	}
	UKismetSystemLibrary::ExecuteConsoleCommand(this, FString(TEXT("vr.PixelDensity 1.0")));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			Subsystem->AddMappingContext(AnimationMappingContext, 1);
		}
		CameraComponent = Cast<UCameraComponent>(GetDefaultSubobjectByName(TEXT("Camera")));
		CapsuleComponent = Cast<UCapsuleComponent>(GetDefaultSubobjectByName(TEXT("CollisionCylinder")));
		VROriginComponent = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("VROrigin")));
		MC_LeftComponent = Cast<UMotionControllerComponent>(GetDefaultSubobjectByName(TEXT("MC_Left")));
		MC_RightComponent = Cast<UMotionControllerComponent>(GetDefaultSubobjectByName(TEXT("MC_Right")));
	}
	InitializeCapsulePosition();
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVRPawn::Move);
		EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Started, this, &AVRPawn::GrabLeft);
		EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Completed, this, &AVRPawn::ReleaseLeft);
		EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Started, this, &AVRPawn::GrabRight);
		EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Completed, this, &AVRPawn::ReleaseRight);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Started, this, &AVRPawn::SnapTurn);
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &AVRPawn::ToggleMenu);
	}
}

void AVRPawn::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && CameraComponent != nullptr)
	{
		// add movement 		
	//	FVector DeltaLocation = CameraComponent->GetForwardVector() * FVector(Value.Get<FVector2D>().Y, Value.Get<FVector2D>().X, 0) * 100 * UGameplayStatics::GetWorldDeltaSeconds(this);
	//	AddActorWorldOffset(DeltaLocation, true);
		UpdateCapsulePosition();
		AddMovementInput(CameraComponent->GetForwardVector(), MovementVector.Y);
		AddMovementInput(CameraComponent->GetRightVector(), MovementVector.X);
		
	}
}

void AVRPawn::SnapTurn(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	float SnapTurnDegreesSend = SnapTurnDegrees;
	if (AxisValue < 0) {
		SnapTurnDegreesSend = -SnapTurnDegrees;
	}
	AddActorWorldRotation(FRotator(0, SnapTurnDegreesSend, 0), false, nullptr, ETeleportType::TeleportPhysics);
}

void AVRPawn::GrabLeft(const FInputActionValue& Value)
{
	UGrabComponent* GrabComponent = GetGrabComponentNearMotionController(MC_LeftComponent);
	if (GrabComponent != nullptr)
	{
		if (GrabComponent->TryGrab(MC_LeftComponent))
		{
			HeldComponentLeft = GrabComponent;
//			if (HeldComponentLeft == HeldComponentRight)
//			{
	//			HeldComponentRight = nullptr;
//			}
		}
	}
}

void AVRPawn::ReleaseLeft(const FInputActionValue& Value)
{
	if (HeldComponentLeft != nullptr)
	{
		if (HeldComponentLeft->TryRelease(MC_LeftComponent))
		{
			HeldComponentLeft = nullptr;
		}
	}
}

void AVRPawn::GrabRight(const FInputActionValue& Value)
{
	UGrabComponent* GrabComponent = GetGrabComponentNearMotionController(MC_RightComponent);
	if (GrabComponent != nullptr)
	{
		if (GrabComponent->TryGrab(MC_RightComponent))
		{
			HeldComponentRight = GrabComponent;
//			if (HeldComponentRight == HeldComponentLeft)
//			{
//				HeldComponentLeft = nullptr;
//			}
		}
	}
}

void AVRPawn::ReleaseRight(const FInputActionValue& Value)
{
	if (HeldComponentRight != nullptr)
	{
		if (HeldComponentRight->TryRelease(MC_RightComponent))
		{
			HeldComponentRight = nullptr;
		}
	}
}

void AVRPawn::ToggleMenu()
{
	if (IsValid(MenuActor))
	{
		MenuActor->CloseMenu();
		UE_LOG(LogTemp, Warning, TEXT("Close"))
	}
	else
	{
		FVector Location(0, 0, 0);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FVector TransformScale(1, 1, 1);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		MenuActor = GetWorld()->SpawnActor<AMenu>(InteractiveMenu, Location, Rotation, SpawnInfo);
	}
}



UGrabComponent* AVRPawn::GetGrabComponentNearMotionController(UMotionControllerComponent* MotionController)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray; // object types to trace
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	FVector LocalGripPosition = MotionController->GetComponentLocation();
	FHitResult OutHitResult;
	TArray<AActor*, FDefaultAllocator> ignoreActors;
	bool bHasHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), LocalGripPosition, LocalGripPosition, GrabRadius, objectTypesArray, false, ignoreActors, EDrawDebugTrace::None, OutHitResult, true);

	if (bHasHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Physics Body"));
		AActor* HitComponent = OutHitResult.GetActor();
		TSet<UActorComponent*> ActorComponents = HitComponent->GetComponents();
		float LowestNearestComponentDistance = 99999;
		UGrabComponent* LocalNearestGrabComponent = nullptr;
		for (UActorComponent* ActorComponent : ActorComponents)
		{
			UGrabComponent* GrabComponent = Cast<UGrabComponent>(ActorComponent);
			if (GrabComponent != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found GrabComponent"));
				float ComponentDistance = FVector(GrabComponent->GetComponentLocation() - LocalGripPosition).Size();
				if (ComponentDistance < LowestNearestComponentDistance)
				{
					LowestNearestComponentDistance = ComponentDistance;
					LocalNearestGrabComponent = GrabComponent;
				}

			}
		}
		return LocalNearestGrabComponent;
	}
	return nullptr;
}

void AVRPawn::UpdateCapsulePosition()
{
	if (CapsuleComponent != nullptr)
	{
		FVector CameraLocation = CameraComponent->GetComponentLocation();
		FVector CapsuleLocation = CapsuleComponent->GetComponentLocation();
		FVector DeltaLocation = FVector(CameraLocation.X - CapsuleLocation.X, CameraLocation.Y - CapsuleLocation.Y, 0);
		AddActorWorldOffset(DeltaLocation, false, nullptr, ETeleportType::TeleportPhysics);	
		if (VROriginComponent != nullptr)
		{
			VROriginComponent->AddWorldOffset(-DeltaLocation, true);
		}
	}	
}

void AVRPawn::InitializeCapsulePosition()
{
	GetWorldTimerManager().SetTimer(UpdateCapsuleTimer, this, &AVRPawn::UpdateCapsuleHeight, 0.02f, true);
	CapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
}



void AVRPawn::UpdateCapsuleHeight()
{
	CapsuleComponent->SetCapsuleSize(CapsuleComponent->GetScaledCapsuleRadius(), CapsuleHalfHeight, true);
	FRotator DeviceRotation{};
	FVector DevicePosition{};

	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(DeviceRotation, DevicePosition);
	CapsuleHalfHeight = CapsuleHalfHeight - DevicePosition.Z / 2;
	VROriginComponent->AddRelativeLocation(FVector(0, 0, CapsuleHalfHeight));
	CapsuleHalfHeight = DevicePosition.Z / 2;
}