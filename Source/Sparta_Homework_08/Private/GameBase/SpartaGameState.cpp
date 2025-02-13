#include "GameBase/SpartaGameState.h"

#include "ActorPool.h"
#include "Item/CoinItem.h"
#include "GameBase/SpartaGameInstance.h"
#include "Player/SpartaPlayerController.h"
#include "Item/Spawner/SpawnVolume.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Item/Spawner/LineItemSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DronePawn.h"

ASpartaGameState::ASpartaGameState()
	: AccTime(0.0f)
	, Score(0.0f)
	, SpawnedCoinCount(0)
	, CollectedCoinCount(0)
	, LevelDuration(30.0f)
	, CurrentLevelIndex(0)
	, MaxLevels(2)
	, CurrentLap(0)
	, MaxLap(3)
	, LastCheckPointNum(0)
	, CanIncreaseLap(true)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	// 오브젝트 풀 세팅
	TArray<AActor*> ActorPools;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActorPool::StaticClass(), ActorPools);
	if (!ActorPools[0])
		return;
	if (Cast<AActorPool>(ActorPools[0]))
	{
		FindActorPool = Cast<AActorPool>(ActorPools[0]);
	}
	
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true);
}

void ASpartaGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccTime += DeltaTime;

	UpdateHUDbyTick();
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance*  GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}
	
	if (UGameInstance*  GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex =  SpartaGameInstance->CurrentLevelIndex;
		}
	}
	
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseSpawner::StaticClass(), FoundVolumes);
	if(FoundVolumes.Num() > 0)
	{
		for (int idx = 0; idx < FoundVolumes.Num(); idx++)
		{
			IItemSpawnerInterface* SpawnVolume = Cast<IItemSpawnerInterface>(FoundVolumes[idx]);

			if (ABaseSpawner* Spawner = Cast<ABaseSpawner>(FoundVolumes[idx]))
			{
				const int32 ItemToSpawnCount = Spawner->GetSpawnCount();
			
				for (int32 i = 0; i < ItemToSpawnCount; i++)
				{
					if (SpawnVolume)
					{
						AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
						if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
						{
							SpawnedCoinCount++;
						}
					}
				}
			}
		}
	}

	UpdateHUD();
	LapUIUpdate();
	HPBarHUDUpdate();
}

void ASpartaGameState::ClearSpawner()
{
	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseSpawner::StaticClass(), FoundSpawners);

	if (FoundSpawners.Num() > 0)
	{
		for (auto SpawnerActor : FoundSpawners)
		{
			if (ABaseSpawner* Spawner = Cast<ABaseSpawner>(SpawnerActor))
			Spawner->ClearSpawnActorData();
		}
	}
}

void ASpartaGameState::NextLap()
{
	if (!FindActorPool)
		return;

	if (FindActorPool)
	{
		// 이전 아이템들 모두 반환해 주어야 함.
		// TArray<AActor*> FoundItems;
		// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseItem::StaticClass(), FoundItems);
		// if(FoundItems.Num() > 0)
		// {
		// 	for (int idx = 0; idx < FoundItems.Num(); idx++)
		// 	{
		// 		ActorPool->ReturnActorToPool(FoundItems[idx]);
		//    }
		// }
		FindActorPool->ResetPool();
		// 스포너들 위치, 스케일 정보 초기화
		ClearSpawner();
		//다시 랜덤 스폰을 해주어야 함.
		StartLevel();
	}
}

