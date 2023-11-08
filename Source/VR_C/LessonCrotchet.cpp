// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonCrotchet.h"

void ALessonCrotchet::BeginPlay()
{
	Super::BeginPlay();
	SetLessonName(FText::FromString(TEXT("Lesson 1")));
	SetLessonText(FText::FromString(TEXT("Sample Lesson")));

	char Notes[] = "CCQQC";
	StartRhythmLesson(Notes, 70);
}