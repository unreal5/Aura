// 版本没有，拷贝自由。


#include "AI/BTTaskNode_Attack.h"

#include "AbilitySystemComponent.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AuraEnemy.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
	CombatTargetSelector.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_Attack, CombatTargetSelector),
	                                     AActor::StaticClass());
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	if (MeleeAttackTag.IsValid())
	{
		TagContainer.AddTag(MeleeAttackTag);
	}

	// 选择
	AActor* CombatTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(CombatTargetSelector.SelectedKeyName));
	//if(CombatTarget->Implements<>())
	if(IsValid(CombatTarget))
	{
		//AuraEnemy->SetCombatTarget(CombatTarget);
		IEnemyInterface::Execute_SetCombatTarget(AuraEnemy, CombatTarget);
		AuraEnemy->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
		// Super类默认返回成功
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

FString UBTTaskNode_Attack::GetStaticDescription() const
{
	FString Desc = FString::Printf(TEXT("Attack %s"), *CombatTargetSelector.SelectedKeyName.ToString());
	return Desc;
}