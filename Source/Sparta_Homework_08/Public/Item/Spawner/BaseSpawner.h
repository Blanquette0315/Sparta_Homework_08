#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpawner.generated.h"

UCLASS()
class SPARTA_HOMEWORK_08_API ABaseSpawner : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseSpawn|Copmonent")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseSpawn|Property")
	UDataTable* SpawnTargetDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseSpawn|Property")
	int32 SpawnCount;
	
public:	
	ABaseSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	const int32 GetSpawnCount() {return SpawnCount;}

	virtual void ClearSpawnActorData() {};
	
	template <typename T>
	T* GetRandomItem() const;
};

template <typename T>
T* ABaseSpawner::GetRandomItem() const
{
	if (!SpawnTargetDataTable) return nullptr;

	TArray<T*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	SpawnTargetDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const T* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulatedChance = 0.0f;

	for (T* Row : AllRows)
	{
		AccumulatedChance += Row->SpawnChance;
		if (RandValue <= AccumulatedChance)
		{
			return Row;
		}
	}

	return nullptr;
}
