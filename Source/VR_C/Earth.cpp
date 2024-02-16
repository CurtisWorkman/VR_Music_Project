// Fill out your copyright notice in the Description page of Project Settings.


#include "Earth.h"

// Sets default values
AEarth::AEarth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComp);

}

// Called when the game starts or when spawned
void AEarth::BeginPlay()
{
	Super::BeginPlay();

	if (EarthCurve != nullptr)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineCallback.BindUFunction(this, FName("DoCurveTick"));
		TimelineFinishedCallback.BindUFunction(this, FName("DoCurveTick"));
		CurveTimeline.AddInterpFloat(EarthCurve, TimelineCallback);
		CurveTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		TimeLineLength = CurveTimeline.GetTimelineLength();
		UE_LOG(LogTemp, Warning, TEXT("Length %f"), TimeLineLength);
	}
	
	MatToChange = StaticMeshComponent->CreateDynamicMaterialInstance(0);
}

// Called every frame
void AEarth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurveTimeline.TickTimeline(DeltaTime);

}

void AEarth::EarthMove(FString InSelectedCountryStr)
{
	StartingRotation = StaticMeshComponent->GetRelativeRotation();

	int SelectedCountryInt = 0;
	if (InSelectedCountryStr == TEXT("Spain"))
	{
		SelectedCountryInt = 0;
		RotationToCountry = FRotator(0, -90, 40);
	}
	else if (InSelectedCountryStr == TEXT("Africa"))
	{
		SelectedCountryInt = 1;
		RotationToCountry = FRotator(0, -90, 0);
	}
	else if (InSelectedCountryStr == TEXT("Mexico"))
	{
		SelectedCountryInt = 2;
		RotationToCountry = FRotator(0, 200, 0);
	}
	else if (InSelectedCountryStr == TEXT("China"))
	{
		SelectedCountryInt = 3;
		RotationToCountry = FRotator(-20, 10, 0);
	}

	MatToChange->SetScalarParameterValue(TEXT("SelectCountry"), SelectedCountryInt);

	CurveTimeline.PlayFromStart();	

}

void AEarth::DoCurveTick()
{
	float WayThrough = CurveTimeline.GetPlaybackPosition();
	float CurveValue = EarthCurve->GetFloatValue(WayThrough);
	MatToChange->SetScalarParameterValue(TEXT("LevelOfRed"), CurveValue);

	FRotator NewRotation = FMath::Lerp(StartingRotation, RotationToCountry, CurveValue);
	StaticMeshComponent->SetRelativeRotation(NewRotation);
}



