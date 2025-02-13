#pragma once

#include "CoreMinimal.h"
#include "BaseSpawner.h"
#include "ItemSpawnerInterface.h"
#include "LineItemSpawner.generated.h"

class USplineComponent;

UCLASS()
class SPARTA_HOMEWORK_08_API ALineItemSpawner : public ABaseSpawner, public IItemSpawnerInterface
{
	GENERATED_BODY()

private:
	TArray<FVector> SpawnedLocations;
	TArray<FVector> SpawnedScales;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LineSpawner|Component")
	USplineComponent* SplineComp;

	virtual void BeginPlay() override;
	
public:
	ALineItemSpawner();

	UFUNCTION(BlueprintCallable, Category = "Spawning|Function")
	virtual AActor* SpawnRandomItem() override;
	virtual AActor* SpawnItem(const TSubclassOf<AActor>& ItemClass) override;
	
	FVector GetRandomPointInLine() const;

	virtual void ClearSpawnActorData() override;
};
