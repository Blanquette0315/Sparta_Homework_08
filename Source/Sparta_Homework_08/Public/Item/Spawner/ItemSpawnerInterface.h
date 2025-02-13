#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemSpawnerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemSpawnerInterface : public UInterface
{
	GENERATED_BODY()
};


class SPARTA_HOMEWORK_08_API IItemSpawnerInterface
{
	GENERATED_BODY()

public:
	virtual AActor* SpawnRandomItem() = 0;
	virtual AActor* SpawnItem(const TSubclassOf<AActor>& ItemClass) = 0;
};
