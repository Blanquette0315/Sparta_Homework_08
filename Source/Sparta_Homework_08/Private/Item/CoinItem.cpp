#include "Item/CoinItem.h"
#include "GameBase/SpartaGameState.h"

ACoinItem::ACoinItem()
	: PointValue(0)
{
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected(PointValue);
			}
		}
		
		DestroyItem();
	}
}
