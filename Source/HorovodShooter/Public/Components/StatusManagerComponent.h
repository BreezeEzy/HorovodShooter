// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StatusManagerComponent.generated.h"

class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOROVODSHOOTER_API UStatusManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusManagerComponent();
	
	void ReceiveStatusEffect(const FGameplayTag& StatusTag, float Duration);
	void ClearAllStatuses();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	ACharacter* OwnerCharacter;
	
	TMap<FGameplayTag, FTimerHandle> ActiveStatuses;
	
	float DefaultMaxWalkSpeed;

	void ApplyStatus(const FGameplayTag& StatusTag);
	void RemoveStatus(FGameplayTag StatusTag);
};
