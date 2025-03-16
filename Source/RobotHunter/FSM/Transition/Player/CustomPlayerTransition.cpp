#include "CustomPlayerTransition.h"
#include "RobotHunter/Player/CustomPlayer.h"

UCustomPlayerTransition::UCustomPlayerTransition()
{
	toCheck = TMap<TEnumAsByte<EPlayerBool>, bool>();

	player = nullptr;
}


void UCustomPlayerTransition::RetrievePlayer()
{
	if (!player)
	{
		if (fsm)
			player = CAST(ACustomPlayer, fsm->GetOwner());
	}
}


bool UCustomPlayerTransition::CheckBool()
{
	if (player)
	{
		for (TPair<EPlayerBool, bool> _bool : toCheck)
		{
			if (player->GetFSMBool(_bool.Key) != _bool.Value)
				return false;
		}

		return true;
	}

	return false;
}


void UCustomPlayerTransition::InitTransition()
{
	RetrievePlayer();
}

bool UCustomPlayerTransition::IsValidTransition()
{
	return CheckBool();
}
