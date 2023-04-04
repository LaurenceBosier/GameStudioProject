// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GSPInventoryItem.generated.h"

//The category of item
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Artifact           UMETA(ToolTip="A non-equipable artifact, apperais in menu 'items' page."), 
	CraftingResource   UMETA(ToolTip="A non-equipable crafting resource, apperais in menu 'items' page."), 
	Sword              UMETA(ToolTip="An equipable sword, apperais in menu 'weapons' page."), 
	Spear              UMETA(ToolTip="An equipable spear, apperais in menu 'weapons' page."), 
	Axe                UMETA(ToolTip="An equipable axe, apperais in menu 'weapons' page."), 
	Helmet             UMETA(ToolTip="An equipable helemt pice, apperais in menu 'armour' page."),
	ChestPlate         UMETA(ToolTip="An equipable chestplate pice, apperais in menu 'armour' page.")
};


//The items rarity
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	COMMON, 
	RARE,
	EPIC,
	LEGENDARY,
	EXOTIC,
};

/* A read-only container of information about a specific inventory item */
UCLASS()
class GAMESTUDIOPROJECT_API UGSPInventoryItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/* Generic item attributes */

	//What is the use of the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TEnumAsByte<EItemCategory> ItemType;

	//The items display name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	//The items description
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	//The items in-game rarity
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TEnumAsByte<EItemRarity> ItemRarity;

	//How much the item is worth to sell to the blacksmith
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int ItemValue; 

	//The mesh used by the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UStaticMesh* ItemMesh;

	//If the inventory item is stack-able
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (EditCondition = "ItemType == EItemType::InventoryItem ", EditConditionHides))
	bool IsStackable = true;

	//The max stack size for the inventory item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (EditCondition = "IsStackable == true", EditConditionHides))
	int MaxStackSize = 999;

	/* Equipable  items */


	//The base damage inflicted by the weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Equipment", meta = (EditCondition = "ItemType == EItemType::Weapon ", EditConditionHides))
	int BaseDamage;

	//The base defense value of the armour 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Equipment", meta = (EditCondition = "ItemType == EItemType::Armour ", EditConditionHides))
	int BaseDefense;

	//The equipment buffs applied to the equipment by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Equipment", meta = (EditCondition = "ItemType != EItemType::InventoryItem ", EditConditionHides))
	TArray<FText> DefaultBuffs;


public:

	//Append Items to the asset id 
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Items", GetFName());
	}

};

USTRUCT(BlueprintType)
struct FEquipmentItemInst
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Item")
	UGSPInventoryItem* EquipmentDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Item")
	int EquipmentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment Item")
	TArray<FText> EquipmentBuffs;

};

USTRUCT(BlueprintType)
struct FInventoryItemInst
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
	UGSPInventoryItem* EquipmentDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
	int CurrentStackSize;

};


