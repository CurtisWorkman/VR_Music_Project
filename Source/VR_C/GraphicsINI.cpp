// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphicsINI.h"

FString SavePath = FPaths::EngineConfigDir() + TEXT("NSSettings.ini");
//FString SavePath = TEXT("D:/Unreal Projects/Unreal Projects/VR/VR_Music/Config/NSSettings.ini");

void UGraphicsINI::ReadIniFile(int& Shadows)
{
	FString Enginestring = FPaths::ConvertRelativePathToFull(FPaths::EngineConfigDir());
	FString ProjectString = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir());
	UE_LOG(LogTemp, Warning, TEXT("Project Path %s"),*ProjectString);
	UE_LOG(LogTemp, Warning, TEXT("Save Path: %s"), *SavePath)
	UE_LOG(LogTemp, Warning, TEXT("Engine Path %s"), *Enginestring)
	GConfig->GetInt(TEXT("/Script/VR_C.GraphicsSettings"),
		TEXT("Shadows"),
		Shadows,
		SavePath);
}
