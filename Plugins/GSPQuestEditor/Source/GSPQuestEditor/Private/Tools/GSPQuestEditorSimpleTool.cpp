// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPQuestEditorSimpleTool.h"
#include "InteractiveToolManager.h"
#include "ToolBuilderUtil.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"

// localization namespace
#define LOCTEXT_NAMESPACE "GSPQuestEditorSimpleTool"

/*
 * ToolBuilder implementation
 */

UInteractiveTool* UGSPQuestEditorSimpleToolBuilder::BuildTool(const FToolBuilderState& SceneState) const
{
	UGSPQuestEditorSimpleTool* NewTool = NewObject<UGSPQuestEditorSimpleTool>(SceneState.ToolManager);
	NewTool->SetWorld(SceneState.World);
	return NewTool;
}



/*
 * ToolProperties implementation
 */

UGSPQuestEditorSimpleToolProperties::UGSPQuestEditorSimpleToolProperties()
{
	//Set defaults here
	QuestName = TEXT("Enter Quest Name");
}


/*
 * Tool implementation
 */

UGSPQuestEditorSimpleTool::UGSPQuestEditorSimpleTool()
{
}


void UGSPQuestEditorSimpleTool::SetWorld(UWorld* World)
{
	this->TargetWorld = World;
}


void UGSPQuestEditorSimpleTool::Setup()
{
	USingleClickTool::Setup();

	Properties = NewObject<UGSPQuestEditorSimpleToolProperties>(this, "DoesntWork");
	AddToolPropertySource(Properties);
}


void UGSPQuestEditorSimpleTool::OnClicked(const FInputDeviceRay& ClickPos)
{
	// we will create actor at this position
	FVector NewActorPos = FVector::ZeroVector;

	// cast ray into world to find hit position
	FVector RayStart = ClickPos.WorldRay.Origin;
	FVector RayEnd = ClickPos.WorldRay.PointAt(99999999.f);
	FCollisionObjectQueryParams QueryParams(FCollisionObjectQueryParams::AllObjects);
	FHitResult Result;
	if (TargetWorld->LineTraceSingleByObjectType(Result, RayStart, RayEnd, QueryParams))
	{
		
	}
}


#undef LOCTEXT_NAMESPACE