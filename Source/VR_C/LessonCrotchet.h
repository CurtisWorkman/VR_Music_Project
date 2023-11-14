// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LessonTemplate.h"
#include "LessonCrotchet.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API ALessonCrotchet : public ALessonTemplate
{
	GENERATED_BODY()
	

public:
	ALessonCrotchet();

protected:
	virtual void BeginPlay() override;

private:
	int CurrentState = -1;

	UPROPERTY(EditAnywhere)
		USoundBase* TextSound;

	UFUNCTION()
	void IncrementState();

	int State = 0;

};
