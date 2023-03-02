// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPQuestEditorEditorModeToolkit.h"
#include "GSPQuestEditorEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "GSPQuestEditorEditorModeToolkit"

FGSPQuestEditorEditorModeToolkit::FGSPQuestEditorEditorModeToolkit()
{
}

void FGSPQuestEditorEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FGSPQuestEditorEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FGSPQuestEditorEditorModeToolkit::GetToolkitFName() const
{
	return FName("GSPQuestEditorEditorMode");
}

FText FGSPQuestEditorEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "GSPQuestEditorEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
