// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabComponent.h"
#include "Components/AudioComponent.h"
#include "Castanet.generated.h"

UCLASS()
class VR_C_API ACastanet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* IMCCastanetRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* IMCCastanetLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ClapLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ClapRightAction;

	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* OnClapHapticEffect;
	
public:	
	// Sets default values for this actor's properties
	ACastanet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAudioComponent* AudioComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UGrabComponent* GrabComponent;



	void OnGrabbed();
	void OnReleased();

	void AddMappingContext(class UInputMappingContext* IMC_Castanet);
	void RemoveMappingContext(class UInputMappingContext* IMC_Castanet);

	void PlayCastanet();

	bool GetIsHeld();

	void PlayHapticEffectOnController(UHapticFeedbackEffect_Base* HitEffect);
};
