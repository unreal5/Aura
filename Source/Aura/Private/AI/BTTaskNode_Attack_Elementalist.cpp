// 版本没有，拷贝自由。


#include "AI/BTTaskNode_Attack_Elementalist.h"

#include "AbilitySystemComponent.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AuraEnemy.h"

UBTTaskNode_Attack_Elementalist::UBTTaskNode_Attack_Elementalist()
{
	CombatTargetSelector.AddObjectFilter(
		this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_Attack_Elementalist, CombatTargetSelector),
		AActor::StaticClass());
}

EBTNodeResult::Type UBTTaskNode_Attack_Elementalist::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//...
	//Get Controlled Pawn
	AAuraAIController* AIController = Cast<AAuraAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AAuraEnemy* AuraEnemy = Cast<AAuraEnemy>(AIController->GetPawn());
	if (!AuraEnemy)
	{
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	// TODO: 测试近战攻击
	FGameplayTagContainer TagContainer;
	/*if (AttackTag.IsValid())
	{
		TagContainer.AddTag(AttackTag);
	}
	*/

	// 选择
	AActor* CombatTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(CombatTargetSelector.SelectedKeyName));
	//if(CombatTarget->Implements<>())
	if (IsValid(CombatTarget))
	{
		//AuraEnemy->SetCombatTarget(CombatTarget);
		IEnemyInterface::Execute_SetCombatTarget(AuraEnemy, CombatTarget);
		//
		int32 MinionCount = ICombatInterface::Execute_GetMinionCount(AuraEnemy);
		if (MinionCount < MinionSpawnThreshold)
		{
			AbilityTag = SummonTag;
		}
		else
		{
			AbilityTag = AttackTag;
		}
		TagContainer.AddTag(AbilityTag);
		AuraEnemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
		// Super类默认返回成功
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

FString UBTTaskNode_Attack_Elementalist::GetStaticDescription() const
{
	FString Desc = FString::Printf(TEXT("范围攻击 %s"), *CombatTargetSelector.SelectedKeyName.ToString());
	return Desc;
}