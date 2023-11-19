// Fill out your copyright notice in the Description page of Project Settings.


#include "LessonTemplate.h"
#include "TextToSpeechEngineSubsystem.h"
#include "Components/WidgetComponent.h"
#include "LessonDetailsWidget.h"

// Sets default values
ALessonTemplate::ALessonTemplate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	LessonWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Lesson Widget"));
	LessonWidget->SetupAttachment(RootComp);

	LessonMetronome = CreateDefaultSubobject<UAudioComponent>(TEXT("LessonMetronome"));
	LessonMetronome->SetupAttachment(RootComponent);

	LessonTextToSpeech = CreateDefaultSubobject<UAudioComponent>(TEXT("LessonTextToSpeech"));
	LessonTextToSpeech->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALessonTemplate::BeginPlay()
{
	Super::BeginPlay();

	SpawnDrum();
	
	DrumRef->OnHitDel.BindUObject(this, &ALessonTemplate::RegisterHit);

	LessonDetailsWidgetRef = Cast<ULessonDetailsWidget>(LessonWidget->GetWidget());

}

void ALessonTemplate::StartLessonText(const FText& Text, USoundBase* TextSound)
{
	LessonDetailsWidgetRef->SetLessonText(Text);
	SetTextToSpeechComponent(TextSound);
	LessonTextToSpeech->OnAudioFinished.Clear();
	LessonTextToSpeech->OnAudioFinished.AddDynamic(this, &ALessonTemplate::IncrementState);
	StartTextToSpeech();
}

void ALessonTemplate::AddToScore(int ScoreToAdd)
{
	int CurrentScore = LessonDetailsWidgetRef->GetLessonScore();
	CurrentScore = CurrentScore + ScoreToAdd;
	LessonDetailsWidgetRef->SetLessonScore(FText::AsNumber(CurrentScore));
}

void ALessonTemplate::SetLessonName(const FText& Text)
{
	LessonDetailsWidgetRef->SetLessonName(Text);
}

void ALessonTemplate::SetTextToSpeechComponent(USoundBase* TextSound)
{
	LessonTextToSpeech->SetSound(TextSound);
}

void ALessonTemplate::StartRhythmLesson(FString Notes, int bpm)
{
	LessonScore = 0;
	Ticks = 4;
	SpawnRhythmLessonBar();
	if (RhythmLessonBarRef != nullptr)
	{
		
		RhythmLessonBarRef->SpawnNoteMeshes(Notes);
		bpmSet = bpm;
		TimeBetweenTick = 1.0 / (bpmSet / 60);
		numberOfPreTicks = 4;
		SetNoteMarks(Notes);
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ALessonTemplate::PlayPreTick, TimeBetweenTick, false);
		
	}
	
}

void ALessonTemplate::EndLesson()
{
	int TotalScore = LessonDetailsWidgetRef->GetLessonScore();
	OnLessonEnd.ExecuteIfBound(TotalScore);
	RhythmLessonBarRef->Destroy();
	DrumRef->Destroy();
	Destroy();
}

// Called every frame
void ALessonTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALessonTemplate::StartTextToSpeech()
{
	if (LessonTextToSpeech != nullptr)
	{
		//Should be an audio component that has a bound function to check when audio is finished to increment state
		LessonTextToSpeech->Play();
	}
}

void ALessonTemplate::SpawnRhythmLessonBar()
{
	if (RhythmLessonBar == nullptr) return;
	FVector Location = this->GetActorLocation();
	FRotator Rotation = this->GetActorRotation();
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
	FRotator Rotation = this->GetActorRotation();
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
				if (NoteValuesArray[i] != 2)
				{
					NoteValuesArray[i] = 2;
					RhythmLessonBarRef->ChangeNoteColour(i, 0);
				}
				
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
//			AddToScore(IntScore);
			LessonScore = LessonScore + IntScore;
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
		LessonMetronome->Play();
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ALessonTemplate::PlayPreTick, TimeBetweenTick, false);
		numberOfPreTicks--;
		UE_LOG(LogTemp, Warning, TEXT("pretick"))
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
		LessonMetronome->Play();
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ALessonTemplate::PlayTick, TimeBetweenTick, false);
		Ticks--;
	}
}

void ALessonTemplate::LessonComplete()
{	
	RhythmLessonBarRef->Destroy();
	float LessonCompleteScore = MaxLessonScore * 0.75;
	if (bEndless)
	{

		if (LessonScore >= LessonCompleteScore)
		{
			AddToScore(LessonScore);
			//delegate execute if bound
			OnEndlessLessonEnd.ExecuteIfBound();
		}
		else
		{
			EndLesson();
		}
	}
	else
	{
		if (LessonScore >= LessonCompleteScore)
		{
			AddToScore(LessonScore);
			StartLessonText(FText::FromString(TEXT("Well Done")), WellDoneAudio);
		}
		else
		{
			LessonTextToSpeech->OnAudioFinished.Clear();
			LessonTextToSpeech->OnAudioFinished.AddDynamic(this, &ALessonTemplate::FinLessonTryAgainAudio);				//Seperate function so doesnt increment state. Can be done in same func with bool
			StartLessonTryAgainAudio();
		}
	}

}

void ALessonTemplate::IncrementState()
{
	OnIncrementState.ExecuteIfBound();
}

void ALessonTemplate::StartLessonTryAgainAudio()
{
	LessonDetailsWidgetRef->SetLessonText(FText::FromString(TEXT("Let's Try Again")));
	SetTextToSpeechComponent(TryAgainAudio);
	StartTextToSpeech();
}

void ALessonTemplate::FinLessonTryAgainAudio()
{	
	StartRhythmLesson(LessonNotes, bpmSet);
}

void ALessonTemplate::SetNoteMarks(FString Notes)
{
	LessonNotes = Notes;
	int arrayLength = Notes.Len();
	float SizeLeft = 1;
	NoteValuesArray.resize(arrayLength);

	MaxLessonScore = arrayLength * 100;

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


