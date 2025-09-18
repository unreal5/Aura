// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Tag/GlobalTag.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	TagsToAttributesMap.Add(GlobalTag::Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Primary_Vigor, GetVigorAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributesMap.Add(GlobalTag::Attributes_Secondary_MaxMana, GetMaxManaAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vigor, COND_None, REPNOTIFY_Always);
	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);
	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 这个函数对”当前值“的修改进行限制
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Super::PostGameplayEffectExecute(Data);
	// Source == causer of the effect, Target == target of the effect(owner of this AS)
	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	if (GetHealthAttribute() == Data.EvaluatedData.Attribute)
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (GetManaAttribute() == Data.EvaluatedData.Attribute)
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

// Primary RepNotify functions
void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Vigor, OldVigor);
}

// Vital RepNotify functions
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// This function exists to make sure that the OnAttributeChange delegate is fired when the attribute is updated from a server replication.
	// This is important for cases where the attribute change is not caused by a gameplay effect, but rather by a direct replication from the server.
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldMana);
}

// Secondary RepNotify functions
void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
                                            FEffectProperties& EffectProperties) const
{
	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();
	EffectProperties.SourceASC = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(EffectProperties.SourceASC) && EffectProperties.SourceASC->AbilityActorInfo.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->GetAvatarActor();
		EffectProperties.SourcePlayerController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (EffectProperties.SourcePlayerController == nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourcePlayerController = Pawn->GetController<APlayerController>();
			}
		}
		EffectProperties.SourceCharacter = nullptr;
		if (EffectProperties.SourcePlayerController)
		{
			EffectProperties.SourceCharacter = EffectProperties.SourcePlayerController->GetPawn<ACharacter>();
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.GetAvatarActor())
	{
		EffectProperties.TargetAvatarActor = Data.Target.GetAvatarActor();
		EffectProperties.TargetPlayerController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);
		EffectProperties.TargetASC = &Data.Target;
	}
}
