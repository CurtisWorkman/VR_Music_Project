// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpressionPad.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

#include "Curves/CurveLinearColor.h"

#include "ExpressionPadSoundMenu.h"

AExpressionPad::AExpressionPad() : ADrum()
{
	ExpressionPadMenu = CreateDefaultSubobject<UWidgetComponent>(TEXT("Menu"));
	ExpressionPadMenu->SetupAttachment(RootComp);

	HandHoverArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HoverArea"));
	HandHoverArea->SetupAttachment(HitMesh);

	Cursor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CursorComp"));
	Cursor->SetupAttachment(ExpressionPadMenu);

	MenuLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Menu Laser"));
	MenuLaser->SetupAttachment(ExpressionPadMenu);

	PrimaryActorTick.bCanEverTick = true;
}

void AExpressionPad::BeginPlay()
{
	Super::BeginPlay();
	UExpressionPadSoundMenu* ExpressionPadSoundMenu = Cast<UExpressionPadSoundMenu>(ExpressionPadMenu->GetUserWidgetObject());
	if (ExpressionPadSoundMenu)
	{
		//	ExpressionPadSoundMenu->SetMenuRef(this);


		for (int i = 0; i < Sounds.Num(); i++)
		{
			FString SoundName = Sounds[i]->GetName();
//			UE_LOG(LogTemp, Warning, TEXT("SOunds %s"), *SoundName);
			ExpressionPadSoundMenu->AddSoundRow(i, SoundName);
		}
	}

	HandHoverArea->OnComponentBeginOverlap.AddDynamic(this, &AExpressionPad::OnHandAreaOverlapBegin);
	HandHoverArea->OnComponentEndOverlap.AddDynamic(this, &AExpressionPad::OnHandAreaOverlapEnd);

	ExpressionPadSoundMenu->SetOwnerRef(this);

	if (ColourCurveReal)
	{
		FOnTimelineLinearColorStatic TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineCallback.BindUFunction(this, FName("DoCurveTick"));
		TimelineFinishedCallback.BindUFunction(this, FName("DoCurveTick"));
		ColourTimeline.AddInterpLinearColor(ColourCurveReal, TimelineCallback);
		ColourTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		TimeLineLength = ColourTimeline.GetTimelineLength();
	}
	MatToChange = HitMesh->CreateDynamicMaterialInstance(0);
//	ColourTimeline.SetPlayRate(.0f);
	
}

int AExpressionPad::RegisterHit(class ADrumstick* DrumstickRef)
{
	int Speed = ADrum::RegisterHit(DrumstickRef);
	if (Speed > 0)
	{
	//	float PlaybackPos = (MaxSpeed - Speed) / MaxSpeed * TimeLineLength;
	//	UE_LOG(LogTemp, Warning, TEXT("playback pos %f"), PlaybackPos);
	//	ColourTimeline.SetPlaybackPosition(PlaybackPos, false);
		ColourTimeline.PlayFromStart();
	}

	return 0;
}

void AExpressionPad::DoCurveTick()
{
	TimelineValue = ColourTimeline.GetPlaybackPosition();
	FLinearColor Color = ColourCurveReal->GetLinearColorValue(TimelineValue);
	
	MatToChange->SetVectorParameterValue(TEXT("Colour"), Color);
//	UE_LOG(LogTemp, Warning, TEXT("Timeline Value: %f"), TimelineValue);
//	UE_LOG(LogTemp, Warning, TEXT("Timeline Updated - Color: %s"), *Color.ToString());
}

void AExpressionPad::SetSound(uint32 Index)
{
//	UE_LOG(LogTemp, Warning, TEXT("Set Sound"));
	AudioHitComp->SetSound(Sounds[Index]);
}

void AExpressionPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ColourTimeline.TickTimeline(DeltaTime);

	if (MotionControllerAimRef != nullptr)
	{
		if (GetWidgetInteractionComponent(!bActiveMenuHandRight)->IsOverHitTestVisibleWidget())
		{
			if (bDoOnceEnableVis)
			{
				FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
				MenuLaser->SetVisibility(true);
				Cursor->SetVisibility(true);
				GetWidgetInteractionComponent(bActiveMenuHandRight)->AttachToComponent(MotionControllerAimRef, AttachmentRules);
				bDoOnceEnableVis = false;
				bDoOnceDisableVis = true;
			}
			UWidgetInteractionComponent* WidgetInteractionComponent = GetWidgetInteractionComponent(!bActiveMenuHandRight);
			FVector CursorLocation = WidgetInteractionComponent->GetLastHitResult().ImpactPoint;
			Cursor->SetWorldLocation(CursorLocation);
			TArray<FVector> VectorArray = { WidgetInteractionComponent->GetComponentLocation(), CursorLocation };
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(MenuLaser, TEXT("User.PointArray"), VectorArray);

		}
		else
		{
			if (bDoOnceDisableVis)
			{
				MenuLaser->SetVisibility(false);
				Cursor->SetVisibility(false);
				bDoOnceEnableVis = true;
				bDoOnceDisableVis = false;
			}
		}
	}
}

void AExpressionPad::OnHandAreaOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName(TEXT("ExpressionPadInteractor"))))
	{
		OpenMenu();
	}
}

void AExpressionPad::OnHandAreaOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName(TEXT("ExpressionPadInteractorClose"))))
	{
		CloseMenu();
	}
}

void AExpressionPad::SetWidgetInteractionReferences()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TArray<UWidgetInteractionComponent*> WidgetInteractionComponents;
	PlayerPawn->GetComponents<UWidgetInteractionComponent>(WidgetInteractionComponents);
	for (UWidgetInteractionComponent* InteractionComponent : WidgetInteractionComponents)
	{
		if (InteractionComponent->PointerIndex == 0)
		{
			WidgetInteractionRefLeft = InteractionComponent;
		}
		else
		{
			WidgetInteractionRefRight = InteractionComponent;
		}
	}

}

void AExpressionPad::SetMotionControllerReference()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TArray<UMotionControllerComponent*> MotionControllerComponents;
	PlayerPawn->GetComponents<UMotionControllerComponent>(MotionControllerComponents);
	FName MotionSourceText;
	if (bActiveMenuHandRight == false)
	{
		MotionSourceText = TEXT("LeftAim");
	}
	else
	{
		MotionSourceText = TEXT("RightAim");
	}
	for (UMotionControllerComponent* MotionControllerComponent : MotionControllerComponents)
	{
		if (MotionControllerComponent->MotionSource == MotionSourceText)
		{
			MotionControllerAimRef = MotionControllerComponent;
		}

	}
}

void AExpressionPad::OnStartInteractRight()
{
	if (WidgetInteractionRefRight != nullptr)
	{
		WidgetInteractionRefRight->PressPointerKey(EKeys::LeftMouseButton);
	}
}

void AExpressionPad::OnCompleteInteractRight()
{
	if (WidgetInteractionRefRight != nullptr)
	{
		WidgetInteractionRefRight->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}

void AExpressionPad::OpenMenu()
{
	if (bMenuOpen == false)
	{
		ExpressionPadMenu->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
		ExpressionPadMenu->SetVisibility(true);
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(MenuMappingContext, 2);
			}
		}

		SetWidgetInteractionReferences();
		SetMotionControllerReference();


		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("Input Setup"));
			EnhancedInputComponent->BindAction(MenuInteractRight, ETriggerEvent::Started, this, &AExpressionPad::OnStartInteractRight);
			EnhancedInputComponent->BindAction(MenuInteractRight, ETriggerEvent::Completed, this, &AExpressionPad::OnCompleteInteractRight);
		}
		bMenuOpen = true;
	}
}

void AExpressionPad::CloseMenu()
{
	if (bMenuOpen == true)
	{
		ExpressionPadMenu->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
		ExpressionPadMenu->SetVisibility(false);

		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		GetWidgetInteractionComponent(bActiveMenuHandRight)->AttachToComponent(MotionControllerAimRef, AttachmentRules);

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(MenuMappingContext);			//Take this out or do checks when other menu is open
			}
		}
		bMenuOpen = false;
	}

}

UWidgetInteractionComponent* AExpressionPad::GetWidgetInteractionComponent(bool bActiveMenuRight)
{
	if (bActiveMenuRight == false)
	{
		return WidgetInteractionRefLeft;
	}
	else
	{
		return WidgetInteractionRefRight;
	}
}
