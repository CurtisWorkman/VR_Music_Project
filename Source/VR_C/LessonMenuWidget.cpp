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

		
	
}

void ULessonMenuWidget::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
}

void ULessonMenuWidget::SetMenuRef(ALessonMenu* InMenuRef)
{
	this->MenuRef = InMenuRef;
}

void ULessonMenuWidget::StartLesson()
{
	UE_LOG(LogTemp, Warning, TEXT("startclick"))
	if (SelectedIndex.IsSet())
	{
		MenuRef->StartLesson(SelectedIndex.GetValue());
	}
}



