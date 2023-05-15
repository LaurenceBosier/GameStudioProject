// Game Studio Project Team F 2023 - Laurence Bosier
#include "Gameplay/GSPInventoryComponent.h"

void UGSPInventoryComponent::BeginPlay()
{

	for (auto& EquipmentItem : DefaultInventoryEquipment)
	{
		AddEquipmentItem(EquipmentItem);
	}

	for (auto& InventoryItem : DefaultInventoryItems)
	{
		AddInventoryItem(InventoryItem);
	}

	Super::BeginPlay();
}

bool UGSPInventoryComponent::AddInventoryItem(FInventoryItemInst InItem)
{
	if (!InItem.DataAsset || InItem.DataAsset->ItemCategory != EItemCategory::Item)
	{
		return false;
	}

	AddedItem = true;

	//Find the item type in the inventory item map
	auto& a = InventoryItemsMap.FindOrAdd(InItem.DataAsset->ItemType);

	//Add the item to an existing stack
	for (FInventoryItemInst& Item : a)
	{
		if(Item.DataAsset->ItemName.ToString() == InItem.DataAsset->ItemName.ToString())
		{
			Item.CurrentStackSize += InItem.CurrentStackSize;
			return true;
		}
	}

	//Add the item to a new stack
	a.Add(InItem);

	return true;
}

bool UGSPInventoryComponent::AddEquipmentItem(FEquipmentItemInst InEquipment)
{

	if (!InEquipment.EquipmentDataAsset || InEquipment.EquipmentDataAsset->ItemCategory != EItemCategory::Equipment)
	{
		return false;
	}
	AddedEquipment = true;

	auto& a = EquipmentItemsMap.FindOrAdd(InEquipment.EquipmentDataAsset->EquipmentType);

	a.Add(InEquipment);

	return true;
}

bool UGSPInventoryComponent::RemoveInventoryItem(FInventoryItemInst InItem)
{
	auto a = InventoryItemsMap.Find(InItem.DataAsset->ItemType);

	if(!a)
	{
		return false;
	}

	a->Remove(InItem);


	return true;
}

bool UGSPInventoryComponent::RemoveInventoryEquipment(FEquipmentItemInst InEquipment)
{

	auto a = EquipmentItemsMap.Find(InEquipment.EquipmentDataAsset->EquipmentType);

	if (!a)
	{
		return false;
	}

	a->Remove(InEquipment);

	return true;
}


bool UGSPInventoryComponent::GetInventoryItems(EItemType InItemType, TArray<FInventoryItemInst>& OutItems)
{
	OutItems.Empty();

	if(InItemType == EItemType::ALL)
	{
		for (const auto& ItemArray : InventoryItemsMap)
		{
			OutItems += ItemArray.Value;
		}
		return true;
	}

	auto a = InventoryItemsMap.Find(InItemType);

	if (!a)
	{
		return false;
	}

	OutItems = *a;

	return true;
}

bool UGSPInventoryComponent::GetEquipmentItems(EEquipmentType InEquipmentType, TArray<FEquipmentItemInst>& OutItems)
{
	OutItems.Empty();

	if (InEquipmentType == EEquipmentType::ALL)
	{
		for (const auto& EquipmentArray : EquipmentItemsMap)
		{
			OutItems += EquipmentArray.Value;
		}
		return true;
	}

	auto a = EquipmentItemsMap.Find(InEquipmentType);

	if(!a)
	{
		return false;
	}

	OutItems = *a;

	return true;
}









