// 版本没有，拷贝自由。


#include "AI/BTTaskNode_GoAroundTarget.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_GoAroundTarget::UBTTaskNode_GoAroundTarget()
{
	// 限制类型
	NewLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_GoAroundTarget, NewLocationKey));
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_GoAroundTarget, TargetKey),
	                          AActor::StaticClass());
}

EBTNodeResult::Type UBTTaskNode_GoAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	auto BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}
	// UBTFunctionLibrary::GetBlackboardValueAsActor(UBTNode* NodeOwner, const FBlackboardKeySelector& Key)
	// 实现了以下功能，这里不使用UBTFunctionLibrary，避免引入不必要的依赖。
	AActor* TargetAcotr = Cast<AActor>(BB->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetAcotr)
	{
		return EBTNodeResult::Failed;
	}

	auto TargetActorLocation =TargetAcotr->GetActorLocation();
	//Get Navigation System
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}
	//Perform Query
	FNavLocation Result;
	bool bSuccess = NavSys->GetRandomReachablePointInRadius(TargetActorLocation, Radius, Result);
	if (!bSuccess) {
		return EBTNodeResult::Failed;
	}
	BB->SetValueAsVector(NewLocationKey.SelectedKeyName, Result.Location);
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

FString UBTTaskNode_GoAroundTarget::GetStaticDescription() const
{
	return TEXT("在目标周围随机移动");
}