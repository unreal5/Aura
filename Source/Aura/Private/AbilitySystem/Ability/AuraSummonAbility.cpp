// 版本没有，拷贝自由。


#include "AbilitySystem/Ability/AuraSummonAbility.h"

#include "NavigationSystem.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return {};
	}

	const auto Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		return {};
	}
	const auto Forward = Avatar->GetActorForwardVector();
	const auto Location = Avatar->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const auto LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	int32 i = 0;
	int32 EmergencyBreak = 0;
	const int32 MaxEmergencyBreak = SpawnSpread * 2;
	while (i < NumMinions)
	{
		if (EmergencyBreak++ > MaxEmergencyBreak)
		{
			UE_LOG(LogTemp, Error, TEXT("寻路过程失败，无法生成所有的召唤物"));
			break;
		}
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		const FVector ChosenSpawnLocation = Location + Direction *
			FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		FNavLocation NavLocation;
		if (!NavSys->ProjectPointToNavigation(ChosenSpawnLocation, NavLocation))
		{
			continue;
		}
		SpawnLocations.Add(NavLocation.Location);
		++i;
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass() const
{
	if(MinionClasses.Num() == 0)
	{
		checkf(false, TEXT("在编辑器中设置召唤物类"));
		return {};
	}
	int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];	
}