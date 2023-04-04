// Game Studio Project Team F 2023 - Laurence Bosier


#include "Gameplay/GSPInventoryComponent.h"

void UGSPInventoryComponent::BeginPlay()
{

	for (auto EquipmentItem : DefaultInventoryEquipment)
	{
		if(!EquipmentItem.EquipmentDataAsset)
		{
			continue;
		}

		switch(EquipmentItem.EquipmentDataAsset->ItemType)
		{
		case EItemCategory::Axe:
			AxesArray.Add(EquipmentItem);
			break;
		case EItemCategory::Spear:
			SpearsArray.Add(EquipmentItem);
			break;
		case EItemCategory::Sword:
			SwordsArray.Add(EquipmentItem);
			break;
		case EItemCategory::Helmet:
			HelmetsArray.Add(EquipmentItem);
			break;
		case EItemCategory::ChestPlate:
			ChestplatesArray.Add(EquipmentItem);
			break;
		default:
			break;
		}
	}

	for (auto InventoryItem : DefaultInventoryItems)
	{
		if(!InventoryItem.EquipmentDataAsset)
		{
			continue;
		}

		switch(InventoryItem.EquipmentDataAsset->ItemType)
		{
		case EItemCategory::Artifact:
			ArtifactsArray.Add(InventoryItem);
			break;
		case EItemCategory::CraftingResource:
			CraftingResourcesArray.Add(InventoryItem);
			break;
		default:
			break;
		}
	}

	Super::BeginPlay();
}

bool UGSPInventoryComponent::GetInventoryItems(EItemCategory InItemCategory, TArray<FInventoryItemInst>& OutItems)
{
	bool bValid = true; //Todo remove dumb check

	switch (InItemCategory)
	{
	case EItemCategory::Artifact:
		OutItems = ArtifactsArray;
		break;
	case EItemCategory::CraftingResource:
		OutItems = CraftingResourcesArray;
		break;
	default:
		bValid = false;
	}

	return bValid;
}

bool UGSPInventoryComponent::GetEquipmentItems(EItemCategory InItemCategory, TArray<FEquipmentItemInst>& OutItems)
{
	bool bValid = true;

	switch (InItemCategory)
	{
	case EItemCategory::Spear:
		OutItems = SpearsArray;
		break;
	case EItemCategory::Sword:
		OutItems = SwordsArray;
		break;
	case EItemCategory::Axe:
		OutItems = AxesArray;
		break;
	case EItemCategory::Helmet:
		OutItems = HelmetsArray;
		break;
	case EItemCategory::ChestPlate:
		OutItems = ChestplatesArray;
		break;
	default:
		bValid = false;
	}

	return bValid;
}







