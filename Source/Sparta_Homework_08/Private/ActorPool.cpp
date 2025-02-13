#include "ActorPool.h"

AActorPool::AActorPool()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AActorPool::BeginPlay()
{
	Super::BeginPlay();

	InitializePool();
}

void AActorPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorPool::InitializePool()
{
	for (const auto& PoolSetting : ActorClasses)
	{
		TSubclassOf<AActor> ActorClass = PoolSetting.Key;
		int32 PoolSize = PoolSetting.Value;

		if (!ActorClass || !ActorClass->IsChildOf(AActor::StaticClass()))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid ActorClass in pool!"));
			continue;
		}
		
		TArray<AActor*>& PoolArray = ActorPools.FindOrAdd(ActorClass);

		for (int32 i = 0; i < PoolSize; ++i)
		{
			AActor* NewObject = GetWorld()->SpawnActor<AActor>(ActorClass);
			if (!NewObject)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn actor of class: %s"), *ActorClass->GetName());
				continue;
			}

			NewObject->SetActorHiddenInGame(true);
			NewObject->SetActorEnableCollision(false);
			NewObject->SetActorTickEnabled(false);

			PoolArray.Add(NewObject);
		}
	}
}

AActor* AActorPool::GetPoolActor(const TSubclassOf<AActor>& ActorClass)
{
	 if (!ActorPools.Contains(ActorClass))
	 {
	 	return nullptr;
	 }
	
	 TArray<AActor*>* ActorArray = ActorPools.Find(ActorClass);
	 if (!ActorArray)
	 {
	 	return nullptr;
	 }
	
	 for (AActor* Actor : *ActorArray)
	 {
	 	// 비활성화된 오브젝트가 있는지
	 	if (!Actor->IsActorTickEnabled())
	 	{
	 		Actor->SetActorHiddenInGame(false);
	 		Actor->SetActorEnableCollision(true);
	 		Actor->PrimaryActorTick.bCanEverTick = true;
	 		Actor->SetActorTickEnabled(true);
	 		return Actor;
	 	}
	 }
	 return nullptr;
}

void AActorPool::ReturnActorToPool(AActor* PoolActor)
{
	PoolActor->SetActorHiddenInGame(true);
	PoolActor->SetActorEnableCollision(false);
	PoolActor->SetActorTickEnabled(false);
}

void AActorPool::ResetPool()
{
	for (const auto& PoolSetting : ActorClasses)
	{
		TSubclassOf<AActor> ActorClass = PoolSetting.Key;
		int32 PoolSize = PoolSetting.Value;

		if (!ActorClass || !ActorClass->IsChildOf(AActor::StaticClass()))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid ActorClass in pool!"));
			continue;
		}
		
		TArray<AActor*>& PoolArray = ActorPools.FindOrAdd(ActorClass);

		for (int32 i = 0; i < PoolSize; ++i)
		{
			// 활성화된 오브젝트가 있는지
			if (PoolArray[i]->IsActorTickEnabled())
			{
				PoolArray[i]->SetActorHiddenInGame(true);
				PoolArray[i]->SetActorEnableCollision(false);
				PoolArray[i]->SetActorTickEnabled(false);
			}
		}
	}
}