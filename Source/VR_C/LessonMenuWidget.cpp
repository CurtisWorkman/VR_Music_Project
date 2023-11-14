// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonMenuWidget.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "LessonRow.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


#include "LessonTemplate.h"

ULessonMenuWidget::ULessonMenuWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> LessonRowBPClass(TEXT("/Game/UI/WBP_LessonRow"));
	if (!ensure(LessonRowBPClass.Class != nullptr)) return;

	LessonRowClass = LessonRowBPClass.Class;

}

bool ULessonMenuWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!ensure(StartButton != nullptr)) return false;
	StartButton->OnClicked.AddDynamic(this, &ULessonMenuWidget::StartLesson);

	return true;
}


void ULessonMenuWidget::AddLessonRow(uint32 Index, struct FLessonDetails* LessonDeets)
{
	FString LessonName = LessonDeets->LessonName;
	LessonList->ClearChildren();
	ULessonRow* Row = CreateWidget<ULessonRow>(this, LessonRowClass);	
	Row->LessonName->SetText(FText::FromString(LessonName));
	Row->Setup(this, Index);
	LessonList->AddChild(Row);

	ArrayOfLessons.Add(Row);
	
}

void ULessonMenuWidget::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UE_LOG(LogTemp, Warning, TEXT("click index %d"), SelectedIndex.GetValue());
}

void ULessonMenuWidget::SetMenuRef(ALessonMenu* InMenuRef)
{
	this->MenuRef = InMenuRef;
}

void ULessonMenuWidget::AddScoreToCurrentLesson(int InScore)
{
	ULessonRow* CurrentRowLesson = ArrayOfLessons[SelectedIndex.GetValue()];
	CurrentRowLesson->SetLessonScore(InScore);
}

void ULessonMenuWidget::StartLesson()
{
	
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("startclick index %d"), SelectedIndex.GetValue());
		if (MenuRef != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("not null"));
			MenuRef->StartLesson(SelectedIndex.GetValue());
		}
		
	}
}



