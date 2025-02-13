#include "Item/HealingItem.h"

#include "Player/DronePawn.h"
#include "Player/SpartaCharacter.h"


AHealingItem::AHealingItem()
{
	ItemType = "Healing";
	HealAmount = 20;
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ADronePawn* PlayerCharacter = Cast<ADronePawn>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		
		DestroyItem();
	}
}
