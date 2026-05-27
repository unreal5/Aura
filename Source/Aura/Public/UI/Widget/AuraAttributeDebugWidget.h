#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "AuraAttributeDebugWidget.generated.h"

class UAuraAttributeSet;
class UAbilitySystemComponent;
class UBorder;
class UCanvasPanel;
class UScrollBox;
class UTextBlock;
class UVerticalBox;

UCLASS()
class AURA_API UAuraAttributeDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeWithAbilitySystem(UAbilitySystemComponent* InAbilitySystemComponent, UAuraAttributeSet* InAttributeSet);
	void TogglePanelVisibility();
	void SetPanelVisible(bool bVisible);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

private:
	void BuildWidgetTree();
	void BindAttributeDelegates();
	void UnbindAttributeDelegates();
	void RefreshValues();
	void RefreshOwnedTags();
	void RefreshActiveGameplayEffects();
	void RefreshActivatableAbilities();
	void UpdateDisplayedValues() const;
	static FString FormatTagContainer(const FGameplayTagContainer& TagContainer);
	static FString FormatDuration(float Duration);

	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
	void HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	void HandleManaChanged(const FOnAttributeChangeData& ChangeData);
	void HandleMaxManaChanged(const FOnAttributeChangeData& ChangeData);

	static void ConfigureTextBlock(UTextBlock* TextBlock, int32 FontSize, const FLinearColor& Color, bool bBold = false);

	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient)
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	UPROPERTY(Transient)
	TObjectPtr<UBorder> PanelBorder;

	UPROPERTY(Transient)
	TObjectPtr<UScrollBox> DebugScrollBox;

	UPROPERTY(Transient)
	TObjectPtr<UVerticalBox> DebugContentBox;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> AttributeHeaderText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> ManaText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> TagHeaderText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> TagValuesText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> EffectHeaderText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> EffectValuesText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> AbilityHeaderText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> AbilityValuesText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> HintText;

	FDelegateHandle HealthChangedHandle;
	FDelegateHandle MaxHealthChangedHandle;
	FDelegateHandle ManaChangedHandle;
	FDelegateHandle MaxManaChangedHandle;

	float CachedHealth = 0.f;
	float CachedMaxHealth = 0.f;
	float CachedMana = 0.f;
	float CachedMaxMana = 0.f;
	FString CachedOwnedTagsText;
	FString CachedActiveEffectsText;
	FString CachedAbilitiesText;
	float RefreshIntervalSeconds = 0.25f;
	float TimeSinceLastRefresh = 0.f;
};
