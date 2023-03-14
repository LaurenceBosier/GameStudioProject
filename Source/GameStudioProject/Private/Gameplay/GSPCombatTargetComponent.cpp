// Game Studio Project Team F 2023 - Laurence Bosier


#include "Gameplay/GSPCombatTargetComponent.h"


UGSPCombatTargetComponent::UGSPCombatTargetComponent()
{
	//Hide the target mesh until locked on to
	bHiddenInGame = true;
}

void UGSPCombatTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	//Try attach the mesh to the correct socket
	if(bAutoAttachToSkeletalMesh)
	{
		if(!LockOnMarkerSocketOrBoneName.IsNone())
		{
			AttemptToPopulateStaticMeshBones();

			if(FoundSkeletalMesh && StaticMeshBones.Contains(LockOnMarkerSocketOrBoneName))
			{
				const FAttachmentTransformRules ARules {
					EAttachmentRule::SnapToTarget,
					EAttachmentRule::SnapToTarget,
					EAttachmentRule::KeepRelative,
					false};

				if(!AttachToComponent(FoundSkeletalMesh, ARules, LockOnMarkerSocketOrBoneName))
				{
					UE_LOG(LogTemp, Error, TEXT("Faild to connect target component to bone: %s"), *LockOnMarkerSocketOrBoneName.ToString());
				}
			}
		}
		else
		{
			//No bone specified
			UE_LOG(LogTemp, Error, TEXT("No bone name spcified for traget component attachment"));
		}
		StaticMeshBones.Empty();
		return;
	}


	//If static attachment 
}

void UGSPCombatTargetComponent::AttemptToPopulateStaticMeshBones()
{
	if(IsValid(GetOwner()))
	{
		FoundSkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

		if(IsValid(FoundSkeletalMesh))
		{
			FoundSkeletalMesh->GetBoneNames(StaticMeshBones);
			StaticMeshBones.Append(FoundSkeletalMesh->GetAllSocketNames());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No skeletal mesh found on the owned actor!"));
			StaticMeshBones.Empty();
		}
	}
}


