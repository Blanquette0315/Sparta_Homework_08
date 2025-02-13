#include "Item/Spawner/SpawnVolume.h"

#include "ActorPool.h"
#include "Components/BoxComponent.h"
#include "GameBase/SpartaGameState.h"
#include "Item/Data/ItemSpawnRow.h"
#include "Kismet/GameplayStatics.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	ClearSpawnActorData();
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (const FItemSpawnRow* SelectedRow = GetRandomItem<FItemSpawnRow>())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}

	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(const TSubclassOf<AActor>& ItemClass)
{
	if (!ItemClass)
		return nullptr;

	
	AActorPool* ActorPool = nullptr;
	if (UWorld* World = GetWorld())
	{
		if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
		{
			ActorPool= GameState->GetActorPool();
		}
	}

	if (!ActorPool)
		return nullptr;
	
	FVector SpawnLocation = FVector::ZeroVector;
	FVector SpawnScale = FVector::ZeroVector;
	bool bValidLocation = false;

	// 최대 10번 반복하여 적절한 위치 찾기
	for (int Attempt = 0; Attempt < 10; Attempt++)
	{
		bValidLocation = true;

		// FActorSpawnParameters Param;
		// Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		// AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(
		// 	ItemClass,
		// 	GetRandomPointInVolume(),
		// 	FRotator::ZeroRotator, Param);

		AActor* SpawnedItem = ActorPool->GetPoolActor(ItemClass);
		if (SpawnedItem)
		{
			SpawnLocation = GetRandomPointInVolume();
			SpawnScale = SpawnedItem->GetComponentByClass<UMeshComponent>()->Bounds.BoxExtent;
	
			if (SpawnedLocations.IsEmpty())
			{
				SpawnedLocations.Push(SpawnLocation);
				SpawnedScales.Push(SpawnScale);
				return SpawnedItem;
			}
			else
			{
				// 기존 액터들과 거리 체크
				for (int i = 0; i < SpawnedLocations.Num(); ++i)
				{
					float BetweenDist = FVector::Dist(SpawnLocation, SpawnedLocations[i]);
					if (BetweenDist < SpawnedScales[i].X + SpawnScale.X
						&& BetweenDist < SpawnedScales[i].Y + SpawnScale.Y
						&& BetweenDist < SpawnedScales[i].Z + SpawnScale.Z)
					{
						bValidLocation = false;
						break;
					}
				}
				if (bValidLocation)
				{
					SpawnedItem->SetActorLocation(SpawnLocation);
					SpawnedLocations.Push(SpawnLocation);
					SpawnedScales.Push(SpawnScale);
					return SpawnedItem;
				};
			}
		}
	}
	
	return nullptr;
}

void ASpawnVolume::ClearSpawnActorData()
{
	SpawnedLocations.Empty();
	SpawnedScales.Empty();
}