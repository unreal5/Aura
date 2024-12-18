// 版本没有，拷贝自由。


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AttributeSet.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Super::BroadcastInitialValues();
	auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	// 侦听属性变化

	// Health
	auto LambdaOnHealthChanged = [this](const FOnAttributeChangeData& Data)
	{
		// auto BaseValue = AuraAttributeSet->Health.GetBaseValue();
		// auto CurrentValue = AuraAttributeSet->Health.GetCurrentValue();
		//
		// UE_LOG(LogTemp, Warning, TEXT("BaseValue = %f, CurrentValue = %f, Data.NewValue = %f"), BaseValue, CurrentValue, Data.NewValue);
		OnHealthChanged.Broadcast(Data.NewValue);

		//OnHealthChanged.Broadcast(CurrentValue);
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).
	                        AddLambda(LambdaOnHealthChanged);

	// MaxHealth
	auto LambdaOnMaxHealthChanged = [this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda(LambdaOnMaxHealthChanged);

	// Mana
	auto LambdaOnManaChanged = [this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
		//OnManaChanged.Broadcast(AuraAttributeSet->Mana.GetCurrentValue());
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
	                      .AddLambda(LambdaOnManaChanged);

	// MaxMana
	auto LambdaOnMaxManaChanged = [this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
	                      .AddLambda(LambdaOnMaxManaChanged);

	auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	check(AuraASC);
	AuraASC->OnEffectAssetTagsDelegate.AddLambda([this](const FGameplayTagContainer& Tags)
	{
		check(MessageWidgetDataTable);
		//现在只对MessageTag感兴趣
		const auto MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
		for (auto Tag : Tags)
		{
			if (false == Tag.MatchesTag(MessageTag)) continue;

			if (const auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Row->Message = %s"), *Row->Message.ToString());
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
		}
	});
}
