// 版权没有，拷贝自由。


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Tag/AuraGlobalTags.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnAttributeChangedWithTag.Broadcast(Attributes::Vital::Health, AS->GetHealth());
	OnAttributeChangedWithTag.Broadcast(Attributes::Vital::MaxHealth, AS->GetMaxHealth());
	OnAttributeChangedWithTag.Broadcast(Attributes::Vital::Mana, AS->GetMana());
	OnAttributeChangedWithTag.Broadcast(Attributes::Vital::MaxMana, AS->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//Super::BindCallbacksToDependencies();


	auto AttributeChangeLambda = [this](const FOnAttributeChangeData& Data, const FGameplayTag& AttributeTag)
	{
		OnAttributeChangedWithTag.Broadcast(AttributeTag, Data.NewValue);
	};

	FOnGameplayAttributeValueChange& HealthDelegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetHealthAttribute());
	HealthDelegate.AddLambda(AttributeChangeLambda, Attributes::Vital::Health.GetTag());

	// 绑定MaxHealth
	FOnGameplayAttributeValueChange& MaxHealthDelegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxHealthAttribute());
	MaxHealthDelegate.AddLambda(AttributeChangeLambda, Attributes::Vital::MaxHealth.GetTag());

	// 绑定Mana
	FOnGameplayAttributeValueChange& ManaDelegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetManaAttribute());
	ManaDelegate.AddLambda(AttributeChangeLambda, Attributes::Vital::Mana.GetTag());

	// 绑定MaxMana
	FOnGameplayAttributeValueChange MaxManaDelegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxManaAttribute());
	MaxManaDelegate.AddLambda(AttributeChangeLambda, Attributes::Vital::MaxMana.GetTag());


	// 查找信息表
	auto AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!IsValid(AuraASC)) return;

	if (!IsValid(MessageWidgetDataTable)) return;

	auto AssetTagsLambda = [this](const FGameplayTagContainer& AssetTags)
	{
		for (const auto& Tag : AssetTags)
		{
			const auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			if (!Row) continue;

			UE_LOG(LogTemp, Warning, TEXT("查找到数据：%s, 消息：%s"), *Row->MessageTag.ToString(), *Row->Message.ToString());
		}
	};
	AuraASC->OnEffectAssetTagsAppliedDelegate.AddLambda(AssetTagsLambda);
}
