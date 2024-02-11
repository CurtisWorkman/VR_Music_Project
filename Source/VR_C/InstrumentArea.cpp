// Fill out your copyright notice in the Description page of Project Settings.


#include "InstrumentArea.h"

#include "VRPawn.h"
#include "Components/TextRenderComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "VR_CGameModeBase.h"

#include "Earth.h"

#include "Kismet/GameplayStatics.h"

AInstrumentArea::AInstrumentArea() : AActor()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	RootComponent = RootComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root Component"));
	StaticMesh->SetupAttachment(RootComp);

	EarthSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Eath Spawn Point"));
	EarthSpawnPoint->SetupAttachment(StaticMesh);

	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	TriggerArea->SetupAttachment(StaticMesh);

	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TextComponent->SetupAttachment(StaticMesh);

	EarthComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Earth Component"));
	EarthComp->SetupAttachment(StaticMesh);

	TextSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Instrument Information"));
	TextSound->SetupAttachment(StaticMesh);

	TextSound->bAutoActivate = false;
}

void AInstrumentArea::BeginPlay()
{
	Super::BeginPlay();

	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AInstrumentArea::OnTriggerAreaOverlap);

	TextSound->OnAudioFinished.AddDynamic(this, &AInstrumentArea::IterateDialogue);

	
}

void AInstrumentArea::OnTriggerAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("overlap %s"), *OtherActor->GetName());
	AVRPawn* VRPawn = Cast<AVRPawn>(OtherActor);
	if (VRPawn != nullptr)
	{		
		StartLesson();
	}
}

void AInstrumentArea::StartLesson()
{
	AVR_CGameModeBase* GameMode = Cast<AVR_CGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode != nullptr)
	{
		AInstrumentArea* CurrentPlayingInstrument = GameMode->GetStoredPlayingInstrument();
		if (CurrentPlayingInstrument == this)
		{
			UE_LOG(LogTemp, Warning, TEXT("returned"))
			return;
		}
		else
		{
			if (CurrentPlayingInstrument != nullptr)
			{
				CurrentPlayingInstrument->EndLessonEarly();
			}
			
			GameMode->StorePlayingInstrument(this);

			DialogueIndex = -1;
			IterateDialogue();

			EarthComp->SetHiddenInGame(false);
			Earth = Cast<AEarth>(EarthComp->GetChildActor());
			if (Earth != nullptr)
			{
				Earth->EarthMove(FString(TEXT("Spain")));
			}
			
		}
	}

	
}

void AInstrumentArea::StartTextAudio()
{
	TextSound->Play();
}

void AInstrumentArea::IterateDialogue()
{
	DialogueIndex++;
	if (DialogueIndex < DialogueText.Num() && DialogueIndex < DialogueSound.Num())
	{
		TextComponent->SetText(FText::FromString(DialogueText[DialogueIndex]));

		TextSound->SetSound(DialogueSound[DialogueIndex]);
		StartTextAudio();
	}
	else
	{
		TextComponent->SetText(FText::FromString(""));
		AVR_CGameModeBase* GameMode = Cast<AVR_CGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode != nullptr)
		{
			GameMode->StorePlayingInstrument(nullptr);
		}
	}
	
}

void AInstrumentArea::EndLessonEarly()
{
	TextComponent->SetText(FText::FromString(""));
	TextSound->Stop();
	if (EarthComp != nullptr)
	{
//		EarthComp->Destroy();
	}	
}

