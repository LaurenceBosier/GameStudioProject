// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InteractiveToolBuilder.h"
#include "BaseTools/SingleClickTool.h"
#include "GSPQuestEditorSimpleTool.generated.h"

/**
 * Builder for UGSPQuestEditorSimpleTool
 */
UCLASS()
class GSPQUESTEDITOR_API UGSPQuestEditorSimpleToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }

	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};



/**
 * Settings UObject for UGSPQuestEditorSimpleTool. This UClass inherits from UInteractiveToolPropertySet,
 * which provides an OnModified delegate that the Tool will listen to for changes in property values.
 */
UCLASS(Transient)
class GSPQUESTEDITOR_API UGSPQuestEditorSimpleToolProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()
public:
	UGSPQuestEditorSimpleToolProperties();

	UPROPERTY(EditAnywhere, Category = "Quest Creator", meta = (DisplayName = "Quest Name"))
	FString QuestName;

	UPROPERTY(EditAnywhere, Category = "Quest Creator",  meta = (DisplayName = "Quest Description", MultiLine =true))
	FString QuestDescription;
};




/**
 * UGSPQuestEditorSimpleTool is an example Tool that opens a message box displaying info about an actor that the user
 * clicks left mouse button. All the action is in the ::OnClicked handler.
 */
UCLASS()
class GSPQUESTEDITOR_API UGSPQuestEditorSimpleTool : public USingleClickTool
{
	GENERATED_BODY()

public:
	UGSPQuestEditorSimpleTool();

	virtual void SetWorld(UWorld* World);

	virtual void Setup() override;

	virtual void OnClicked(const FInputDeviceRay& ClickPos);


protected:
	UPROPERTY()
	TObjectPtr<UGSPQuestEditorSimpleToolProperties> Properties;


protected:
	/** target World we will raycast into to find actors */
	UWorld* TargetWorld;
};