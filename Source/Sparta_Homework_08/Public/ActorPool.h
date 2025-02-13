#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPool.generated.h"

UCLASS()
class SPARTA_HOMEWORK_08_API AActorPool : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="ActorPools")
	TMap<TSubclassOf<AActor>,int32> ActorClasses;
	
	TMap<TSubclassOf<AActor>, TArray<AActor*>> ActorPools;
	
public:	
	AActorPool();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void InitializePool();
	AActor* GetPoolActor(const TSubclassOf<AActor>& ActorClass);
	void ReturnActorToPool(AActor* PoolActor);

	void ResetPool();
};
