// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LessonMenu.h"

#include "LessonMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API ULessonMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULessonMenuWidget(const FObjectInitializer& ObjectInitializer);

	void AddLessonRow(uint32 Index, struct FLessonDetails* LessonDeets);

	void SelectIndex(uint32 Index);

	void SetMenuRef(ALessonMenu* MenuRef);

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* LessonList;

	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class ULessonRow> LessonRowClass;

	UFUNCTION()
	void StartLesson();

	ALessonMenu* MenuRef;

protected:
	virtual bool Initialize();


	TOptional<uint32> SelectedIndex;

};
