// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonSemibreve.h"

ALessonSemibreve::ALessonSemibreve() : ALessonTemplate()
{
	LessonDetails.LessonName = TEXT("The Semibreve");
}

void ALessonSemibreve::BeginPlay()
{
	Super::BeginPlay();
	SetLessonName(FText::FromString(TEXT("Semibreve")));
	AddToScore(0);
	OnIncrementState.BindUObject(this, &ALessonSemibreve::IncrementState);
	IncrementState();
}

void ALessonSemibreve::IncrementState()
{
	State++;

	switch (State)
	{
	case 0:
		StartLessonText(FText::FromString(TEXT("Semibreves, the giants of musical notes, signify the longest duration in standard notation, lasting exactly four beats.")), Speech[State]);
		break;
	case 1:
		StartLessonText(FText::FromString(TEXT("Unlike crotchets, which last one beat, semibreves require sustaining a note for the entirety of four beats.")), Speech[State]);
		break;
	case 2:
		SpawnRhythmLessonBar();
		if (RhythmLessonBarRef != nullptr)
		{
			RhythmLessonBarRef->SpawnNoteMeshes(FString(TEXT("S")));
		}
		OnIncrementState.ExecuteIfBound();
		break;
	case 3:
		StartLessonText(FText::FromString(TEXT("They are represented by a hollow circle without a stem.")), Speech[State]);
		break;
	case 4:
		StartLessonText(FText::FromString(TEXT("We will count in 4 beats then you can play the semibreve.")), Speech[State]);
		break;
	case 5:
		StartLessonText(FText::FromString(TEXT("Let's start!")), Speech[State]);
		break;
	case 6:
		RhythmLessonBarRef->Destroy();
		OnIncrementState.ExecuteIfBound();
		break;
	case 7:
		StartRhythmLesson(FString(TEXT("S")), 120);
		break;
	case 8:
		StartLessonText(FText::FromString(TEXT("Excellent! You've now grasped the essence of semibreves in musical rhythm.")), Speech[State]);
		break;
	case 9:
		EndLesson();
		break;
	default:
		break;
	}
}