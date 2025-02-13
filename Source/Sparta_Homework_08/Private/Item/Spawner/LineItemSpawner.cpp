#include "Item/Spawner/LineItemSpawner.h"

#include "ActorPool.h"
#include "Components/SplineComponent.h"
#include "GameBase/SpartaGameState.h"
#include "Item/Data/ItemSpawnRow.h"
#include "Kismet/GameplayStatics.h"

ALineItemSpawner::ALineItemSpawner()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComp->SetupAttachment(Scene);
}

void ALineItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	ClearSpawnActorData();
}

AActor* ALineItemSpawner::SpawnRandomItem()
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

AActor* ALineItemSpawner::SpawnItem(const TSubclassOf<AActor>& ItemClass)
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
		// 	GetRandomPointInLine(),
		// 	FRotator::ZeroRotator, Param);

		AActor* SpawnedItem = ActorPool->GetPoolActor(ItemClass);
		if (SpawnedItem)
		{
			SpawnLocation = GetRandomPointInLine();
			FVector Origin;
			FVector BoxExtent;
			SpawnedItem->GetActorBounds(false, Origin, BoxExtent);
			SpawnScale = BoxExtent;
		
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

	return nullptr;
}

FVector ALineItemSpawner::GetRandomPointInLine() const
{
	float SplineLength = SplineComp->GetSplineLength();

	float RandomDistance = FMath::RandRange(0.0f, SplineLength);

	// 해당 거리에서 월드 위치 가져오기
	FVector SpawnLocation = SplineComp->GetLocationAtDistanceAlongSpline(RandomDistance, ESplineCoordinateSpace::World);
	return SpawnLocation;
}

void ALineItemSpawner::ClearSpawnActorData()
{
	SpawnedLocations.Empty();
	SpawnedScales.Empty();
}
