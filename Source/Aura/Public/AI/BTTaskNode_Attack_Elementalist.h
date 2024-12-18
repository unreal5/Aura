// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Attack_Elementalist.generated.h"

/**
 * 如果minions小于某个数值，则使用召唤技能，否则使用火球技能
 */
UCLASS(Blueprintable)
class AURA_API UBTTaskNode_Attack_Elementalist : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_Attack_Elementalist();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="技能|标记")
	FBlackboardKeySelector CombatTargetSelector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="技能|标记|远程攻击标记")
	FGameplayTag AttackTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="技能|标记|召唤标记")
	FGameplayTag SummonTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="技能|标记")
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="技能|参数")
	float MinionSpawnThreshold = 2.f;
};