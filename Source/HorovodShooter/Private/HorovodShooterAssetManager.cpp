// Fill out your copyright notice in the Description page of Project Settings.


#include "HorovodShooterAssetManager.h"
#include "MyGameplayTags.h"


UHorovodShooterAssetManager& UHorovodShooterAssetManager::Get()
{
	check (GEngine);
	
	if (UHorovodShooterAssetManager* Singleton = Cast<UHorovodShooterAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	
	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini. It must be set"));
	return *NewObject<UHorovodShooterAssetManager>();
}

void UHorovodShooterAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FMyGameplayTags::InitializeNativeGameplayTags();
	
	UE_LOG(LogTemp, Warning, TEXT("NativeGameplayTags Initialized"));
}
