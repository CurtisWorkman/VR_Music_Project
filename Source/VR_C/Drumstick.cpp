// Fill out your copyright notice in the Description page of Project Settings.


#include "Drumstick.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrumstick::ADrumstick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;

	DrumstickHitArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hit Area"));
	DrumstickHitArea->SetupAttachment(StaticMesh);

	HitterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hit Mesh"));
	HitterMesh->SetupAttachment(StaticMesh);

	AudioHitComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Hit Sound"));
	AudioHitComp->SetupAttachment(StaticMesh);

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab"));
	GrabComponent->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ADrumstick::BeginPlay()
{
	Super::BeginPlay();
//	StaticMesh->OnComponentHit.AddDynamic(this, &ADrumstick::OnHit);
	
	DrumstickHitArea->OnComponentBeginOverlap.AddDynamic(this, &ADrumstick::OnHit);
	DrumstickHitArea->OnComponentEndOverlap.AddDynamic(this, &ADrumstick::EndHit);

}

// Called every frame
void ADrumstick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ADrumstick::GetHitterSpeed()
{
	return HitterMesh->GetComponentVelocity();
}

void ADrumstick::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADrumstick* OtherStick = Cast<ADrumstick>(OtherActor);
	if (OtherStick != nullptr)
	{
		AudioHitComp->Play();
		PlayHapticEffectOnController(OnHitStickHapticEffect);
	}
}

void ADrumstick::EndHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADrumstick* OtherStick = Cast<ADrumstick>(OtherActor);
	if (OtherStick != nullptr)
	{
		StopHapticEffectOnController();
	}
}

void ADrumstick::StopHapticEffectOnController()
{
	if (GetIsHeld())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayHaptic"));
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController != nullptr)
		{
			PlayerController->StopHapticEffect(GrabComponent->GetHeldByHand());
		}
	}
}

bool ADrumstick::GetIsHeld()
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

void ADrumstick::PlayHapticEffectOnController(UHapticFeedbackEffect_Base* HitEffect)
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

