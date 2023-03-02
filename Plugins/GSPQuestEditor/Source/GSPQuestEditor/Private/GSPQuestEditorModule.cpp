// Copyright Epic Games, Inc. All Rights Reserved.

#include "GSPQuestEditorModule.h"
#include "GSPQuestEditorEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "GSPQuestEditorModule"

void FGSPQuestEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FGSPQuestEditorEditorModeCommands::Register();
}

void FGSPQuestEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FGSPQuestEditorEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGSPQuestEditorModule, GSPQuestEditorEditorMode)