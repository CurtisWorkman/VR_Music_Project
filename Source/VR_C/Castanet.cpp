// Fill out your copyright notice in the Description page of Project Settings.


#include "Castanet.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ACastanet::ACastanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PhysicsStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Physics Static Mesh"));
	RootComponent = PhysicsStaticMesh;

	TopStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top Static Mesh"));
	TopStaticMesh->SetupAttachment(PhysicsStaticMesh);

	BottomStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom Static Mesh"));
	BottomStaticMesh->SetupAttachment(PhysicsStaticMesh);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Sound"));
	AudioComp->SetupAttachment(PhysicsStaticMesh);

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab"));
	GrabComponent->SetupAttachment(PhysicsStaticMesh);


}

// Called when the game starts or when spawned
void ACastanet::BeginPlay()
{
	Super::BeginPlay();
	GrabComponent->OnGrabbed.BindUObject(this, &ACastanet::OnGrabbed);
	GrabComponent->OnReleased.BindUObject(this, &ACastanet::OnReleased);

//	AddMappingContext(IMCCastanetRight);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(UGameplayStatics::GetPlayerController(this, 0)->InputComponent))
	{
		// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
		EnhancedInputComponent->BindAction(ClapLeftAction, ETriggerEvent::Started, this, &ACastanet::PlayCastanetLeft);
		EnhancedInputComponent->BindAction(ClapLeftAction, ETriggerEvent::Completed, this, &ACastanet::StopCastanetLeft);
		EnhancedInputComponent->BindAction(ClapRightAction, ETriggerEvent::Started, this, &ACastanet::PlayCastanetRight);
		EnhancedInputComponent->BindAction(ClapRightAction, ETriggerEvent::Completed, this, &ACastanet::StopCastanetRight);
		UE_LOG(LogTemp, Warning, TEXT("Input setup"))

	}
}


// Called every frame
void ACastanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACastanet::OnGrabbed()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EControllerHand Controller = GrabComponent->GetHeldByHand();
			switch (Controller)
			{
			case EControllerHand::Left:

				Subsystem->AddMappingContext(IMCCastanetLeft, 0);
				UE_LOG(LogTemp, Warning, TEXT("left"))
//				if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(UGameplayStatics::GetPlayerController(this, 0)->InputComponent))
//				{
//					// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
//					EnhancedInputComponent->BindAction(ClapLeftAction, ETriggerEvent::Started, this, &ACastanet::PlayCastanet);
//					EnhancedInputComponent->BindAction(ClapLeftAction, ETriggerEvent::Completed, this, &ACastanet::StopCastanet);
	//				EnhancedInputComponent->Remove
//					UE_LOG(LogTemp, Warning, TEXT("Input setup"))
//				}
				break;
			case EControllerHand::Right:
				Subsystem->AddMappingContext(IMCCastanetRight, 0);
//				if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(UGameplayStatics::GetPlayerController(this, 0)->InputComponent))
//				{
	//				// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	//				EnhancedInputComponent->BindAction(ClapRightAction, ETriggerEvent::Started, this, &ACastanet::PlayCastanet);
	//				EnhancedInputComponent->BindAction(ClapRightAction, ETriggerEvent::Completed, this, &ACastanet::StopCastanet);
	//				UE_LOG(LogTemp, Warning, TEXT("Input setup"))
//				}
				break;
			default:
				break;
			}
			bIsGrabbed = true;
		}
	}
}

void ACastanet::OnReleased()
{
	EControllerHand Controller = GrabComponent->GetHeldByHand();
	switch (Controller)
	{
	case EControllerHand::Left:
		RemoveMappingContext(IMCCastanetLeft);
		break;
	case EControllerHand::Right:

		RemoveMappingContext(IMCCastanetRight);
		break;
	default:
		break;
	}
	bIsGrabbed = false;
}

void ACastanet::AddMappingContext(UInputMappingContext* IMC_Castanet)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	EnableInput(PlayerController);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Castanet, 1);

		}
	}
}

void ACastanet::RemoveMappingContext(UInputMappingContext* IMC_Castanet)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	DisableInput(PlayerController);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(IMC_Castanet);
		}
	}
}

void ACastanet::PlayCastanetLeft()
{
	EControllerHand Controller = GrabComponent->GetHeldByHand();
	switch (Controller)
	{
	case EControllerHand::Left:
		if (!bIsGrabbed) return;
		UE_LOG(LogTemp, Warning, TEXT("Castanet"))
			if (AudioComp != nullptr)
			{
				AudioComp->Play();
			}
		PlayHapticEffectOnController(OnClapHapticEffect);

		//move both castanets
		TopStaticMesh->AddRelativeRotation(FRotator(0, 0, -5));
		BottomStaticMesh->AddRelativeRotation(FRotator(0, 0, 5));
		break;
	default:
		break;
	}
}

void ACastanet::StopCastanetLeft()
{
	EControllerHand Controller = GrabComponent->GetHeldByHand();
	switch (Controller)
	{
	case EControllerHand::Left:
		if (!bIsGrabbed) return;
		TopStaticMesh->AddRelativeRotation(FRotator(0, 0, 5));
		BottomStaticMesh->AddRelativeRotation(FRotator(0, 0, -5));
		break;
	default:
		break;
	}
}

void ACastanet::PlayCastanetRight()
{
	EControllerHand Controller = GrabComponent->GetHeldByHand();
	switch (Controller)
	{
	case EControllerHand::Right:
		if (!bIsGrabbed) return;
		UE_LOG(LogTemp, Warning, TEXT("Castanet"))
			if (AudioComp != nullptr)
			{
				AudioComp->Play();
			}
		PlayHapticEffectOnController(OnClapHapticEffect);

		//move both castanets
		TopStaticMesh->AddRelativeRotation(FRotator(0, 0, -5));
		BottomStaticMesh->AddRelativeRotation(FRotator(0, 0, 5));
		break;
	default:
		break;
	}
}

void ACastanet::StopCastanetRight()
{
	EControllerHand Controller = GrabComponent->GetHeldByHand();
	switch (Controller)
	{
	case EControllerHand::Right:
		if (!bIsGrabbed) return;
		TopStaticMesh->AddRelativeRotation(FRotator(0, 0, 5));
		BottomStaticMesh->AddRelativeRotation(FRotator(0, 0, -5));
		break;
	default:
		break;
	}
}

void ACastanet::PlayHapticEffectOnController(UHapticFeedbackEffect_Base* HitEffect)
{
	if (GetIsHeld())
	{
		//		UE_LOG(LogTemp, Warning, TEXT("PlayHaptic"));
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController != nullptr && HitEffect != nullptr)
		{
			PlayerController->PlayHapticEffect(HitEffect, GrabComponent->GetHeldByHand());
		}
	}
}

bool ACastanet::GetIsHeld()
{
	if (GrabComponent->GetHeldByHand() == EControllerHand::Special)
	{
		return false;
	}
	else
	{
		return true;
	}
}


