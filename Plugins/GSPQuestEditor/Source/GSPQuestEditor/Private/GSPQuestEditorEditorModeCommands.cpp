// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPQuestEditorEditorModeCommands.h"
#include "GSPQuestEditorEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "GSPQuestEditorEditorModeCommands"

FGSPQuestEditorEditorModeCommands::FGSPQuestEditorEditorModeCommands()
	: TCommands<FGSPQuestEditorEditorModeCommands>("GSPQuestEditorEditorMode",
		NSLOCTEXT("GSPQuestEditorEditorMode", "GSPQuestEditorEditorModeCommands", "GSPQuestEditor Editor Mode"),
		NAME_None,
		FEditorStyle::GetStyleSetName())
{
}

void FGSPQuestEditorEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Quest Creator", "Tool used for creating quests", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);


}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FGSPQuestEditorEditorModeCommands::GetCommands()
{
	return FGSPQuestEditorEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
