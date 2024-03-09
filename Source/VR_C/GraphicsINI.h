// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GraphicsINI.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API UGraphicsINI : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "Ini Graphics")
	static void ReadIniFile(int& Shadows);
};
