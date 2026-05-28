// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DebugPanelWidget.generated.h"

class UAuraAttributeSet;
class UAbilitySystemComponent;
class UBorder;
class UCanvasPanel;
class UScrollBox;
class UTextBlock;
class UVerticalBox;

UCLASS()
class AURA_API UDebugPanelWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient)
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UBorder> PanelBorder;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UScrollBox> DebugScrollBox;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UVerticalBox> DebugContentBox;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> AttributeHeaderText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> ManaText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> TagHeaderText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> TagValuesText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> EffectHeaderText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> EffectValuesText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> AbilityHeaderText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> AbilityValuesText;

	UPROPERTY(Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> HintText;
};
