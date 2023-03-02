// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPQuestEditorEditorMode.h"
#include "GSPQuestEditorEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "GSPQuestEditorEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/GSPQuestEditorSimpleTool.h"
#include "Tools/GSPQuestEditorInteractiveTool.h"

// step 2: register a ToolBuilder in FGSPQuestEditorEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "GSPQuestEditorEditorMode"

const FEditorModeID UGSPQuestEditorEditorMode::EM_GSPQuestEditorEditorModeId = TEXT("EM_GSPQuestEditorEditorMode");

FString UGSPQuestEditorEditorMode::SimpleToolName = TEXT("GSPQuestEditor_ActorInfoTool");

FString UGSPQuestEditorEditorMode::InteractiveToolName = TEXT("GSPQuestEditor_MeasureDistanceTool");


//Todo Quest Edit Tool?
//FString UGSPQuestEditorEditorMode::NewTestToolName = TEXT("GSPQuestEditor_QuestCreationTool");



UGSPQuestEditorEditorMode::UGSPQuestEditorEditorMode() 
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UGSPQuestEditorEditorMode::EM_GSPQuestEditorEditorModeId,
		LOCTEXT("ModeName", "GSPQuestEditor"),
		FSlateIcon(),
		true);
}


UGSPQuestEditorEditorMode::~UGSPQuestEditorEditorMode()
{
}


void UGSPQuestEditorEditorMode::ActorSelectionChangeNotify()
{
}

void UGSPQuestEditorEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FGSPQuestEditorEditorModeCommands& SampleToolCommands = FGSPQuestEditorEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UGSPQuestEditorInteractiveToolBuilder>(this));

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UGSPQuestEditorSimpleToolBuilder>(this));


	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UGSPQuestEditorEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FGSPQuestEditorEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UGSPQuestEditorEditorMode::GetModeCommands() const
{
	return FGSPQuestEditorEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
