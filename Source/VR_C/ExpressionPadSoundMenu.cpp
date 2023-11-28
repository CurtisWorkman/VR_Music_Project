// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpressionPadSoundMenu.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"

#include "ExpressionPadSoundMenuRow.h"

UExpressionPadSoundMenu::UExpressionPadSoundMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> SoundMenuRowBPClass(TEXT("/Game/ExpressiveSystem/UI/WBP_ExpressionPadSoundMenuRow"));

	if (!ensure(SoundMenuRowBPClass.Class != nullptr)) return;
	SoundMenuRowClass = SoundMenuRowBPClass.Class;
}

bool UExpressionPadSoundMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!ensure(SelectButton != nullptr)) return false;
	SelectButton->OnClicked.AddDynamic(this, &UExpressionPadSoundMenu::SelectSound);
	SoundList->ClearChildren();

	return true;
}

void UExpressionPadSoundMenu::SetOwnerRef(AExpressionPad* InOwnerRef)
{
	this->OwnerRef = InOwnerRef;
}

void UExpressionPadSoundMenu::SelectSound()
{
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("startclick index %d"), SelectedIndex.GetValue());
		if (OwnerRef != nullptr)
		{
			OwnerRef->SetSound(SelectedIndex.GetValue());
		}

	}
}

void UExpressionPadSoundMenu::AddSoundRow(uint32 Index, FString SoundName)
{
	UE_LOG(LogTemp, Warning, TEXT("Add"));
	if (SoundMenuRowClass != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("not null"))
		UExpressionPadSoundMenuRow* Row = CreateWidget<UExpressionPadSoundMenuRow>(this, SoundMenuRowClass);
		Row->SoundName->SetText(FText::FromString(SoundName));
		Row->Setup(this, Index);
		SoundList->AddChild(Row);
	}
}

void UExpressionPadSoundMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UE_LOG(LogTemp, Warning, TEXT("click index %d"), SelectedIndex.GetValue());

}
