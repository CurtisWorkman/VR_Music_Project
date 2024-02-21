// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LessonTemplate.h"
#include "LessonSemibreve.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API ALessonSemibreve : public ALessonTemplate
{
	GENERATED_BODY()
	
public:
	ALessonSemibreve();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		TArray<USoundBase*> Speech;

	UFUNCTION()
		void IncrementState();

	int State = -1;
};
