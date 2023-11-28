// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LessonTemplate.h"
#include "BPMLesson.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API ABPMLesson : public ALessonTemplate
{
	GENERATED_BODY()
	
public:
	ABPMLesson();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		TArray<USoundBase*> Speech;

	UFUNCTION()
		void IncrementState();

	int State = -1;
};
