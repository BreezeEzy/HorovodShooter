#include "HorovodPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "MainCharacter.h" // Подключаем, чтобы видеть метод ResetCharacterState

void AHorovodPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 1. Инициализация Ввода
	// Мы делаем это здесь, чтобы контроллер сразу владел контекстом.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AHorovodPlayerController::OnPawnDeath()
{
	// 1. Отключаем управление (Input)
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(DefaultMappingContext);
	}

	// 2. Показываем UI "Game Over"
	ShowGameOverUI();

	// 3. Запускаем таймер респауна
	// Безопасно, так как Контроллер не уничтожается при смерти пешки.
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AHorovodPlayerController::RespawnPlayer, RespawnDelay, false);
	}
}

void AHorovodPlayerController::RespawnPlayer()
{
	// 1. Ищем точку старта
	AActor* FoundStart = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass());
	if (!FoundStart)
	{
		UE_LOG(LogTemp, Error, TEXT("CRITICAL: No PlayerStart found on the level!"));
		return;
	}

	// 2. Получаем нашу пешку (Pawn) и сбрасываем её
	// Мы кастим к AMainCharacter, чтобы вызвать специфичный метод сброса физики
	AMainCharacter* MyPawn = Cast<AMainCharacter>(GetPawn());
	if (MyPawn)
	{
		// Телепортируем (Квантовый скачок)
		MyPawn->TeleportTo(FoundStart->GetActorLocation(), FoundStart->GetActorRotation());
		
		// Сбрасываем физическое состояние (внутри персонажа)
		MyPawn->ResetCharacterState();
	}

	// 3. Синхронизируем камеру с поворотом точки старта
	SetControlRotation(FoundStart->GetActorRotation());

	// 4. Возвращаем управление
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// 5. Убираем UI
	HideGameOverUI();
}

void AHorovodPlayerController::ShowGameOverUI()
{
	if (GameOverWidgetClass && !GameOverWidgetInstance)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
	}

	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->AddToViewport();
	}
}

void AHorovodPlayerController::HideGameOverUI()
{
	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromParent();
		GameOverWidgetInstance = nullptr; // Сбрасываем, чтобы создать новый в следующий раз
	}
}