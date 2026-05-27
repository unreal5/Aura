// 版权没有，拷贝自由。


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Tag/AuraGlobalTags.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnAttributeChangedWithTag.Broadcast(AuraAttributeTags::Health, AS->GetHealth());
	OnAttributeChangedWithTag.Broadcast(AuraAttributeTags::MaxHealth, AS->GetMaxHealth());
	OnAttributeChangedWithTag.Broadcast(AuraAttributeTags::Mana, AS->GetMana());
	OnAttributeChangedWithTag.Broadcast(AuraAttributeTags::MaxMana, AS->GetMaxMana());
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
	HealthDelegate.AddLambda(AttributeChangeLambda, AuraAttributeTags::Health.GetTag());

	// 绑定MaxHealth
	FOnGameplayAttributeValueChange& MaxHealthDelegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxHealthAttribute());
	MaxHealthDelegate.AddLambda(AttributeChangeLambda, AuraAttributeTags::MaxHealth.GetTag());

	// 绑定Mana
	FOnGameplayAttributeValueChange& ManaDelegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetManaAttribute());
	ManaDelegate.AddLambda(AttributeChangeLambda, AuraAttributeTags::Mana.GetTag());

	// 绑定MaxMana
	FOnGameplayAttributeValueChange MaxManaDelegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxManaAttribute());
	MaxManaDelegate.AddLambda(AttributeChangeLambda, AuraAttributeTags::MaxMana.GetTag());
}
