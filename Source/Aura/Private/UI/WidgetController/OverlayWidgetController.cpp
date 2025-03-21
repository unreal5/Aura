// 版权没有，拷贝自由。


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	auto AS = Cast<UAuraAttributeSet>(AttributeSet.Get());
	if (!AS) return;

	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxManaChanged.Broadcast(AS->GetMaxMana());
	OnManaChanged.Broadcast(AS->GetMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent) return;

	auto MaxHealthChanged = [this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxHealthAttribute()).
	                        AddLambda(MaxHealthChanged);

	auto HealthChanged = [this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetHealthAttribute()).
	                        AddLambda(HealthChanged);

	auto MaxManaChanged = [this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxManaAttribute()).
	                        AddLambda(MaxManaChanged);

	auto ManaChanged = [this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetManaAttribute()).
	                        AddLambda(ManaChanged);


	// 从消息表中查找资产Tag对应的消息
	if (!MessageWidgetDataTable) return;

	if (auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->EffectAssetTags.AddLambda([this](const FGameplayTagContainer& AssetTags)
		{
			auto MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			for (auto tag : AssetTags)
			{
				if (!tag.MatchesTag(MessageTag)) continue;

				auto FoundRow = GetDataTableRowByTag<FUiWidgetRow>(MessageWidgetDataTable.Get(), tag);
				if (FoundRow == nullptr) continue;

				MessageWidgetRowDelegate.Broadcast(*FoundRow);
			}
		});
	}
}
