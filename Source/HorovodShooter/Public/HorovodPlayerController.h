#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HorovodPlayerController.generated.h"

// Forward declarations для сокращения времени компиляции
class UInputMappingContext;
class UUserWidget;

/**
 * Контроллер отвечает за правила игры (Game Flow) для локального игрока.
 * Он управляет UI, Смертью и Респауном.
 */
UCLASS()
class HOROVODSHOOTER_API AHorovodPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// Вызывается из AMainCharacter, когда здоровье падает до нуля.
	// Запускает цепочку Game Over.
	void OnPawnDeath();

protected:
	// --- INPUT SETTINGS ---
	// Контекст теперь живет здесь, так как контроллер решает, когда включать/выключать управление.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// --- UI SETTINGS ---
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	// Ссылка на созданный виджет, чтобы потом его удалить
	UPROPERTY()
	TObjectPtr<UUserWidget> GameOverWidgetInstance;

	// --- RESPAWN SETTINGS ---
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float RespawnDelay = 2.0f;

	FTimerHandle RespawnTimerHandle;

	// Основная логика возрождения (вызывается таймером)
	void RespawnPlayer();

	// Вспомогательные функции для UI
	void ShowGameOverUI();
	void HideGameOverUI();
};