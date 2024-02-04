// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LessonDetailsWidget.generated.h"
/**
 * 
 */
UCLASS()
class VR_C_API ULessonDetailsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetLessonName(const FText& LessonName);
	void SetLessonScore(const FText& LessonScore);
	void SetLessonText(const FText& LessonText);

	int GetLessonScore();

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LessonName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LessonScore;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* LessonText;

	FString RemoveNonNumeric(FString Input);
	
};
