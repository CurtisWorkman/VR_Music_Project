// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonMenu.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "LessonMenuWidget.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

// Sets default values
ALessonMenu::ALessonMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	LessonMenuWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Lesson Menu Widget"));
	LessonMenuWidgetComp->SetupAttachment(RootComp);

	MenuLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Menu Laser"));
	MenuLaser->SetupAttachment(RootComp);

	Cursor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cursor"));
	Cursor->SetupAttachment(LessonMenuWidgetComp);

	ConstructorHelpers::FClassFinder<UUserWidget> LessonMenuWidgetClassBP(TEXT("/Game/UI/WBP_LessonMenuWidget"));
	if (!ensure(LessonMenuWidgetClassBP.Class != nullptr)) return;

	LessonMenuWidgetClass = LessonMenuWidgetClassBP.Class;

}

// Called when the game starts or when spawned
void ALessonMenu::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MenuMappingContext, 3);
		}
	}

	SetWidgetInteractionReferences();
	SetMotionControllerReference();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Setup"));
		EnhancedInputComponent->BindAction(MenuInteractRight, ETriggerEvent::Started, this, &ALessonMenu::OnStartInteractRight);
		EnhancedInputComponent->BindAction(MenuInteractRight, ETriggerEvent::Completed, this, &ALessonMenu::OnCompleteInteractRight);
	}

	ULessonMenuWidget* LessonMenuWidget = Cast<ULessonMenuWidget>(LessonMenuWidgetComp->GetUserWidgetObject());

	for (int i = 0; i < Lessons.Num(); i++)
	{
		TSubclassOf<ALessonTemplate> Lesson = Lessons[i];
		if (Lesson)
		{
			ALessonTemplate* LessonInstance = Lesson.GetDefaultObject();

			FString LessonName = LessonInstance->LessonDetails.LessonName;
			UE_LOG(LogTemp, Warning, TEXT("Lesson Name %s"), *LessonName);
	
			LessonMenuWidget->AddLessonRow(i, &LessonInstance->LessonDetails);
		}		
	}
	
}

// Called every frame
void ALessonMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void ALessonMenu::StartLesson(int Index)
{
	FVector Location = this->GetActorLocation();
	FRotator Rotation = this->GetActorRotation();
	FVector TransformScale(1, 1, 1);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentLesson = GetWorld()->SpawnActor<ALessonTemplate>(Lessons[Index], Location, Rotation, SpawnInfo);
}

void ALessonMenu::SetWidgetInteractionReferences()
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

void ALessonMenu::SetMotionControllerReference()
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

void ALessonMenu::OnStartInteractRight()
{
	if (WidgetInteractionRefRight != nullptr)
	{
		WidgetInteractionRefRight->PressPointerKey(EKeys::LeftMouseButton);
	}
	
}

void ALessonMenu::OnCompleteInteractRight()
{	
	if (WidgetInteractionRefRight != nullptr)
	{
		WidgetInteractionRefRight->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}

UWidgetInteractionComponent* ALessonMenu::GetWidgetInteractionComponent(bool bActiveMenuRight)
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

