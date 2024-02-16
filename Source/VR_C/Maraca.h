// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Maraca.generated.h"

UCLASS()
class VR_C_API AMaraca : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaraca();

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
		USceneComponent* SpeedMeasurePoint;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
		UAudioComponent* InstrumentSound;

	UPROPERTY(EditAnywhere)
		class UGrabComponent* GrabComponent;

	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* OnHitStickHapticEffect;

	FVector CurrentSpeed = FVector(0, 0, 0);
	FVector OldSpeed = FVector(0, 0, 0);

	float MaxSpeed = 100;

	bool GetIsHeld();

	void PlayHapticEffectOnController(UHapticFeedbackEffect_Base* HitEffect);
};
