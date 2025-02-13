#include "Item/SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{
	ItemType = "SmallCoin";
	PointValue = 10;
}

void ASmallCoinItem::ActivateItem(AActor* Activator)
{
	ACoinItem::ActivateItem(Activator);
}


