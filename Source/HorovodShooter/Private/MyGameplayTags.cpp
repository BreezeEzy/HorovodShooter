// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayTags.h"
#include "GameplayTagsManager.h"

FMyGameplayTags FMyGameplayTags::GameplayTags;

void FMyGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	
	auto AddTag = [&](FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
	{
		OutTag = Manager.AddNativeGameplayTag(FName(TagName), FString(ANSI_TO_TCHAR(TagComment)));
	};
	
	AddTag(GameplayTags.Input_Move, "Input_Move", "Movement Input");
	AddTag(GameplayTags.Input_Jump, "Input_Jump", "Jump Input");
	
	AddTag(GameplayTags.Damage, "Damage", "Base Damage Tag");
	AddTag(GameplayTags.Damage_Contact, "Damage.Contact", "Instant kill");
	AddTag(GameplayTags.Damage_Physical_Blunt, "Damage.Physical.Blunt", "Impact Damage");
	AddTag(GameplayTags.Damage_Physical_Sharp, "Damage.Physical.Sharp", "Cutting Damage");
	
	AddTag(GameplayTags.Weakness_Blunt, "Weakness.Physical.Blunt", "Impact Weakness");
	AddTag(GameplayTags.Weakness_Sharp, "Weakness.Physical.Sharp", "Cutting Weakness");
}




