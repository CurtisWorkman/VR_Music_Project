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

int ULessonDetailsWidget::GetLessonScore()
{
	FString ScoreString = LessonScore->GetText().ToString();
	FString ScoreRemovedComma = RemoveNonNumeric(ScoreString);
	if (ScoreRemovedComma.IsNumeric())
	{
		int MyInt = FCString::Atoi(*ScoreRemovedComma);
		return MyInt;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not num"));
		return 0;
	}
	
}

FString ULessonDetailsWidget::RemoveNonNumeric(FString Input)
{
	FString Result;
	for (char character : Input)
	{
		if (isdigit(character))
		{
			Result += character;
		}
	}
	return Result;
}