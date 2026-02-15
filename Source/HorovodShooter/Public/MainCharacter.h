#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DamagableInterface.h"
#include "GameplayTagContainer.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UGrabberComponent;


UCLASS()
class HOROVODSHOOTER_API AMainCharacter : public ACharacter, public IDamagableInterface
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Интерфейс получения урона
	virtual void TakeDamage_Implementation(const FGameplayTagContainer& IncomingDamageTags) override;

	// Функция полного сброса состояния (вызывается Контроллером при респауне)
	void ResetCharacterState();

protected:
	// Событие для Блюпринтов (Звуки, Партиклы смерти)
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnPlayerDied();

	// --- COMPONENTS ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UGrabberComponent> GrabberComponent;

	// --- INPUT ACTIONS ---
	// MappingContext уехал в контроллер, здесь только действия
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> DashAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	// --- DASH SETTINGS ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = "0.0"))
	float DashForce = 4000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = "0.0"))
	float DashDuration = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = "0.0"))
	float DashCooldown = 1.0f;

	// --- STATE ---
	bool bIsDead = false;
	bool bCanDash = true;

	// Таймеры Дэша
	FTimerHandle DashTimerHandle;     // Кулдаун
	FTimerHandle DashDurationTimer;   // Время скольжения

	// Переменные для хранения дефолтного трения
	float DefaultGroundFriction;
	float DefaultBrakingDeceleration;

	// --- INPUT HANDLERS ---
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	
	// Вспомогательные функции Дэша
	void StopDashing();
	void ResetDash();
	
	void OnPrimaryAction();
};