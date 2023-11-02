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
	StaticMesh->OnComponentHit.AddDynamic(this, &ADrumstick::OnHit);
	
}

// Called every frame
void ADrumstick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ADrumstick::GetHitterSpeed()
{
	return HitterMesh->GetComponentVelocity().Length();
}

void ADrumstick::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ADrumstick* OtherStick = Cast<ADrumstick>(OtherActor);
	if (OtherStick != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound"));
		AudioHitComp->Play();
		PlayHapticEffectOnController(OnHitStickHapticEffect);
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
		UE_LOG(LogTemp, Warning, TEXT("PlayHaptic"));
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController != nullptr && HitEffect != nullptr)
		{
			PlayerController->PlayHapticEffect(HitEffect, GrabComponent->GetHeldByHand());
		}
	}
}

