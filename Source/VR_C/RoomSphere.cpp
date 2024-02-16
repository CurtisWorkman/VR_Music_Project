// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSphere.h"

// Sets default values
ARoomSphere::ARoomSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Room Sphere"));
	RootComponent = SphereMesh;

}

// Called when the game starts or when spawned
void ARoomSphere::BeginPlay()
{
	Super::BeginPlay();
	
	MatToChange = SphereMesh->CreateDynamicMaterialInstance(0);
}

// Called every frame
void ARoomSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomSphere::ChangeRoomTexture(FString CountryString)
{
	int SelectedCountryInt = 0;
	if (CountryString == TEXT("Spain"))
	{
		SelectedCountryInt = 0;
	}
	else if (CountryString == TEXT("Africa"))
	{
		SelectedCountryInt = 1;
	}
	else if (CountryString == TEXT("Mexico"))
	{
		SelectedCountryInt = 2;
	}
	else if (CountryString == TEXT("China"))
	{
		SelectedCountryInt = 3;
	}
	MatToChange->SetScalarParameterValue(FName("CountrySwitch"), SelectedCountryInt);
}

