// 版本没有，拷贝自由。


#include "AI/EQS/EnvQueryContext_PlayerContext.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Kismet/GameplayStatics.h"

void UEnvQueryContext_PlayerContext::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                    FEnvQueryContextData& ContextData) const
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(this, PlayerClass, Players);
	Players.Remove(nullptr);
	if (Players.Num())
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, Players);
	}
}