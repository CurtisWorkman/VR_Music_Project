// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LessonRow.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API ULessonRow : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LessonName;


	void Setup(class ULessonMenuWidget* Parent, uint32 Index);

	void SetLessonScore(int TotalScore);

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

	UPROPERTY()
		class ULessonMenuWidget* Parent;

	uint32 Index;

	UFUNCTION()
		void OnClicked();

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LessonScore;

};
