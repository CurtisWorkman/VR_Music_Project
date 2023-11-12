// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "RhythmLesson.h"
#include "Drum.h"
#include "Components/AudioComponent.h"
#include "LessonTemplate.generated.h"

USTRUCT()
struct FLessonDetails				//edited in lesson template sub classes
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString LessonName = TEXT("Sample");
};

UCLASS()
class VR_C_API ALessonTemplate : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	ALessonTemplate();

	FLessonDetails LessonDetails;

	FString Test = TEXT("Test");

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetLessonText(const FText& Text);
	void AddToScore(int ScoreToAdd);
	void SetLessonName(const FText& Text);

	void StartRhythmLesson(char Notes[], int bpm);

	ARhythmLesson* RhythmLessonBarRef;
	ADrum* DrumRef;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
		USceneComponent* RootComp;


	UPROPERTY(EditAnywhere)
		UTextRenderComponent* LessonName;

	UPROPERTY(EditAnywhere)
		UTextRenderComponent* LessonScore;

	UPROPERTY(EditAnywhere)
		UTextRenderComponent* LessonText;

	UPROPERTY(EditAnywhere)
		UAudioComponent* LessonMetronome;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ARhythmLesson> RhythmLessonBar;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADrum> Drum;

	void SpawnRhythmLessonBar();
	void SpawnDrum();

	void RegisterHit();

	void PlayPreTick();
	void PlayTick();
	void LessonComplete();

//	void SetNoteValuesArray(std::vector<float> NoteValuesArray);
//	void GetNoteValuesArray();

	std::vector<float> NoteValuesArray;

	void SetNoteMarks(char Notes[]);

	FTimerHandle LessonTimerHandle;
	FTimerHandle TickTimerHandle;

	int numberOfPreTicks = 4;
	int Ticks = 4;
	int TotalScore = 0;
	float TimeBetweenTick;
	float bpmSet;
//	char lessonNotes [];

//	float Score;
};
