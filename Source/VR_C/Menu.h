// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "MotionControllerComponent.h"
#include "NiagaraComponent.h"
#include "Components/TimelineComponent.h"
#include "Menu.generated.h"

UCLASS()
class VR_C_API AMenu : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* MenuMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MenuInteractRight;

public:	
	// Sets default values for this actor's properties
	AMenu();
	void CloseMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetWidgetInteractionReferences();
	void SetMotionControllerReference();

	UWidgetInteractionComponent* WidgetInteractionRefLeft;
	UWidgetInteractionComponent* WidgetInteractionRefRight;
	UMotionControllerComponent* MotionControllerAimRef;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* InteractiveMenu;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* MenuLaser;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Cursor;

	float WidgetRelativeScaleOriginal;

	UPROPERTY(EditAnywhere)
		UCurveFloat* MenuCurve;

	UFUNCTION()
		void DoTick();

	UFUNCTION()
		void FinCurve();

	float CurveFloatValue;
	float TimelineValue;
	FTimeline MyTimeline;

	float CursorLimitY;
	float CursorLimitX;

	bool bMenuOpening;

	bool bActiveMenuHandRight = false;

	UWidgetInteractionComponent* GetWidgetInteractionComponent(bool bActiveMenuRight);

	bool bDoOnceEnableVis = true;
	bool bDoOnceDisableVis = true;

	void OnStartInteractRight();
	void OnCompleteInteractRight();
};
