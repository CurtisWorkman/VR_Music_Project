// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonTemplate.h"

// Sets default values
ALessonTemplate::ALessonTemplate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	LessonName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LessonName"));
	LessonName->SetupAttachment(RootComponent);

	LessonScore = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LessonScore"));
	LessonScore->SetupAttachment(RootComponent);

	LessonText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LessonText"));
	LessonText->SetupAttachment(RootComponent);

	LessonMetronome = CreateDefaultSubobject<UAudioComponent>(TEXT("LessonMetronome"));
	LessonMetronome->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALessonTemplate::BeginPlay()
{
	Super::BeginPlay();

	SpawnRhythmLessonBar();
	SpawnDrum();
	
	DrumRef->OnHitDel.BindUObject(this, &ALessonTemplate::RegisterHit);


}

void ALessonTemplate::SetLessonText(const FText& Text)
{
	LessonText->SetText(Text);
}

void ALessonTemplate::AddToScore(int ScoreToAdd)
{
//	Score += ScoreToAdd;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), ScoreToAdd);
	TotalScore = TotalScore + ScoreToAdd;
	LessonScore->SetText(FText::AsNumber(TotalScore));
}

void ALessonTemplate::SetLessonName(const FText& Text)
{
	LessonName->SetText(Text);
}

void ALessonTemplate::StartRhythmLesson(char Notes[], int bpm)
{
	if (RhythmLessonBarRef != nullptr)
	{
		RhythmLessonBarRef->SpawnNoteMeshes(Notes);
		//Start Timing with notes
	//	lessonNotes = Notes;			//use ufunvtion with params instead of globes
		bpmSet = bpm;
		TimeBetweenTick = 1.0 / (bpmSet / 60);
		UE_LOG(LogTemp, Warning, TEXT("between tick %f"), TimeBetweenTick)
		numberOfPreTicks = 4;
		SetNoteMarks(Notes);
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ALessonTemplate::PlayPreTick, TimeBetweenTick, false);

	}
	
}

// Called every frame
void ALessonTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALessonTemplate::SpawnRhythmLessonBar()
{
	if (RhythmLessonBar == nullptr) return;
	FVector Location = this->GetActorLocation();
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FVector TransformScale(1, 1, 1);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	RhythmLessonBarRef = GetWorld()->SpawnActor<ARhythmLesson>(RhythmLessonBar, Location, Rotation, SpawnInfo);
}

void ALessonTemplate::SpawnDrum()
{
	if (Drum == nullptr) return;
	FVector Location = this->GetActorLocation() +FVector(0, 0, -100);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FVector TransformScale(1, 1, 1);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	DrumRef = GetWorld()->SpawnActor<ADrum>(Drum, Location, Rotation, SpawnInfo);
}

void ALessonTemplate::RegisterHit()
{
	//if lesson running
	if (GetWorld()->GetTimerManager().IsTimerActive(LessonTimerHandle))
	{
		//check if close to timing. add to score
		float TimeThrough = GetWorld()->GetTimerManager().GetTimerElapsed(LessonTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Time Through %f"), TimeThrough);
		//percent of the way through
		float PercentThrough = TimeThrough / (4.0 / (bpmSet / 60));
		UE_LOG(LogTemp, Warning, TEXT("Percentthrough %f"), PercentThrough);
		//find closest to percentthrough
		float closestNum = 1;
		int ClosestPosition = 0;

		for (int i = 0; i < NoteValuesArray.size(); i++)
		{
			float NotePosition = NoteValuesArray[i];
			float scoretoadd = (std::abs(PercentThrough - NotePosition));
			if (scoretoadd < closestNum)
			{
				closestNum = scoretoadd;
				ClosestPosition = i;
			}
		}

		if (NoteValuesArray[ClosestPosition] != 2)			//arbitrary number to show note has been used and shouldn't be scored again. should loop through so all prev cant be scored
		{
			for (int i = ClosestPosition; i >= 0; i--)
			{
				NoteValuesArray[i] = 2;
			}		
			float Score = 1 - closestNum;
			if (Score > 0.95)
			{
				Score = 100;
			}
			else if (Score > 0.90)
			{
				Score = 50;
			}
			else if (Score > 0.85)
			{
				Score = 25;
			}
			else
			{
				Score = 0;
			}
			int IntScore = static_cast<int>(Score);
			AddToScore(IntScore);
			//Remove note from array so cant be scored again
			//Change note colour depending on score
			RhythmLessonBarRef->ChangeNoteColour(ClosestPosition, IntScore);
		}	
	}
}

void ALessonTemplate::PlayPreTick()
{
	if (numberOfPreTicks > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("tick"));
		LessonMetronome->Play();
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ALessonTemplate::PlayPreTick, TimeBetweenTick, false);
		numberOfPreTicks--;
	}
	else
	{
		//Start LessonTimer		Should timer start tick before?
		LessonMetronome->Play();
		//do something to signify start
		GetWorld()->GetTimerManager().SetTimer(LessonTimerHandle, this, &ALessonTemplate::LessonComplete, 4 / (bpmSet / 60), false);
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ALessonTemplate::PlayTick, TimeBetweenTick, false);
	}
}

void ALessonTemplate::PlayTick()
{

	if (Ticks - 1 > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("tick"));
		LessonMetronome->Play();
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ALessonTemplate::PlayTick, TimeBetweenTick, false);
		Ticks--;
	}
}

void ALessonTemplate::LessonComplete()
{
	
}

void ALessonTemplate::SetNoteMarks(char Notes[])
{
	int arrayLength = strlen(Notes);
	float SizeLeft = 1;
	NoteValuesArray.resize(arrayLength);

	for (int i = 0; i < arrayLength; i++)
	{
		char Note = Notes[i];
		NoteValuesArray[i] = 1 - SizeLeft;
		switch (Note)
		{
		case 'S':	
			SizeLeft = SizeLeft - 1.0;
			break;
		case 'M':
			SizeLeft = SizeLeft - 1.0 / 2;
			break;
		case 'C':
			SizeLeft = SizeLeft -  1.0 / 4;
			break;
		case 'Q':
			SizeLeft = SizeLeft - 1.0 / 8;
			break;
		case 'T':
			SizeLeft = SizeLeft - 1.0 / 4 / 3;
			break;
		default:
			break;
		}
	}
}


