// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_GoAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTTaskNode_GoAroundTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_GoAroundTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务参数")
	FBlackboardKeySelector NewLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务参数")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务参数")
	float Radius = 300.0f;
};