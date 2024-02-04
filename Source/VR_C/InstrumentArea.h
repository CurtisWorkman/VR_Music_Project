// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstrumentArea.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API AInstrumentArea : public AActor
{
	GENERATED_BODY()
	
public:
	AInstrumentArea();

protected:
	UPROPERTY(EditAnywhere)
		class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
		class USceneComponent* EarthSpawnPoint;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* TriggerArea;

	UPROPERTY(EditAnywhere)
		class UTextRenderComponent* TextComponent;
	
	UPROPERTY(EditAnywhere)
		class UAudioComponent* TextSound;
	
	UPROPERTY(EditAnywhere)
		TArray<FString> DialogueText;

	UPROPERTY(EditAnywhere)
		TArray<USoundBase*> DialogueSound;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnTriggerAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void StartLesson();

	void StartTextAudio();

	UFUNCTION()
	void IterateDialogue();

	int DialogueIndex = 0;

	void EndLessonEarly();

	class AEarth* Earth;

	UPROPERTY(EditAnywhere)
		FRotator CountryRotation;
};
