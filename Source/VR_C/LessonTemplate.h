// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "RhythmLesson.h"
#include "Drum.h"
#include "Components/AudioComponent.h"
#include "LessonTemplate.generated.h"

DECLARE_DELEGATE_OneParam(FOnLessonEndDelegate, int);
DECLARE_DELEGATE(FOnIncrementState);
DECLARE_DELEGATE(FOnEndlessonEndDelegate);

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


	FOnLessonEndDelegate OnLessonEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartLessonText(const FText& Text, USoundBase* TextSound);
	void AddToScore(int ScoreToAdd);
	void SetLessonName(const FText& Text);

	

	void StartRhythmLesson(FString Notes, int bpm);

	void EndLesson();

	ARhythmLesson* RhythmLessonBarRef;
	ADrum* DrumRef;

	FOnIncrementState OnIncrementState;
	FOnEndlessonEndDelegate OnEndlessLessonEnd;

	bool bEndless = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
		USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* LessonWidget;

	UPROPERTY(EditAnywhere)
		UAudioComponent* LessonMetronome;

	UPROPERTY(EditAnywhere)
		UAudioComponent* LessonTextToSpeech;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ARhythmLesson> RhythmLessonBar;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADrum> Drum;

	UPROPERTY(EditAnywhere)
		class USoundBase* TryAgainAudio;

	UPROPERTY(EditAnywhere)
		class USoundBase* WellDoneAudio;

	class ULessonDetailsWidget* LessonDetailsWidgetRef;

	void StartTextToSpeech();
	void SetTextToSpeechComponent(USoundBase* TextSound);
	void SpawnRhythmLessonBar();
	void SpawnDrum();

	void RegisterHit();

	void PlayPreTick();
	void PlayTick();
	void LessonComplete();

	void IncrementState();

	void StartLessonTryAgainAudio();

	UFUNCTION()
	void FinLessonTryAgainAudio();


//	void SetNoteValuesArray(std::vector<float> NoteValuesArray);
//	void GetNoteValuesArray();

	std::vector<float> NoteValuesArray;

	void SetNoteMarks(FString Notes);

	FTimerHandle LessonTimerHandle;
	FTimerHandle TickTimerHandle;

	int numberOfPreTicks = 4;
	int Ticks = 4;
	float TimeBetweenTick;
	float bpmSet;

	FString LessonNotes;
//	char lessonNotes [];

//	float Score;

	int MaxLessonScore;
	int LessonScore;
};
