// Fill out your copyright notice in the Description page of Project Settings.


#include "Maraca.h"
#include "Components/AudioComponent.h"
#include "GrabComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMaraca::AMaraca()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh comp"));
	BaseMesh->SetupAttachment(RootComp);

	SpeedMeasurePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Speed Measure"));
	SpeedMeasurePoint->SetupAttachment(BaseMesh);

	InstrumentSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound Comp"));
	InstrumentSound->SetupAttachment(BaseMesh);

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Comp"));
	GrabComponent->SetupAttachment(BaseMesh);

	InstrumentSound->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AMaraca::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMaraca::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentSpeed = BaseMesh->GetPhysicsLinearVelocityAtPoint(SpeedMeasurePoint->GetRelativeLocation());
//	UE_LOG(LogTemp, Warning, TEXT("current speed %f"), CurrentSpeed);
	FVector SpeedDiff = CurrentSpeed - OldSpeed;

	float DotProd = FVector::DotProduct(CurrentSpeed, OldSpeed);
//	UE_LOG(LogTemp, Warning, TEXT("Dot prod %f"), DotProd);

	if (DotProd < -30)
	{
		float Speed = FMath::Pow(-DotProd, 1.0f/3.0f);
		UE_LOG(LogTemp, Warning, TEXT("Speed %f"), Speed)
		if (Speed > MaxSpeed)
		{
			Speed = MaxSpeed;
		}
		float VolumeMultiplier = Speed / MaxSpeed;
		InstrumentSound->VolumeMultiplier = VolumeMultiplier;
		InstrumentSound->Play();
		PlayHapticEffectOnController(OnHitStickHapticEffect);
	}

	OldSpeed = CurrentSpeed;
}

bool AMaraca::GetIsHeld()
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

void AMaraca::PlayHapticEffectOnController(UHapticFeedbackEffect_Base* HitEffect)
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

