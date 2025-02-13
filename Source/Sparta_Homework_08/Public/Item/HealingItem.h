#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class SPARTA_HOMEWORK_08_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item|Property")
	int32 HealAmount;
	
public:
	AHealingItem();

	virtual void ActivateItem(AActor* Activator) override;
};
