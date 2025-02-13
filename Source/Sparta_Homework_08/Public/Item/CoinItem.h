#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class SPARTA_HOMEWORK_08_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Property")
	int32 PointValue;
	
public:
	ACoinItem();

	virtual void ActivateItem(AActor* Activator) override;
};
