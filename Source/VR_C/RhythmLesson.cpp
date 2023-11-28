// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmLesson.h"
#include "Components/AudioComponent.h"
#include "Drum.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ARhythmLesson::ARhythmLesson()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	SheetMusicBarStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SheetBar"));
	SheetMusicBarStaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARhythmLesson::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ARhythmLesson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARhythmLesson::SpawnNoteMeshes(FString Notes)
{
	int arrayLength = Notes.Len();
	ArrayOfMeshes.resize(arrayLength);

	FVector Min;
	FVector Max;

	SheetMusicBarStaticMesh->GetLocalBounds(Min, Max);
//	UE_LOG(LogTemp, Warning, TEXT("Max %s"), *Max.ToString());

	float MaxLength = 2 * Max.Y;			//add margins
	float SizeLeft = MaxLength;

	
	for (int i = 0; i < arrayLength; i++)
	{	
		char Note = Notes[i];

//		UE_LOG(LogTemp, Warning, TEXT("NOte %c"), Note);

		switch (Note)
		{
		case 'S':
			AddMusicNote(SemibreveStaticMesh, SizeLeft, MaxLength, i);
			SizeLeft = SizeLeft - MaxLength;
			break;
		case 'M':
			AddMusicNote(MinimStaticMesh, SizeLeft, MaxLength, i);
			SizeLeft = SizeLeft - MaxLength /2;
			break;
		case 'C':
			AddMusicNote(CrotchetStaticMesh, SizeLeft, MaxLength, i);
			SizeLeft = SizeLeft - MaxLength / 4;
			break;
		case 'Q':
			AddMusicNote(QuaverStaticMesh, SizeLeft, MaxLength, i);
			SizeLeft = SizeLeft - MaxLength / 8;
			break;
		case 'T':
			AddMusicNote(TripletStaticMesh, SizeLeft, MaxLength, i);
			SizeLeft = SizeLeft - MaxLength / 4 / 3;
			break;
		default:
			break;
		}
//		UE_LOG(LogTemp, Warning, TEXT("SizeLeft %f"), SizeLeft);
		
	}
}

void ARhythmLesson::ChangeNoteColour(int NoteNum, int Score)
{
//	SheetMusicBarStaticMesh.Text
	UStaticMeshComponent* NoteToChange = ArrayOfMeshes[NoteNum];
	UMaterialInstanceDynamic* MatToChange =  NoteToChange->CreateDynamicMaterialInstance(0);
	FVector4 ColourToChangeTo;
	switch (Score)
	{
	case 100:
		ColourToChangeTo = FVector4(0, 1, 0, 1);
		break;
	case 50:
		ColourToChangeTo = FVector4(0.5, 1, 0, 1);
		break;
	case 25:
		ColourToChangeTo = FVector4(0.5, 0, 0, 1);
		break;
	case 0:
		ColourToChangeTo = FVector4(1, 0, 0, 1);
		break;
	default:
		ColourToChangeTo = FVector4(1, 1, 1, 1);
		break;
	}
	MatToChange->SetVectorParameterValue(TEXT("Base Colour"), ColourToChangeTo);
	SpawnNiagaraSystem(NoteToChange, ColourToChangeTo);

}

void ARhythmLesson::AddMusicNote(UStaticMesh* NoteToAdd, float SizeLeft, float MaxLength, int noteNum)
{
	if (NoteToAdd != nullptr)
	{
		float AddedOffset = MaxLength / 2;

		UStaticMeshComponent* NewNote = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(TEXT("Note %d"), noteNum));
		NewNote->RegisterComponent();

		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		NewNote->AttachToComponent(SheetMusicBarStaticMesh, FAttachmentTransformRules::KeepRelativeTransform);
		NewNote->CreationMethod = EComponentCreationMethod::Instance;
		NewNote->SetStaticMesh(NoteToAdd);
		NewNote->SetMaterial(0, NoteMaterial);
		
		//Move notes to their correct positions. Depending on the note - take up a percentage of the space of the bar. A variable for space taken up
		NewNote->SetRelativeLocation(FVector(0, -SizeLeft + AddedOffset, 3));
		NewNote->SetRelativeRotation(FRotator(90, 0, 180));

		//Make array of references to be accessed anc colour changed.
		ArrayOfMeshes[noteNum] = NewNote;
	}
}

void ARhythmLesson::SpawnNiagaraSystem(UStaticMeshComponent* Note, FVector4 Color)
{
	if (HitParticles) 
	{
		// This spawns the chosen effect on the owning WeaponMuzzle SceneComponent
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(HitParticles, Note, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		// Parameters can be set like this (see documentation for further info) - the names and type must match the user exposed parameter in the Niagara System
		NiagaraComp->SetNiagaraVariableLinearColor(FString("Colour"), FLinearColor(Color * 255));
	}
}


