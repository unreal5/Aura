// 版权没有，拷贝自由。


#include "Animation/AnimNotify/AnimNotify_MontageEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"

UAnimNotify_MontageEvent::UAnimNotify_MontageEvent()
{
	NotifyColor = FColor::Green;
}

void UAnimNotify_MontageEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const auto Owner = MeshComp->GetOwner())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, FGameplayEventData());
	}
}

FString UAnimNotify_MontageEvent::GetNotifyName_Implementation() const
{
	return EventTag.GetTagLeafName().ToString();
}
