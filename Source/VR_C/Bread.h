// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabComponent.h"
#include "Bread.generated.h"

UCLASS()
class VR_C_API ABread : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABread();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void OnGrabbed();
	void OnReleased();

	UPROPERTY(EditAnywhere)
		UGrabComponent* GrabComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

};
