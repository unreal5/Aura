// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MontageEvent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAnimNotify_MontageEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_MontageEvent();
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "蒙太奇事件")
	FGameplayTag EventTag;
};
