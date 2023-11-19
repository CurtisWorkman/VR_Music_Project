// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LessonTemplate.h"
#include "EndlessLesson.generated.h"

/**
 * 
 */
struct GeneratedNotes {
	FString GeneratedNotesString;
	int Tempo;
};

UCLASS()
class VR_C_API AEndlessLesson : public ALessonTemplate
{
	GENERATED_BODY()
	
public:
	AEndlessLesson();

protected:
	virtual void BeginPlay() override;

private:
	GeneratedNotes GenerateNotes();

	void StartNewLesson();

};
