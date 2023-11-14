// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "EnhancedInputComponent.h"
#include "VRPawn.h"

// Sets default values
AMenu::AMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	InteractiveMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interactive Menu"));
	InteractiveMenu->SetupAttachment(RootComp);
	InteractiveMenu->SetDrawSize(FVector2D(500, 500));

	MenuLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Menu Laser"));
	MenuLaser->SetupAttachment(RootComp);

	Cursor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cursor"));
	Cursor->SetupAttachment(InteractiveMenu);


}


// Called when the game starts or when spawned
void AMenu::BeginPlay()
{
	Super::BeginPlay();

	bMenuOpening = true;

	if (MenuCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("DoTick"));
		TimelineFinishedCallback.BindUFunction(this, FName{ TEXT("FinCurve") });
		MyTimeline.AddInterpFloat(MenuCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
				Subsystem->AddMappingContext(MenuMappingContext, 2);	
		}
	}

	SetWidgetInteractionReferences();
	SetMotionControllerReference();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Setup"));
		EnhancedInputComponent->BindAction(MenuInteractRight, ETriggerEvent::Started, this, &AMenu::OnStartInteractRight);
		EnhancedInputComponent->BindAction(MenuInteractRight, ETriggerEvent::Completed, this, &AMenu::OnCompleteInteractRight);
	}

	WidgetRelativeScaleOriginal = InteractiveMenu->GetRelativeScale3D().X;

	MyTimeline.PlayFromStart();
}

void AMenu::CloseMenu()
{
	bMenuOpening = false;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(MenuMappingContext);			//Take this out or do checks when other menu is open
		}
	}

	MyTimeline.Reverse();
//a	MyTimeline.PlayFromStart();
}

// Called to bind functionality to input
//void AMenu::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Input Setup"));
//	}
//}

// Called every frame
void AMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);

	if (MotionControllerAimRef != nullptr)
	{
		//Set Location Of Menu
		FVector ControllerLocation = MotionControllerAimRef->GetComponentLocation();
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		FVector CameraLocation = CameraManager->GetCameraLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ControllerLocation, CameraLocation);
		FVector ForwardLocation = UKismetMathLibrary::GetForwardVector(LookAtRotation) * 10;
		FVector UpLocation = UKismetMathLibrary::GetUpVector(LookAtRotation) * 10;
		FVector MoveMenuLocation = ControllerLocation + ForwardLocation + UpLocation;
		InteractiveMenu->SetWorldLocation(MoveMenuLocation);

		//Set Rotation of Menu
		FRotator MenuLookAtRotation = UKismetMathLibrary::FindLookAtRotation(InteractiveMenu->GetComponentLocation(), CameraLocation);
		InteractiveMenu->SetWorldRotation(MenuLookAtRotation);

		//
		if (GetWidgetInteractionComponent(!bActiveMenuHandRight)->IsOverHitTestVisibleWidget())
		{
			if (bDoOnceEnableVis)
			{
				FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
				MenuLaser->SetVisibility(true);
				Cursor->SetVisibility(true);
				GetWidgetInteractionComponent(bActiveMenuHandRight)->AttachToComponent(MotionControllerAimRef, AttachmentRules);
				bDoOnceEnableVis = false;
				bDoOnceDisableVis = true;
			}
			UWidgetInteractionComponent* WidgetInteractionComponent = GetWidgetInteractionComponent(!bActiveMenuHandRight);
			FVector CursorLocation = WidgetInteractionComponent->GetLastHitResult().ImpactPoint;
			Cursor->SetWorldLocation(CursorLocation);
			TArray<FVector> VectorArray = { WidgetInteractionComponent->GetComponentLocation(), CursorLocation};
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(MenuLaser, TEXT("User.PointArray"), VectorArray);
	
		}
		else
		{
			if (bDoOnceDisableVis)
			{
				MenuLaser->SetVisibility(false);
				Cursor->SetVisibility(false);
				bDoOnceEnableVis = true;
				bDoOnceDisableVis = false;
			}
		}
	}
}

void AMenu::SetWidgetInteractionReferences()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TArray<UWidgetInteractionComponent*> WidgetInteractionComponents;
	PlayerPawn->GetComponents<UWidgetInteractionComponent>(WidgetInteractionComponents);
	for (UWidgetInteractionComponent* InteractionComponent : WidgetInteractionComponents)
	{
		if (InteractionComponent->PointerIndex == 0)
		{
			WidgetInteractionRefLeft = InteractionComponent;
		}
		else
		{
			WidgetInteractionRefRight = InteractionComponent;
		}
	}

}

void AMenu::SetMotionControllerReference()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TArray<UMotionControllerComponent*> MotionControllerComponents;
	PlayerPawn->GetComponents<UMotionControllerComponent>(MotionControllerComponents);
	FName MotionSourceText;
	if (bActiveMenuHandRight == false)
	{
		MotionSourceText = TEXT("LeftAim");
	}
	else
	{
		MotionSourceText = TEXT("RightAim");
	}
	for (UMotionControllerComponent* MotionControllerComponent : MotionControllerComponents)
	{
		if (MotionControllerComponent->MotionSource == MotionSourceText)
		{
			MotionControllerAimRef = MotionControllerComponent;
		}

	}

}

void AMenu::DoTick()
{
	TimelineValue = MyTimeline.GetPlaybackPosition();
	InteractiveMenu->SetRelativeScale3D(FVector(TimelineValue * WidgetRelativeScaleOriginal));
	UE_LOG(LogTemp, Warning, TEXT("Curve %f"), TimelineValue);
}

void AMenu::FinCurve()
{
	FVector2f WidgetSize = InteractiveMenu->GetUserWidgetObject()->GetPaintSpaceGeometry().GetAbsoluteSize();
	CursorLimitX = WidgetSize.X / 2;
	CursorLimitY = WidgetSize.Y / 2;

	if (!bMenuOpening)
	{
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		GetWidgetInteractionComponent(bActiveMenuHandRight)->AttachToComponent(MotionControllerAimRef, AttachmentRules);
		bool bIsDestroyed = Destroy();
		if (bIsDestroyed)
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroyed"));
		}
	}
}

UWidgetInteractionComponent* AMenu::GetWidgetInteractionComponent(bool bActiveMenuRight)
{
	if (bActiveMenuRight == false)
	{
		return WidgetInteractionRefLeft;
	}
	else
	{
		return WidgetInteractionRefRight;
	}
}

void AMenu::OnCompleteInteractRight()
{
	WidgetInteractionRefRight->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AMenu::OnStartInteractRight()
{
	WidgetInteractionRefRight->PressPointerKey(EKeys::LeftMouseButton);
}

