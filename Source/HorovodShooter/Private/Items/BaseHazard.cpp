// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BaseHazard.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/DamagableInterface.h"
#include "Interfaces/StatusReceiverInterface.h"

// Sets default values
ABaseHazard::ABaseHazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	RootComponent = TriggerArea;
	
	TriggerArea->SetCollisionProfileName(TEXT("Custom"));
	TriggerArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerArea->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	HazardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HazardMesh"));
	HazardMesh->SetupAttachment(RootComponent);
	HazardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABaseHazard::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ABaseHazard::ABaseHazard::OnHazardBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &ABaseHazard::OnHazardEndOverlap);
	if (PulseInterval > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(PulseTimerHandle, this, &ABaseHazard::HazardPulse, PulseInterval, true);
	}
}

void ABaseHazard::ApplyStatusEffect(AActor* Victim)
{
	if (!IsValid(Victim)) {return;}
	
	for (const FGameplayTag& Tag : HazardTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Status")))
		{
			if (Victim->Implements<UStatusReceiverInterface>())
			{
				IStatusReceiverInterface::Execute_ReceiveStatusEffect(Victim, Tag, StatusDuration);
			}
		}
		else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Damage")))
		{
			if (Victim->Implements<UDamagableInterface>())
			{
				FGameplayTagContainer SingleTagContainer;
				SingleTagContainer.AddTag(Tag);
				
				IDamagableInterface::Execute_TakeDamage(Victim, SingleTagContainer);
			}
		}
	}
}

void ABaseHazard::OnHazardBeginOverlap(UPrimitiveComponent* OverlappedComponents, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !VictimsInside.Contains(OtherActor))
	{
		VictimsInside.Add(OtherActor);
		ApplyStatusEffect(OtherActor);
	}
}

void ABaseHazard::OnHazardEndOverlap(UPrimitiveComponent* OverlappedComponents, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		VictimsInside.Remove(OtherActor);
	}
}

void ABaseHazard::HazardPulse()
{
	VictimsInside.RemoveAll([](AActor* Victim) {return !IsValid(Victim);});
	
	for (AActor* Victim : VictimsInside)
	{
		ApplyStatusEffect(Victim);
	}
}



