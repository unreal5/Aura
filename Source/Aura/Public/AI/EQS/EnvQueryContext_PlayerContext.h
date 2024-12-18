// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_PlayerContext.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AURA_API UEnvQueryContext_PlayerContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Context")
	TSubclassOf<AActor> PlayerClass;
};