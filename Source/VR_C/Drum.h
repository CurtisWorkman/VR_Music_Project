// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Drum.generated.h"


DECLARE_DELEGATE(FOnHitDelegate);

UCLASS()
class VR_C_API ADrum : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual int RegisterHit(class ADrumstick* DrumstickRef);

	UPROPERTY(EditAnywhere)
		USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* HitMesh;

	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioHitComp;
	
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 250;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnHitDelegate OnHitDel;

private:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* HitArea;
//	UPROPERTY(EditAnywhere)
//		UBoxComponent* HitComp;


	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* OnHitDrumHapticEffect;

	FTimerHandle HitTimerHandle;

	void ResetHitAllowed();

	bool bIsHitAllowed = true;

	UPROPERTY(EditAnywhere)
		float ForceMultiplier = 5;
};


