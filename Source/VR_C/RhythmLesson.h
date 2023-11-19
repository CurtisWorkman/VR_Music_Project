// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RhythmLesson.generated.h"

UCLASS()
class VR_C_API ARhythmLesson : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARhythmLesson();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnNoteMeshes(FString Notes);
	void ChangeNoteColour(int NoteNum, int Score);
	int BPM;

private:

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
		UStaticMesh* CrotchetStaticMesh;

	UPROPERTY(EditAnywhere)
		UStaticMesh* SemibreveStaticMesh;

	UPROPERTY(EditAnywhere)
		UStaticMesh* QuaverStaticMesh;

	UPROPERTY(EditAnywhere)
		UStaticMesh* MinimStaticMesh;

	UPROPERTY(EditAnywhere)
		UStaticMesh* TripletStaticMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SheetMusicBarStaticMesh;

	UPROPERTY(EditAnywhere)
		UMaterial* NoteMaterial;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* TickComponent;

	void AddMusicNote(UStaticMesh* NoteToAdd, float SizeLeft, float MaxLength, int noteNum);

	std::vector<UStaticMeshComponent*> ArrayOfMeshes;

};
