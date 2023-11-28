// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Drum.h"
#include "Components/WidgetInteractionComponent.h"
#include "MotionControllerComponent.h"
#include "ExpressionPad.generated.h"

/**
 * 
 */
UCLASS()
class VR_C_API AExpressionPad : public ADrum
{
	GENERATED_BODY()
	

public:
	AExpressionPad();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSound(uint32 Index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
private:
	UPROPERTY(EditAnywhere)
		class UWidgetComponent* ExpressionPadMenu;

	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* HandHoverArea;

	UPROPERTY(EditAnywhere)
		TArray<USoundBase*> Sounds;

	UFUNCTION()
	void OnHandAreaOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHandAreaOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetWidgetInteractionReferences();
	void SetMotionControllerReference();

	void OnStartInteractRight();
	void OnCompleteInteractRight();

//	void SelectAudio();

	void OpenMenu();

	void CloseMenu();

	UPROPERTY(EditAnywhere)
		class UNiagaraComponent* MenuLaser;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Cursor;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* MenuMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MenuInteractRight;

	bool bActiveMenuHandRight = false;
	bool bDoOnceEnableVis = true;
	bool bDoOnceDisableVis = true;

	UWidgetInteractionComponent* WidgetInteractionRefLeft;
	UWidgetInteractionComponent* WidgetInteractionRefRight;
	UMotionControllerComponent* MotionControllerAimRef;

	UWidgetInteractionComponent* GetWidgetInteractionComponent(bool bActiveMenuRight);
};
