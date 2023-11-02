// Fill out your copyright notice in the Description page of Project Settings.


#include "Bread.h"



// Sets default values
ABread::ABread()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = StaticMeshComponent;

	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));
	GrabComponent->SetupAttachment(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ABread::BeginPlay()
{
	Super::BeginPlay();
//	UGrabComponent* GrabComponent = Cast<UGrabComponent>(GetDefaultSubobjectByName(TEXT("Grab")));
	GrabComponent->OnGrabbed.BindUObject(this, &ABread::OnGrabbed);
	GrabComponent->OnReleased.BindUObject(this, &ABread::OnReleased);
}

// Called every frame
void ABread::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABread::OnGrabbed()
{
	UE_LOG(LogTemp, Warning, TEXT("Delegate Grab done"));
}

void ABread::OnReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Delegate Release done"));
}

