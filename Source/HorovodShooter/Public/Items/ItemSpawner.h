// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

class AActor;

UCLASS()
class HOROVODSHOOTER_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();
	
	UFUNCTION(BlueprintCallable, Category = "Spawner Events")
	void ExtractItem();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> SpawnerBaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	TSubclassOf<AActor> ItemClassToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	float RespawnTime = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	int32 MaxActiveItems = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	FVector SpawnOffset = FVector(0.0f, 0.0f, 60.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	bool bSpawnOnStart = true;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Settings")
	float HoverAmplitude;	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Settings")
	float RotateSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Settings")
	bool bHoverAndRotateItem = true;

	UFUNCTION(BlueprintImplementableEvent, Category = "Spawner Events")
	void OnItemSpawned();
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawner Events")
	void OnSpawnLimitReached();
	

private:
	FTimerHandle RespawnTimerHandle;
	
	UPROPERTY()
	AActor* CurrentWaitingItem;
	
	UPROPERTY()
	TArray<AActor*> TrackedItems;
	
	float HoverRunningTime = 0.0f;
	FVector BaseItemLocation;
	
	void TrySpawnItem();
	void CleanUpTrackedItems();
	
	UFUNCTION()
	void OnTrackedItemDestroyed(AActor* DestroyedActor);
};
