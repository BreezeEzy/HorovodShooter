// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemSpawner.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

#include <ntstatus.h>

// Sets default values
AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnerBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));
	RootComponent = SpawnerBaseMesh;
}



// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bSpawnOnStart)
	{
		TrySpawnItem();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AItemSpawner::TrySpawnItem, RespawnTime, false);
	}
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bHoverAndRotateItem && IsValid(CurrentWaitingItem))
	{
		HoverRunningTime += DeltaTime;
		
		float ZOffset = FMath::Sin(HoverRunningTime * 2.0f) * HoverAmplitude;
		FVector NewLocation = BaseItemLocation + FVector(0.0f, 0.0f, ZOffset);
	
		FRotator NewRotation = CurrentWaitingItem->GetActorRotation();
		NewRotation.Yaw += RotateSpeed * DeltaTime;
	
		CurrentWaitingItem->SetActorLocationAndRotation(NewLocation, NewRotation,true, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AItemSpawner::TrySpawnItem()
{
	if (!ItemClassToSpawn) {return;}
	CleanUpTrackedItems();
	if (TrackedItems.Num() >= MaxActiveItems)
	{
		OnSpawnLimitReached();
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AItemSpawner::TrySpawnItem, RespawnTime, false);
		return;
	}
	
	FVector SpawnLocation = GetActorLocation() + SpawnOffset;
	FRotator SpawnRotation = GetActorRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	
	if (SpawnedActor)
	{
		if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(SpawnedActor->GetRootComponent()))
		{
			RootComp->SetSimulatePhysics(false);
		}
		
		TrackedItems.Add(SpawnedActor);
		CurrentWaitingItem = SpawnedActor;
		BaseItemLocation = SpawnedActor->GetActorLocation();
		HoverRunningTime = 0.0f;
		
		SpawnedActor->OnDestroyed.AddDynamic(this, &AItemSpawner::OnTrackedItemDestroyed);
		OnItemSpawned();
	}
}

void AItemSpawner::CleanUpTrackedItems()
{
	TrackedItems.RemoveAll([](AActor* Item) {return !IsValid(Item);});
}

void AItemSpawner::ExtractItem()
{
	if (CurrentWaitingItem)
	{
		CurrentWaitingItem = nullptr;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AItemSpawner::TrySpawnItem, RespawnTime, false);
	}
}
void AItemSpawner::OnTrackedItemDestroyed(AActor* DestroyedActor)
{
	if (DestroyedActor == CurrentWaitingItem)
	{
		ExtractItem();
	}
}
