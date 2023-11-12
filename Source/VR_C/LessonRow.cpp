// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonRow.h"

#include "LessonMenuWidget.h"
#include "Components/Button.h"

void ULessonRow::Setup(ULessonMenuWidget* InParent, int InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &ULessonRow::OnClicked);
}

void ULessonRow::OnClicked()
{
	if (Parent != nullptr) return;
	Parent->SelectIndex(Index);
	UE_LOG(LogTemp, Warning, TEXT("RowCLick"))
}
