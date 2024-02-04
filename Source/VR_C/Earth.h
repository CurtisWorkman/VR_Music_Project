// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Earth.generated.h"

UCLASS()
class VR_C_API AEarth : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEarth();

	void EarthMove(FRotator CountryRotaion);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
		USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UMaterialInstanceDynamic* MatToChange;

	UFUNCTION()
	void DoCurveTick();

	

	UPROPERTY(EditAnywhere)
	class UCurveFloat* EarthCurve;

	float TimelineValue;
	float TimeLineLength;
	FTimeline CurveTimeline;

	FRotator RotationToCountry;
	FRotator StartingRotation;
};
