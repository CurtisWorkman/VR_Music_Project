// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "GrabComponent.h"
#include "Drumstick.generated.h"

UCLASS()
class VR_C_API ADrumstick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrumstick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* AudioHitComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetHitterSpeed();
	void PlayHapticEffectOnController(UHapticFeedbackEffect_Base* HitEffect);

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* HitterMesh;
	UPROPERTY(EditAnywhere)
		UGrabComponent* GrabComponent;

	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* OnHitStickHapticEffect;

	

	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool GetIsHeld();
};
