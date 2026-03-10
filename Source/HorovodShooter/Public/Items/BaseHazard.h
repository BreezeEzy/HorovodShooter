// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "BaseHazard.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class HOROVODSHOOTER_API ABaseHazard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseHazard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> TriggerArea;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> HazardMesh;
	//Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard Settings", meta = (ClampMin = 0.05f))
	float PulseInterval = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard Settings")
	FGameplayTagContainer HazardTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard Settings", meta = (ClampMin = 0.1f))
	float StatusDuration = 0.5f;
	
	//Logic
	virtual void ApplyStatusEffect(AActor *Victim);
	
	UFUNCTION()
	virtual void OnHazardBeginOverlap(
		UPrimitiveComponent* OverlappedComponents,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnHazardEndOverlap(
		UPrimitiveComponent* OverlappedComponents,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex);
	
	
private:
	UPROPERTY()
	TArray<AActor*> VictimsInside;
	FTimerHandle PulseTimerHandle;
	
	void HazardPulse();
};
