// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ExpressionPadSoundMenuRow.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API UExpressionPadSoundMenuRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoundName;

	void Setup(class UExpressionPadSoundMenu* Parent, uint32 Index);

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

	UPROPERTY()
		class UExpressionPadSoundMenu* Parent;

	uint32 Index;

	UFUNCTION()
	void OnClicked();
	
};
