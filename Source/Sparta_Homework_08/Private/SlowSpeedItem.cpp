#include "SlowSpeedItem.h"
#include "Player/DronePawn.h"

ASlowSpeedItem::ASlowSpeedItem()
	: SlowSpeedAmount(0.0f)
	, MaintainTime(0.0f)
{
}

void ASlowSpeedItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ADronePawn* PlayerCharacter = Cast<ADronePawn>(Activator))
		{
			PlayerCharacter->DecreaseSpeed(SlowSpeedAmount ,MaintainTime);
		}
		
		DestroyItem();
	}
}
