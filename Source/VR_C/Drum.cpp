// Fill out your copyright notice in the Description page of Project Settings.


#include "Drum.h"
#include "HitterComponent.h"
#include "Drumstick.h"

// Sets default values
ADrum::ADrum()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComp);

	HitArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hit Area"));
	HitArea->SetupAttachment(StaticMesh);

	AudioHitComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Hit Sound"));
	AudioHitComp->SetupAttachment(StaticMesh);
	
	AudioHitComp->bCanPlayMultipleInstances = true;
	AudioHitComp->bAutoActivate = false;

}

// Called when the game starts or when spawned
void ADrum::BeginPlay()
{
	Super::BeginPlay();
	HitArea->OnComponentBeginOverlap.AddDynamic(this, &ADrum::OverlapBegin);
	
}

int ADrum::RegisterHit(class ADrumstick* DrumstickRef)
{
	//play sound
	UE_LOG(LogTemp, Warning, TEXT("speed %s"), *DrumstickRef->GetHitterSpeed().ToString());
	float Speed = -FVector::DotProduct(DrumstickRef->GetHitterSpeed(), HitArea->GetUpVector());
	UE_LOG(LogTemp, Warning, TEXT("speed %f"), Speed);
	
	if (Speed > MaxSpeed)
	{
		Speed = MaxSpeed;
	}
	else if (Speed < 0)
	{
		return 0;
	}
	float VolumeMultiplier = (Speed * Speed * ForceMultiplier) / (MaxSpeed * MaxSpeed);
	AudioHitComp->VolumeMultiplier = VolumeMultiplier;
	AudioHitComp->Play();

	//Take Velocity into account and change effect amplitude
	DrumstickRef->PlayHapticEffectOnController(OnHitDrumHapticEffect);
	OnHitDel.ExecuteIfBound();
	bIsHitAllowed = false;
	return Speed;
}

// Called every frame
void ADrum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADrum::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		UHitterComponent* Hitter = Cast<UHitterComponent>(OtherComp);
		ADrumstick* Drumstick = Cast<ADrumstick>(OtherActor);
		if (Hitter != nullptr)
		{
			RegisterHit(Drumstick);						
		}
}

void ADrum::ResetHitAllowed()
{
	bIsHitAllowed = true;
}


