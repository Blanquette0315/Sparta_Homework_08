#include "Item/Spawner/BaseSpawner.h"

ABaseSpawner::ABaseSpawner()
	: SpawnCount(10)
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawnTargetDataTable = nullptr;
}

void ABaseSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

