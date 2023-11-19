// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonCrotchet.h"

ALessonCrotchet::ALessonCrotchet() : ALessonTemplate()
{
	LessonDetails.LessonName = TEXT("Lesson Crotchet");
}

void ALessonCrotchet::BeginPlay()
{
	Super::BeginPlay();
	SetLessonName(FText::FromString(TEXT("Lesson Crotchet")));
	AddToScore(0);
	OnIncrementState.BindUObject(this, &ALessonCrotchet::IncrementState);
	IncrementState();
}

void ALessonCrotchet::IncrementState()
{
	State++;

	char Notes[] = "CCCC";
	switch (State)
	{
	case 0:
		StartLessonText(FText::FromString(TEXT("A crotchet is a musical note with the time value of one beat - or a quarter of a semibreve. It is played for one quarter of the length of a semibreve.")), TextSound);
		break;
	case 1:
		StartRhythmLesson(Notes, 70);
		break;
	case 2:
		EndLesson();
		break;
	default:
		break;
	}
}

