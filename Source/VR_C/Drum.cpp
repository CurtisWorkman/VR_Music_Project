// Fill out your copyright notice in the Description page of Project Settings.


#include "Drum.h"
#include "HitterComponent.h"
#include "Drumstick.h"

// Sets default values
ADrum::ADrum()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;

	HitArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hit Area"));
	HitArea->SetupAttachment(StaticMesh);

	AudioHitComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Hit Sound"));
	AudioHitComp->SetupAttachment(StaticMesh);
	

}

// Called when the game starts or when spawned
void ADrum::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->OnComponentHit.AddDynamic(this, &ADrum::HitMesh);
	HitArea->OnComponentBeginOverlap.AddDynamic(this, &ADrum::OverlapBegin);
	
}

// Called every frame
void ADrum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrum::HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
//	UE_LOG(LogTemp, Warning, TEXT("Hit"));
}

void ADrum::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap %s"), *OverlappedComponent->GetName());
//	UE_LOG(LogTemp, Warning, TEXT("Overlap %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Overlap %s"), *OtherComp->GetName());
	//Cast to drumstick
	UHitterComponent* Hitter = Cast<UHitterComponent>(OtherComp);
	ADrumstick* Drumstick = Cast<ADrumstick>(OtherActor);
	if (Hitter != nullptr)
	{
		//play sound
		float Speed = Drumstick->GetHitterSpeed();
		UE_LOG(LogTemp, Warning, TEXT("Velocity %f"), Speed);
		if (Speed > 200)
		{
			Speed = 200;
		}
		AudioHitComp->VolumeMultiplier = Speed/200;
		AudioHitComp->Play();

		//Take Velocity into account and change effect amplitude
		Drumstick->PlayHapticEffectOnController(OnHitDrumHapticEffect);
	}
}


