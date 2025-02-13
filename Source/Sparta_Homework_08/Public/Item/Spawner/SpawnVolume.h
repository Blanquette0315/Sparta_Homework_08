#pragma once

#include "CoreMinimal.h"
#include "BaseSpawner.h"
#include "ItemSpawnerInterface.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SPARTA_HOMEWORK_08_API ASpawnVolume : public ABaseSpawner, public IItemSpawnerInterface
{
	GENERATED_BODY()

private:
	TArray<FVector> SpawnedLocations;
	TArray<FVector> SpawnedScales;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning|Property")
	UBoxComponent* SpawningBox;

	virtual void BeginPlay() override;
	
public:	
	ASpawnVolume();
	
	
	UFUNCTION(BlueprintCallable, Category = "Spawning|Function")
	virtual AActor* SpawnRandomItem() override;
	virtual AActor* SpawnItem(const TSubclassOf<AActor>& ItemClass) override;
	
	FVector GetRandomPointInVolume() const;
	//FItemSpawnRow* GetRandomItem() const;

	virtual void ClearSpawnActorData() override;
};
