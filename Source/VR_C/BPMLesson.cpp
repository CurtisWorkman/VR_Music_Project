// Fill out your copyright notice in the Description page of Project Settings.


#include "BPMLesson.h"

ABPMLesson::ABPMLesson() : ALessonTemplate()
{
	LessonDetails.LessonName = TEXT("Tempo (BPM) & The Crotchet");
}

void ABPMLesson::BeginPlay()
{
	Super::BeginPlay();
	SetLessonName(FText::FromString(TEXT("Tempo")));
	AddToScore(0);
	OnIncrementState.BindUObject(this, &ABPMLesson::IncrementState);
	IncrementState();
}

void ABPMLesson::IncrementState()
{
	State++;

	switch (State)
	{
	case 0:
		StartLessonText(FText::FromString(TEXT("Have you ever noticed some songs are slow, whilst others are fast?")), Speech[State]);
		break;
	case 1:
		StartLessonText(FText::FromString(TEXT("The speed of a piece of music is determined by the musical 'Heartbeat', which we sometimes call Tempo or Beats Per Minute.")), Speech[State]);
		break;
	case 2:
		StartLessonText(FText::FromString(TEXT("A higher BPM will speed up the piece, generally making it feel more lively or energetic.")), Speech[State]);
		break;
	case 3:
		StartLessonText(FText::FromString(TEXT("Whilst a lower BPM can feel more calm or relaxed.")), Speech[State]);
		break;
	case 4:
		StartLessonText(FText::FromString(TEXT("Let's listen to what 50BPM sounds like.")), Speech[State]);
		break;
	case 5:
		StartRhythmLesson(FString(TEXT("")), 50);
		break;
	case 6:
		StartLessonText(FText::FromString(TEXT("Now let's listen to what 200BPM sounds like.")), Speech[State]);
		break;
	case 7:
		StartRhythmLesson(FString(TEXT("")), 200);
		break;
	case 8:
		StartLessonText(FText::FromString(TEXT("Big difference right?")), Speech[State]);
		break;
	case 9:
		StartLessonText(FText::FromString(TEXT("Now let's play along to each one.")), Speech[State]);
		break;
	case 10:
		StartLessonText(FText::FromString(TEXT("We can play along using crotchets, these tell us to play notes that last exactly one beat.")), Speech[State]);
		break;
	case 11:
		StartLessonText(FText::FromString(TEXT("So they will be played exactly to the tempo.")), Speech[State]);
		break;
	case 12:
		SpawnRhythmLessonBar();
		if (RhythmLessonBarRef != nullptr)
		{
			RhythmLessonBarRef->SpawnNoteMeshes(FString(TEXT("C")));
		}
		OnIncrementState.ExecuteIfBound();
		break;
	case 13:
		StartLessonText(FText::FromString(TEXT("Crotchets are usually represented with a solid circle and a vertical line... like this")), Speech[State]);
		break;

	case 14:
		StartLessonText(FText::FromString(TEXT("Let's start at 50BPM and work our way faster")), Speech[State]);
		break;
	case 15:
		RhythmLessonBarRef->Destroy();
		OnIncrementState.ExecuteIfBound();
		break;
	case 16:
		StartLessonText(FText::FromString(TEXT("Ready? Let's start!")), Speech[State]);
		break;
	case 17:
		StartRhythmLesson(FString(TEXT("CCCC")), 50);
		break;
	case 18:
		StartRhythmLesson(FString(TEXT("CCCC")), 80);
		break;
	case 19:
		StartRhythmLesson(FString(TEXT("CCCC")), 120);
		break;
	case 20:
		StartRhythmLesson(FString(TEXT("CCCC")), 160);
		break;
	case 21:
		StartRhythmLesson(FString(TEXT("CCCC")), 200);
		break;
	case 22:
		StartLessonText(FText::FromString(TEXT("Great job! You have now completed the lesson on Tempo and Crotchets.")), Speech[State]);
		break;
	case 23:
		EndLesson();
		break;
	default:
		break;
	}
}