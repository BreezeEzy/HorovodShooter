// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyAIController.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
