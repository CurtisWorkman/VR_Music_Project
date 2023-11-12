// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VRGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API UVRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UVRGameInstance();

private:
	TSubclassOf<class UUserWidget>MenuClass;
	TSubclassOf<class UUserWidget> LessonRowClass;

	class ULessonMenuWidget* Menu;
	class ULessonRow* Row;

public:
	UFUNCTION(BlueprintCallable)
		void LoadMenu();
};
