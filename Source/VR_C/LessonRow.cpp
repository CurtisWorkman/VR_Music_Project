// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonRow.h"

#include "LessonMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h" 

void ULessonRow::Setup(ULessonMenuWidget* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &ULessonRow::OnClicked);
}

void ULessonRow::OnClicked()
{	
	if (Parent == nullptr) return;
	Parent->SelectIndex(Index);
	
}

void ULessonRow::SetLessonScore(int TotalScore)
{
	FString ScoreString = LessonScore->GetText().ToString();
	if (ScoreString.IsNumeric())
	{
		int MyInt = FCString::Atoi(*ScoreString);
		if (TotalScore > MyInt)
		{
			LessonScore->SetText(FText::AsNumber(TotalScore));
		}
	}
}
