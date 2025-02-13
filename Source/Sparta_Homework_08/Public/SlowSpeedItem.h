#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "SlowSpeedItem.generated.h"


UCLASS()
class SPARTA_HOMEWORK_08_API ASlowSpeedItem : public ABaseItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Item|Property")
	float SlowSpeedAmount;
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Item|Property")
	float MaintainTime;
	
public:
	ASlowSpeedItem();
	
	virtual void ActivateItem(AActor* Activator) override;
};
