// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpressionPadSoundMenuRow.h"
#include "ExpressionPadSoundMenu.h"
#include "Components/Button.h"

void UExpressionPadSoundMenuRow::Setup(UExpressionPadSoundMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UExpressionPadSoundMenuRow::OnClicked);
}

void UExpressionPadSoundMenuRow::OnClicked()
{
	if (Parent == nullptr) return;
	Parent->SelectIndex(Index);
}
