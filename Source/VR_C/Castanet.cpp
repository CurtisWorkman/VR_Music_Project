// Fill out your copyright notice in the Description page of Project Settings.


#include "Castanet.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ACastanet::ACastanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Sound"));
	AudioComp->SetupAttachment(StaticMesh);

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab"));
	GrabComponent->SetupAttachment(StaticMesh);


}

// Called when the game starts or when spawned
void ACastanet::BeginPlay()
{
	Super::BeginPlay();
	GrabComponent->OnGrabbed.BindUObject(this, &ACastanet::OnGrabbed);
	GrabComponent->OnReleased.BindUObject(this, &ACastanet::OnReleased);

	

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(UGameplayStatics::GetPlayerController(this, 0)->InputComponent))
	{
		// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
		EnhancedInputComponent->BindAction(ClapLeftAction, ETriggerEvent::Started, this, &ACastanet::PlayCastanet);
		EnhancedInputComponent->BindAction(ClapRightAction, ETriggerEvent::Started, this, &ACastanet::PlayCastanet);
		UE_LOG(LogTemp, Warning, TEXT("Input setup"))
	}

//	AddMappingContext(IMCCastanetRight);
}


// Called every frame
void ACastanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACastanet::OnGrabbed()
{
	EControllerHand Controller = GrabComponent->GetHeldByHand();
	switch (Controller) 
	{
	case EControllerHand::Left:
		AddMappingContext(IMCCastanetLeft);
		break;
	case EControllerHand::Right:
		AddMappingContext(IMCCastanetRight);
		break;
	default:
		break;
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

void ACastanet::PlayCastanet()
{
	UE_LOG(LogTemp, Warning, TEXT("Castanet"))
	if (AudioComp != nullptr)
	{
		AudioComp->Play();
	}
	PlayHapticEffectOnController(OnClapHapticEffect);
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


