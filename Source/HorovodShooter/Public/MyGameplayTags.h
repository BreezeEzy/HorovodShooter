// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FMyGameplayTags
{
public:
	static const FMyGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Input_Move;
	FGameplayTag Input_Jump;
	
	FGameplayTag Damage;
	FGameplayTag Damage_Contact;
	FGameplayTag Damage_Physical_Sharp;
	FGameplayTag Damage_Physical_Blunt;
	
	FGameplayTag Weakness_Sharp;
	FGameplayTag Weakness_Blunt;
	
protected:
	static FMyGameplayTags GameplayTags;
};