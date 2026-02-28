// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class HOROVODSHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected: 
	virtual void OnPossess(APawn* InPawn) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
