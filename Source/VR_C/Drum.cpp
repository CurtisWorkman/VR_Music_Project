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
		UHitterComponent* Hitter = Cast<UHitterComponent>(OtherComp);
		ADrumstick* Drumstick = Cast<ADrumstick>(OtherActor);
		if (Hitter != nullptr)
		{
				//play sound
				float Speed = -Drumstick->GetHitterSpeed();
				//		UE_LOG(LogTemp, Warning, TEXT("Velocity %f"), Speed);
				if (Speed > 500)
				{
					Speed = 500;
				}
				else if (Speed < 0)
				{
					return;
				}
				AudioHitComp->VolumeMultiplier = Speed * Speed / 25000 * 7;
				AudioHitComp->Play();

				//Take Velocity into account and change effect amplitude
				Drumstick->PlayHapticEffectOnController(OnHitDrumHapticEffect);
				OnHitDel.ExecuteIfBound();
				bIsHitAllowed = false;
		
		}


}

void ADrum::ResetHitAllowed()
{
	bIsHitAllowed = true;
}


