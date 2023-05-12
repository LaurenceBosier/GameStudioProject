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
	if (!InItem.EquipmentDataAsset)
	{
		return false;
	}

	switch (InItem.EquipmentDataAsset->ItemType)
	{
	case EItemCategory::Artifact:
		ArtifactsArray.Add(InItem);
		break;
	case EItemCategory::CraftingResource:
		CraftingResourcesArray.Add(InItem);
		break;
	default:
		break;
	}

	return true;
}

bool UGSPInventoryComponent::AddEquipmentItem(FEquipmentItemInst InEquipment)
{
	if (!InEquipment.EquipmentDataAsset)
	{
		return false;
	}

	switch (InEquipment.EquipmentDataAsset->ItemType)
	{
	case EItemCategory::Axe:
		AxesArray.Add(InEquipment);
		break;
	case EItemCategory::Spear:
		SpearsArray.Add(InEquipment);
		break;
	case EItemCategory::Sword:
		SwordsArray.Add(InEquipment);
		break;
	case EItemCategory::Helmet:
		HelmetsArray.Add(InEquipment);
		break;
	case EItemCategory::ChestPlate:
		ChestplatesArray.Add(InEquipment);
		break;
	default:
		break;
	}

	return true;
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







