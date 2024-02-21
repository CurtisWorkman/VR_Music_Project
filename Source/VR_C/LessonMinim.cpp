// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonMinim.h"

ALessonMinim::ALessonMinim() : ALessonTemplate()
{
	LessonDetails.LessonName = TEXT("The Minim");
}

void ALessonMinim::BeginPlay()
{
	Super::BeginPlay();
	SetLessonName(FText::FromString(TEXT("Minim")));
	AddToScore(0);
	OnIncrementState.BindUObject(this, &ALessonMinim::IncrementState);
	IncrementState();
}

void ALessonMinim::IncrementState()
{
	State++;

	switch (State)
	{
	case 0:
		StartLessonText(FText::FromString(TEXT("Minims, the next tier down from semibreves, represent half the duration of semibreves, lasting for two beats in common time.")), Speech[State]);
		break;
	case 1:
		SpawnRhythmLessonBar();
		if (RhythmLessonBarRef != nullptr)
		{
			RhythmLessonBarRef->SpawnNoteMeshes(FString(TEXT("M")));
		}
		OnIncrementState.ExecuteIfBound();
		break;
	case 2:
		StartLessonText(FText::FromString(TEXT("Much like crotchets, they are depicted with a vertical line, however, minims are hollow.")), Speech[State]);
		break;
	case 3:
		StartLessonText(FText::FromString(TEXT("We will count in 4 beats then you can play two minims.")), Speech[State]);
		break;
	case 4:
		StartLessonText(FText::FromString(TEXT("Let's start!")), Speech[State]);;
		break;
	case 5:
		RhythmLessonBarRef->Destroy();
		OnIncrementState.ExecuteIfBound();
		break;
	case 6:
		StartRhythmLesson(FString(TEXT("MM")), 120);
		break;
	case 7:
		StartLessonText(FText::FromString(TEXT("Great job! You've completed the lesson on Minims.")), Speech[State]);	
		break;
	case 8:
		EndLesson();
		break;
	default:
		break;
	}
}

