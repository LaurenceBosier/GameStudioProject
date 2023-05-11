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
	bool GetInventoryItems(EItemCategory InItemCategory, TArray<FInventoryItemInst>& OutItems);

	UFUNCTION(BlueprintCallable, Category = "Inventory Access")
	bool GetEquipmentItems(EItemCategory InItemCategory, TArray<FEquipmentItemInst>& OutItems);

	UPROPERTY(EditAnywhere, Category = "Inventory Defaults")
	TArray<FInventoryItemInst> DefaultInventoryItems;

	UPROPERTY(EditAnywhere, Category = "Inventory Defaults")
	TArray<FEquipmentItemInst> DefaultInventoryEquipment;


protected:

	/* Item Containers */

	TArray<FInventoryItemInst> CraftingResourcesArray;
	TArray<FInventoryItemInst> ArtifactsArray;

	TArray<FEquipmentItemInst> SwordsArray;
	TArray<FEquipmentItemInst> AxesArray;
	TArray<FEquipmentItemInst> SpearsArray;

	TArray<FEquipmentItemInst> ChestplatesArray;
	TArray<FEquipmentItemInst> HelmetsArray;
};
