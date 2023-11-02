// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Drum.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* HitArea;
//	UPROPERTY(EditAnywhere)
//		UBoxComponent* HitComp;
	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioHitComp;

	UFUNCTION()
		void HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* OnHitDrumHapticEffect;
};


