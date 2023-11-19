// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessLesson.h"



AEndlessLesson::AEndlessLesson()
{
	LessonDetails.LessonName = TEXT("Endless");
}

void AEndlessLesson::BeginPlay()
{
	Super::BeginPlay();
	SetLessonName(FText::FromString(TEXT("Endless")));
	AddToScore(0);
	bEndless = true;

    OnEndlessLessonEnd.BindUObject(this, &AEndlessLesson::StartNewLesson);
	StartNewLesson();
}

GeneratedNotes AEndlessLesson::GenerateNotes()
{
	GeneratedNotes GeneratedNotes;

    int Upper = 120;
    int Lower = 60;
    int RandTempo = rand() % (Upper - Lower + 1) + Lower;
	GeneratedNotes.Tempo = RandTempo;

    // Define note lengths and their corresponding durations in 4/4 time signature
    TArray<FString> noteLengths = { "C", "Q", "S", "M", "TTT" };
    std::vector<double> durations = { 1, 0.5, 4, 2, 1 };

    // Initialize the total duration of the bar
    double totalDuration = 4;

    // Vector to store the generated notes
    FString musicBar;

    while (totalDuration > 0) {
        // Randomly select a note length
        int randomIndex = rand() % noteLengths.Max();
        FString selectedNoteLength = noteLengths[randomIndex];

        // Convert the note length to its corresponding duration
        double duration = durations[randomIndex];

        // Check if the selected duration exceeds the remaining space in the bar
        if (duration <= totalDuration) {
            musicBar.Append(selectedNoteLength);
            totalDuration -= duration;
        }
    }

    GeneratedNotes.GeneratedNotesString = musicBar;
    UE_LOG(LogTemp, Warning, TEXT("Notes %s"), *GeneratedNotes.GeneratedNotesString);
	return GeneratedNotes;
}

void AEndlessLesson::StartNewLesson()
{
	GeneratedNotes GeneratedNotes = GenerateNotes();
	StartRhythmLesson(FString(TEXT("CCCC")), GeneratedNotes.Tempo);
}
