// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroLesson.h"

AIntroLesson::AIntroLesson() : ALessonTemplate()
{
	LessonDetails.LessonName = TEXT("Introduction");
}

void AIntroLesson::BeginPlay()
{
	Super::BeginPlay();
	SetLessonName(FText::FromString(TEXT("Introduction")));
	AddToScore(0);
	OnIncrementState.BindUObject(this, &AIntroLesson::IncrementState);
	IncrementState();
}

void AIntroLesson::IncrementState()
{
	State++;

	switch (State)
	{
	case 0:
		StartLessonText(FText::FromString(TEXT("Welcome to Virtual Reality rhythm training!")), Speech[State]);
		break;
	case 1:
		StartLessonText(FText::FromString(TEXT("In these lessons, we are going to teach you some fundamental musical ideas.")), Speech[State]);
		break;
	case 2:
		StartLessonText(FText::FromString(TEXT("In each lesson, you will have an opportunity to solidify your knowledge by scoring points!")), Speech[State]);
		break;
	case 3:
		StartLessonText(FText::FromString(TEXT("Grab those drumsticks in front of you and play along to the ticks by hitting the drum!")), Speech[State]);
		break;
	case 4:
		StartLessonText(FText::FromString(TEXT("We will count you in for four beats then you can start!")), Speech[State]);
		break;
	case 5:
		StartLessonText(FText::FromString(TEXT("Let's Start!")), Speech[State]);
		break;
	case 6:
		StartRhythmLesson(FString(TEXT("CCCC")), 60);
		break;
	case 7:
		StartLessonText(FText::FromString(TEXT("Congratulations! You have completed the introduction lesson.")), Speech[State]);
		break;
	case 8:
		StartLessonText(FText::FromString(TEXT("Good luck with the other lessons and have fun!")), Speech[State]);
		break;
	case 9:
		EndLesson();
		break;
	default:
		break;
	}
}