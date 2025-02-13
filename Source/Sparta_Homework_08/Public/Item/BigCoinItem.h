#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "BigCoinItem.generated.h"

UCLASS()
class SPARTA_HOMEWORK_08_API ABigCoinItem : public ACoinItem
{
	GENERATED_BODY()

protected:

public:
	ABigCoinItem();

	virtual void ActivateItem(AActor* Activator) override;
};
