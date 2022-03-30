// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class FlightAttendantSim : ModuleRules
{
	public FlightAttendantSim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"AIModule",
			"UMG",
			"DlgSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
		{
			"FlightAttendantSim/Public/Components", 
			"FlightAttendantSim/Public/Quests",
			"FlightAttendantSim/Public/Quests/Transitions",
			"FlightAttendantSim/Public/Quests/TransitionEvents",
			"FlightAttendantSim/Public/WorldMap",
			"FlightAttendantSim/Public/WorldMap/TreeLayoutDrawing",
			"FlightAttendantSim/Public/Characters",
			"FlightAttendantSim/Public/Characters/Actions",
			"FlightAttendantSim/Public/Characters/Passengers",
			"FlightAttendantSim/Public/Characters/Workers",
			"FlightAttendantSim/Public/Characters/Workers/Events",
			"FlightAttendantSim/Public/SpacePlane",
			"FlightAttendantSim/Public/CargoDelivery",
			"FlightAttendantSim/Public/Flight"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
