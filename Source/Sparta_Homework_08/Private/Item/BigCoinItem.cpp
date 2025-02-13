#include "Item/BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	ItemType = "BigCoin";
	PointValue = 50;
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	ACoinItem::ActivateItem(Activator);
}
