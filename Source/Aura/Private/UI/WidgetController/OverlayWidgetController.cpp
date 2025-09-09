// 版权没有，拷贝自由。


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	if (auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
		OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	}
}

using FDelegatePtr = FOnDataChangedDelegate UOverlayWidgetController::*;

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	check(AbilitySystemComponent);
	// 定义属性和委托成员指针的数组，必须定义为局部变量，以便正确的保证初始化顺序。
	static const FDelegatePtr DelegateArray[] = {
		&UOverlayWidgetController::OnHealthChanged,
		&UOverlayWidgetController::OnMaxHealthChanged,
		&UOverlayWidgetController::OnManaChanged,
		&UOverlayWidgetController::OnMaxManaChanged
	};

	static const FGameplayAttribute AttributeArray[] = {
		UAuraAttributeSet::GetHealthAttribute(),
		UAuraAttributeSet::GetMaxHealthAttribute(),
		UAuraAttributeSet::GetManaAttribute(),
		UAuraAttributeSet::GetMaxManaAttribute()
	};

	if (Cast<UAuraAttributeSet>(AttributeSet) == nullptr) return;


	// 合并lambda
	for (int32 i = 0; i < UE_ARRAY_COUNT(AttributeArray); ++i)
	{
		auto& Delegate = this->*DelegateArray[i];
		auto&& AttributeChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AttributeArray[i]);
		AttributeChangedDelegate.AddLambda([&Delegate](const FOnAttributeChangeData& Data)
		{
			Delegate.Broadcast(Data.NewValue);
		});
	}
	// 监听效果标签变化

	if (auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->EffectAssetTags.AddLambda([this](const FGameplayTagContainer& AssetTags)
		{
			const auto ParentTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			for (const auto& Tag : AssetTags)
			{
				if (!Tag.MatchesTag(ParentTag)) continue;

				auto UIWidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				MessageWidgetRowDelegate.Broadcast(*UIWidgetRow);
			}
		});
	}
}


#pragma region OldCode
/*  UOverlayWidgetController::BindCallbacksToDependencies()的冗余代码。
 *	auto&& OnHealthChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute());
	OnHealthChangedDelegate.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	auto&& OnMaxHealthChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute());
	OnMaxHealthChangedDelegate.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	auto&& OnManaChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute());
	OnManaChangedDelegate.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	auto&& OnMaxManaChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute());
	OnMaxManaChangedDelegate.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});
*/
#pragma endregion
