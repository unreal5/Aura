// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Attack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AURA_API UBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_Attack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="技能标记")
	FBlackboardKeySelector CombatTargetSelector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="技能标记")
	FGameplayTag MeleeAttackTag;
};