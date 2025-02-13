#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

class AActorPool;
class ASpartaPlayerController;

UCLASS()
class SPARTA_HOMEWORK_08_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()

public:
	float AccTime;

private:
	AActorPool* FindActorPool;
	
public:
	ASpartaGameState();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coin")
	int32 CollectedCoinCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lap|Property")
	int32 CurrentLap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lap|Property")
	int32 MaxLap;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lap|Property")
	int32 LastCheckPointNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lap|Property")
	bool CanIncreaseLap;
	
	FTimerHandle LevelTimerHandle;
	FTimerHandle VelocityHUDUpdateTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	
	UFUNCTION(BlueprintPure,Category="Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable,Category="Score")
	void AddScore(int32 Amount);
	// UFUNCTION(BlueprintCallable,Category="Level")
	// void OnGameOver();
	//
	void StartLevel();
	void NextLap();
	void OnCoinCollected(int32 Amount);
	// void EndLevel();
	//

	void ClearSpawner();
	
	void UpdateHUD();
	void UpdateHUDbyTick();
	
	void VelocitiyHUDUpdate();
	void TimeHUDUpdate();
	void PlayScoreAnimation(int32 Amount);

	const int32 GetLastCheckPointNum() {return LastCheckPointNum;}
	void SetLasteCheckPointNum(int32 CheckPointNum) {LastCheckPointNum = CheckPointNum;}
	void IncreaseLap();
	void LapUIUpdate();
	void SetChangeLap(bool set) {CanIncreaseLap = set;};

	void HPBarHUDUpdate();


	AActorPool* GetActorPool() {return FindActorPool;}
};
