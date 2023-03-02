// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolBuilder.h"
#include "BaseTools/ClickDragTool.h"
#include "GSPQuestEditorInteractiveTool.generated.h"


/**
 * Builder for UGSPQuestEditorInteractiveTool
 */
UCLASS()
class GSPQUESTEDITOR_API UGSPQuestEditorInteractiveToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& SceneState) const override { return true; }

	virtual UInteractiveTool* BuildTool(const FToolBuilderState& SceneState) const override;
};


/**
 * Property set for the UGSPQuestEditorInteractiveTool
 */
UCLASS(Transient)
class GSPQUESTEDITOR_API UGSPQuestEditorInteractiveToolProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()

public:
	UGSPQuestEditorInteractiveToolProperties();

	/** First point of measurement */
	UPROPERTY(EditAnywhere, Category = Options)
	FVector StartPoint;

	/** Second point of measurement */
	UPROPERTY(EditAnywhere, Category = Options)
	FVector EndPoint;
	
	/** Current distance measurement */
	UPROPERTY(EditAnywhere, Category = Options)
	double Distance;


	/** Current distance measurement */
	UPROPERTY(EditAnywhere, Category = Options)
	double ThisCouldBeAnything;

	UFUNCTION(CallInEditor)
	void PrintToLog();
};



/**
 * UGSPQuestEditorInteractiveTool is an example Tool that allows the user to measure the 
 * distance between two points. The first point is set by click-dragging the mouse, and
 * the second point is set by shift-click-dragging the mouse.
 */
UCLASS()
class GSPQUESTEDITOR_API UGSPQuestEditorInteractiveTool : public UInteractiveTool, public IClickDragBehaviorTarget
{
	GENERATED_BODY()

public:
	virtual void SetWorld(UWorld* World);

	/** UInteractiveTool overrides */
	virtual void Setup() override;
	virtual void Render(IToolsContextRenderAPI* RenderAPI) override;
	virtual void OnPropertyModified(UObject* PropertySet, FProperty* Property) override;

	/** IClickDragBehaviorTarget implementation */
	virtual FInputRayHit CanBeginClickDragSequence(const FInputDeviceRay& PressPos) override;

	virtual void OnClickPress(const FInputDeviceRay& PressPos) override;

	virtual void OnClickDrag(const FInputDeviceRay& DragPos) override;

	// these are not used in this Tool
	virtual void OnClickRelease(const FInputDeviceRay& ReleasePos) override {}

	virtual void OnTerminateDragSequence() override {}

	/** IModifierToggleBehaviorTarget implementation (inherited via IClickDragBehaviorTarget) */
	virtual void OnUpdateModifierState(int ModifierID, bool bIsOn) override;


protected:
	/** Properties of the tool are stored here */
	UPROPERTY()
	TObjectPtr<UGSPQuestEditorInteractiveToolProperties> Properties;


protected:
	UWorld* TargetWorld = nullptr;		// target World we will raycast into


	FInputRayHit FindRayHit(const FRay& WorldRay, FVector& HitPos);		// raycasts into World

	void UpdatePosition(const FRay& WorldRay);					// updates first or second point based on raycast

	void UpdateDistance();										// updates distance
};
