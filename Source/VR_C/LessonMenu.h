// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LessonTemplate.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"

#include "Components/WidgetInteractionComponent.h"
#include "MotionControllerComponent.h"

#include "LessonMenu.generated.h"


UCLASS()
class VR_C_API ALessonMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALessonMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartLesson(uint32 Index);


private:

	void EndLesson(int NewScore);

	void CloseMenu();

	void SetWidgetInteractionReferences();
	void SetMotionControllerReference();

	void OnStartInteractRight();
	void OnCompleteInteractRight();

	UWidgetInteractionComponent* GetWidgetInteractionComponent(bool bActiveMenuRight);

	bool bActiveMenuHandRight = false;
	bool bDoOnceEnableVis = true;
	bool bDoOnceDisableVis = true;

	UWidgetInteractionComponent* WidgetInteractionRefLeft;
	UWidgetInteractionComponent* WidgetInteractionRefRight;
	UMotionControllerComponent* MotionControllerAimRef;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<ALessonTemplate>> Lessons;

	UPROPERTY(EditAnywhere)
		USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
		UWidgetComponent* LessonMenuWidgetComp;

	UPROPERTY(EditAnywhere)
		UNiagaraComponent* MenuLaser;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Cursor;

	TSubclassOf<class UUserWidget> LessonMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* MenuMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MenuInteractRight;

	ALessonTemplate* CurrentLesson;
};
