// Game Studio Project Team F 2023 - Laurence Bosier

#pragma once

#include "CoreMinimal.h"
#include "GameCore/GSPActorComponentBase.h"
#include "GameCore/GSPInventoryItem.h"
#include "GSPInventoryComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(GSP), 
	meta=(BlueprintSpawnableComponent))
class GAMESTUDIOPROJECT_API UGSPInventoryComponent : public UGSPActorComponentBase
{
	GENERATED_BODY()


public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool AddInventoryItem(FInventoryItemInst InItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool AddEquipmentItem(FEquipmentItemInst InEquipment);

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool RemoveInventoryItem(FInventoryItemInst InItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool RemoveInventoryEquipment(FEquipmentItemInst InEquipment);

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool GetInventoryItems(EItemType InItemType, TArray<FInventoryItemInst>& OutItems);

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool GetEquipmentItems(EEquipmentType InEquipmentType, TArray<FEquipmentItemInst>& OutItems);

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool HasNewInventoryItem() const { return AddedItem;  }

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool HasNewInventoryEquipment() const { return AddedEquipment; }

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	void ClearHasNewInventoryEquipment() { AddedEquipment = false; }

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	void ClearHasNewInventoryItem() { AddedItem = false; }


public:

	UPROPERTY(EditAnywhere, Category = "Inventory Defaults")
	TArray<FInventoryItemInst> DefaultInventoryItems;

	UPROPERTY(EditAnywhere, Category = "Inventory Defaults")
	TArray<FEquipmentItemInst> DefaultInventoryEquipment;


protected:

	/* Item Containers */

	TMap<EItemType, TArray<FInventoryItemInst>> InventoryItemsMap;
	bool AddedItem = false;

	TMap<EEquipmentType, TArray<FEquipmentItemInst>> EquipmentItemsMap;
	bool AddedEquipment = false;
};
