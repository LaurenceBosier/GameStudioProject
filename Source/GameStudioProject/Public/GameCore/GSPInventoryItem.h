// Game Studio Project Team F 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GSPInventoryItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	InventoryItem, 
	Axe,
	Sword,
	Spear,
	Helmet,
	ChestPlate,
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	COMMON, 
	RARE,
	EPIC,
	LEGENDARY,
	EXOTIC,
};


/**
 * 
 */
UCLASS()
class GAMESTUDIOPROJECT_API UGSPInventoryItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TEnumAsByte<EItemType> ItemType;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TEnumAsByte<EItemRarity> ItemRarity;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int ItemLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (EditCondition = "ItemType != EItemType::InventoryItem ", EditConditionHides))
	TArray<FText> DefaultEquipmentBuffs;

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Items", GetFName());
	}

};
