// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpressionPad.h"
#include "ExpressionPadSoundMenu.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API UExpressionPadSoundMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UExpressionPadSoundMenu(const FObjectInitializer& ObjectInitializer);

	void AddSoundRow(uint32 Index, FString SoundName);

	void SelectIndex(uint32 Index);

	void SetOwnerRef(AExpressionPad* OwnerRef);

private:

	UPROPERTY(meta = (BindWidget))
		class UScrollBox* SoundList;

	UPROPERTY(meta = (BindWidget))
		class UButton* SelectButton;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UExpressionPadSoundMenuRow> SoundMenuRowClass;

	UFUNCTION()
	void SelectSound();

	AExpressionPad* OwnerRef;

protected:
	virtual bool Initialize();


	TOptional<uint32> SelectedIndex;
};