void ASpartaGameState::OnCoinCollected(int32 Amount)
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected : %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount);
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		//EndLevel();
	}

	PlayScoreAnimation(Amount);
}
//
// void ASpartaGameState::EndLevel()
// {
// 	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
// 	CurrentLevelIndex++;
//
// 	if (UGameInstance*  GameInstance = GetGameInstance())
// 	{
// 		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
// 		if (SpartaGameInstance)
// 		{
// 			AddScore(Score);
// 			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
// 		}
//
// 		if (CurrentLevelIndex >= MaxLevels)
// 		{
// 			OnGameOver();
// 			return;
// 		}
//
// 		if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
// 		{
// 			UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
// 		}
// 		else
// 		{
// 			OnGameOver();
// 		}
// 	}
// }
//
// void ASpartaGameState::OnGameOver()
// {
// 	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
// 	{
// 		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
// 		{
// 			SpartaPlayerController->SetPause(true);
// 			SpartaPlayerController->ShowMainMenu(true);
// 		}
// 	}
// }
//
 void ASpartaGameState::UpdateHUD()
 {
 	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
 	{
 		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
 		{
 			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
 			{
 				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HUD_Score"))))
 				{
 					if (UGameInstance* GameInstance = GetGameInstance())
 					{
 						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
 						if (SpartaGameInstance)
 						{
 							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score . %d"), SpartaGameInstance->TotalScore)));
 						}
 					}
 				}

 				//if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
 				//{
 				//	LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level : %d"), CurrentLevelIndex + 1)));
 				//}
 			}
 		}
 	}
 }

void ASpartaGameState::UpdateHUDbyTick()
{
	VelocitiyHUDUpdate();
	TimeHUDUpdate();
}

void ASpartaGameState::LapUIUpdate()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* CurrentSpeedText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("LapOf"))))
				{
					CurrentSpeedText->SetText(FText::FromString(FString::Printf(TEXT("Lap %d of %d"), CurrentLap, MaxLap)));
				}
			}
		}
	}
}

void ASpartaGameState::HPBarHUDUpdate()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (APawn* PlayerPawn = SpartaPlayerController->GetPawn())
				{
					if (ADronePawn* DronePawn = Cast<ADronePawn>(PlayerPawn))
					{
						float DroneHPPercentage = DronePawn->GetHealthPercentage();
						if (UProgressBar* HPBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("BP_Bar"))))
						{
							HPBar->SetPercent(DroneHPPercentage);
						}
					}
				}
			}
		}
	}
}

void ASpartaGameState::VelocitiyHUDUpdate()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (APawn* PlayerPawn = SpartaPlayerController->GetPawn())
				{
					if (ADronePawn* DronePawn = Cast<ADronePawn>(PlayerPawn))
					{
						if (UImage* SpeedBar_In = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("SpeedBar_In"))))
						{
							if (UMaterialInstanceDynamic* UIMaterial = SpeedBar_In->GetDynamicMaterial())
							{
								UIMaterial->SetScalarParameterValue(TEXT("Percent"), DronePawn->GetSpeedPercentage());
								UIMaterial->SetScalarParameterValue(TEXT("Thickness"), 0.2f);
							}
						}
							
						if (UTextBlock* CurrentSpeedText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CurrentSpeed"))))
						{
							const int32 CurrentSpeed = FMath::Floor<int32>(DronePawn->GetCurrentSpeed() / 10.0f);
							FString CurrentSpeedString = "";
							
							CurrentSpeedString += FString::Printf(TEXT("%d"), CurrentSpeed);
							CurrentSpeedText->SetText(FText::FromString(CurrentSpeedString));
						}
					}
				}
			}
		}
	}
}

void ASpartaGameState::TimeHUDUpdate()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				int32 Minutes = static_cast<int32>(AccTime) / 60;
				int32 Seconds = static_cast<int32>(AccTime) % 60;
				int32 Milliseconds = static_cast<int32>((AccTime - FMath::Floor(AccTime)) * 100);
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time_Min"))))
				{
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d"), Minutes)));
				}
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time_Sec"))))
				{
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d"), Seconds)));
				}
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time_MilliSec"))))
				{
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d"), Milliseconds)));
				} 
			}
		}
	}
}

void ASpartaGameState::PlayScoreAnimation(int32 Amount)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* AddScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("AddScore"))))
				{
					AddScoreText->SetText(FText::FromString(FString::Printf(TEXT("Add %d"), Amount)));
					AddScoreText->SetVisibility(ESlateVisibility::Visible);
				}
				
				UFunction* PlayAnimFunc = HUDWidget->FindFunction(FName("FAnimAddScore"));
				if (PlayAnimFunc)
				{
					HUDWidget->ProcessEvent(PlayAnimFunc, nullptr);
				}
			}
		}
	}
}

void ASpartaGameState::IncreaseLap()
{
	if (CanIncreaseLap)
	{
		CurrentLap++;
		LapUIUpdate();
		NextLap();
	}
}