// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomSphere.generated.h"

UCLASS()
class VR_C_API ARoomSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeRoomTexture(FString CountryString);

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SphereMesh;

	UMaterialInstanceDynamic* MatToChange;

};
