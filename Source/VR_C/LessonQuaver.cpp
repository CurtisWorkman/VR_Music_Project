// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonQuaver.h"

ALessonQuaver::ALessonQuaver() : ALessonTemplate()
{
	LessonDetails.LessonName = TEXT("The Quaver");
}

void ALessonQuaver::BeginPlay()
{
	Super::BeginPlay();
	SetLessonName(FText::FromString(TEXT("Quaver")));
	AddToScore(0);
	OnIncrementState.BindUObject(this, &ALessonQuaver::IncrementState);
	IncrementState();
}

void ALessonQuaver::IncrementState()
{
	State++;

	switch (State)
	{
	case 0:
		StartLessonText(FText::FromString(TEXT("Descending further in note durations, quavers are twice as quick as crotchets, each lasting one-eighth of a beat in common time.")), Speech[State]);
		break;
	case 1:
		SpawnRhythmLessonBar();
		if (RhythmLessonBarRef != nullptr)
		{
			RhythmLessonBarRef->SpawnNoteMeshes(FString(TEXT("Q")));
		}
		OnIncrementState.ExecuteIfBound();
		break;
	case 2:
		StartLessonText(FText::FromString(TEXT("They are depicted by a solid circle with a vertical line, a tail, and a flag, like this.")), Speech[State]);
		break;
	case 3:
		StartLessonText(FText::FromString(TEXT("Let's integrate quavers into our rhythm.")), Speech[State]);
		break;
	case 4:
		StartLessonText(FText::FromString(TEXT("Let's start!")), Speech[State]);;
		break;
	case 5:
		RhythmLessonBarRef->Destroy();
		OnIncrementState.ExecuteIfBound();
		break;
	case 6:
		StartRhythmLesson(FString(TEXT("CQQCQQ")), 120);
		break;
	case 7:
		StartLessonText(FText::FromString(TEXT("Great job! You've completed the lesson on Quavers.")), Speech[State]);
		break;
	case 8:
		EndLesson();
		break;
	default:
		break;
	}
}

