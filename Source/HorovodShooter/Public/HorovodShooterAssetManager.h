// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HorovodShooterAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class HOROVODSHOOTER_API UHorovodShooterAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UHorovodShooterAssetManager& Get();
	
	virtual void StartInitialLoading() override;
};
