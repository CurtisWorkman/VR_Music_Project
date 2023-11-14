// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonDetailsWidget.h"
#include "Components/TextBlock.h"

void ULessonDetailsWidget::SetLessonName(const FText& InLessonName)
{
	LessonName->SetText(InLessonName);
}

void ULessonDetailsWidget::SetLessonScore(const FText& InLessonScore)
{
	LessonScore->SetText(InLessonScore);
}

void ULessonDetailsWidget::SetLessonText(const FText& InLessonText)
{
	LessonText->SetText(InLessonText);
}
