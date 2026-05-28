#include "UI/Widget/AuraAttributeDebugWidget.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Styling/CoreStyle.h"
#include "Blueprint/WidgetTree.h"

void UAuraAttributeDebugWidget::InitializeWithAbilitySystem(UAbilitySystemComponent* InAbilitySystemComponent,
                                                            UAuraAttributeSet* InAttributeSet)
{
	if (!IsValid(InAbilitySystemComponent) || !IsValid(InAttributeSet)) return;

	UAbilitySystemComponent* PreviousAbilitySystemComponent = AbilitySystemComponent;
	const bool bAbilitySystemChanged = PreviousAbilitySystemComponent != InAbilitySystemComponent;
	if (bAbilitySystemChanged && IsValid(PreviousAbilitySystemComponent))
	{
		AbilitySystemComponent = PreviousAbilitySystemComponent;
		UnbindAttributeDelegates();
	}

	AbilitySystemComponent = InAbilitySystemComponent;
	AuraAttributeSet = InAttributeSet;

	if (bAbilitySystemChanged)
	{
		BindAttributeDelegates();
	}

	RefreshValues();
	SetPanelVisible(false);
}

void UAuraAttributeDebugWidget::TogglePanelVisibility()
{
	SetPanelVisible(!IsVisible());
}

void UAuraAttributeDebugWidget::SetPanelVisible(const bool bVisible)
{
	if (bVisible)
	{
		TimeSinceLastRefresh = 0.f;
		RefreshValues();
	}

	SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UAuraAttributeDebugWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BuildWidgetTree();
	SetPanelVisible(false);
	UpdateDisplayedValues();
}

void UAuraAttributeDebugWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsVisible() || !IsValid(AbilitySystemComponent)) return;

	TimeSinceLastRefresh += InDeltaTime;
	if (TimeSinceLastRefresh < RefreshIntervalSeconds) return;

	TimeSinceLastRefresh = 0.f;
	RefreshValues();
}

void UAuraAttributeDebugWidget::NativeDestruct()
{
	UnbindAttributeDelegates();
	Super::NativeDestruct();
}

void UAuraAttributeDebugWidget::BuildWidgetTree()
{
	if (!WidgetTree || WidgetTree->RootWidget) return;

	UCanvasPanel* RootPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootPanel"));
	WidgetTree->RootWidget = RootPanel;

	PanelBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("PanelBorder"));
	PanelBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.72f));
	PanelBorder->SetPadding(FMargin(10.f));

	if (UCanvasPanelSlot* BorderSlot = RootPanel->AddChildToCanvas(PanelBorder))
	{
		BorderSlot->SetAutoSize(false);
		BorderSlot->SetPosition(FVector2D(0.f, 0.f));
		BorderSlot->SetSize(FVector2D(960.f, 800.f));
	}

	DebugScrollBox = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("DebugScrollBox"));
	PanelBorder->SetContent(DebugScrollBox);

	DebugContentBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("DebugContentBox"));
	DebugScrollBox->AddChild(DebugContentBox);

	auto AddTextToPanel = [this](const TCHAR* WidgetName, const int32 FontSize, const FLinearColor& Color,
	                             const bool bBold, const FMargin& SlotPadding) -> UTextBlock*
	{
		UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), WidgetName);
		ConfigureTextBlock(TextBlock, FontSize, Color, bBold);
		if (UVerticalBoxSlot* Slot = DebugContentBox->AddChildToVerticalBox(TextBlock))
		{
			Slot->SetPadding(SlotPadding);
		}
		return TextBlock;
	};

	TitleText = AddTextToPanel(TEXT("TitleText"), 28, FLinearColor(1.f, 0.85f, 0.2f), true,
	                           FMargin(12.f, 10.f, 12.f, 10.f));
	TitleText->SetText(FText::FromString(TEXT("GAS 属性调试面板")));

	AttributeHeaderText = AddTextToPanel(TEXT("AttributeHeaderText"), 18, FLinearColor(0.95f, 0.65f, 0.2f), true,
	                                     FMargin(12.f, 6.f, 12.f, 6.f));
	AttributeHeaderText->SetText(FText::FromString(TEXT("[Attributes]")));

	HealthText = AddTextToPanel(TEXT("HealthText"), 18, FLinearColor(0.75f, 1.f, 0.75f), false,
	                            FMargin(24.f, 2.f, 12.f, 2.f));
	ManaText = AddTextToPanel(TEXT("ManaText"), 18, FLinearColor(0.65f, 0.85f, 1.f), false,
	                          FMargin(24.f, 2.f, 12.f, 8.f));

	TagHeaderText = AddTextToPanel(TEXT("TagHeaderText"), 18, FLinearColor(0.55f, 0.85f, 1.f), true,
	                               FMargin(12.f, 6.f, 12.f, 6.f));
	TagHeaderText->SetText(FText::FromString(TEXT("[Owned Gameplay Tags]")));
	TagValuesText = AddTextToPanel(TEXT("TagValuesText"), 18, FLinearColor(0.88f, 0.92f, 1.f), false,
	                               FMargin(24.f, 2.f, 12.f, 8.f));

	EffectHeaderText = AddTextToPanel(TEXT("EffectHeaderText"), 18, FLinearColor(1.f, 0.72f, 0.72f), true,
	                                  FMargin(12.f, 6.f, 12.f, 6.f));
	EffectHeaderText->SetText(FText::FromString(TEXT("[Active Gameplay Effects]")));
	EffectValuesText = AddTextToPanel(TEXT("EffectValuesText"), 12, FLinearColor(1.f, 0.0f, 0.9f), false,
	                                  FMargin(24.f, 2.f, 12.f, 8.f));

	AbilityHeaderText = AddTextToPanel(TEXT("AbilityHeaderText"), 18, FLinearColor(0.85f, 0.7f, 1.f), true,
	                                   FMargin(12.f, 6.f, 12.f, 6.f));
	AbilityHeaderText->SetText(FText::FromString(TEXT("[Activatable Abilities]")));
	AbilityValuesText = AddTextToPanel(TEXT("AbilityValuesText"), 18, FLinearColor(0.94f, 0.88f, 1.f), false,
	                                   FMargin(24.f, 2.f, 12.f, 8.f));

	HintText = AddTextToPanel(TEXT("HintText"), 18, FLinearColor(0.85f, 0.85f, 0.85f), false,
	                          FMargin(12.f, 12.f, 12.f, 14.f));
	HintText->SetText(FText::FromString(TEXT("F12: 显示/隐藏面板")));
}

void UAuraAttributeDebugWidget::BindAttributeDelegates()
{
	if (!IsValid(AbilitySystemComponent)) return;


	HealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UAuraAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	MaxHealthChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UAuraAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	ManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UAuraAttributeSet::GetManaAttribute()).AddUObject(this, &ThisClass::HandleManaChanged);
	MaxManaChangedHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UAuraAttributeSet::GetMaxManaAttribute()).AddUObject(this, &ThisClass::HandleMaxManaChanged);
}

void UAuraAttributeDebugWidget::UnbindAttributeDelegates()
{
	if (!IsValid(AbilitySystemComponent))
	{
		HealthChangedHandle.Reset();
		MaxHealthChangedHandle.Reset();
		ManaChangedHandle.Reset();
		MaxManaChangedHandle.Reset();
		return;
	}

	if (HealthChangedHandle.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetHealthAttribute()).Remove(
			HealthChangedHandle);
		HealthChangedHandle.Reset();
	}

	if (MaxHealthChangedHandle.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxHealthAttribute()).
		                        Remove(
			                        MaxHealthChangedHandle);
		MaxHealthChangedHandle.Reset();
	}

	if (ManaChangedHandle.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetManaAttribute()).Remove(
			ManaChangedHandle);
		ManaChangedHandle.Reset();
	}

	if (MaxManaChangedHandle.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxManaAttribute()).
		                        Remove(
			                        MaxManaChangedHandle);
		MaxManaChangedHandle.Reset();
	}
}

void UAuraAttributeDebugWidget::RefreshValues()
{
	if (!IsValid(AbilitySystemComponent))
	{
		CachedOwnedTagsText = TEXT("<未绑定 AbilitySystemComponent>");
		CachedActiveEffectsText = TEXT("<未绑定 AbilitySystemComponent>");
		CachedAbilitiesText = TEXT("<未绑定 AbilitySystemComponent>");
		UpdateDisplayedValues();
		return;
	}

	CachedHealth = AbilitySystemComponent->GetNumericAttribute(UAuraAttributeSet::GetHealthAttribute());
	CachedMaxHealth = AbilitySystemComponent->GetNumericAttribute(UAuraAttributeSet::GetMaxHealthAttribute());
	CachedMana = AbilitySystemComponent->GetNumericAttribute(UAuraAttributeSet::GetManaAttribute());
	CachedMaxMana = AbilitySystemComponent->GetNumericAttribute(UAuraAttributeSet::GetMaxManaAttribute());
	RefreshOwnedTags();
	RefreshActiveGameplayEffects();
	RefreshActivatableAbilities();

	UpdateDisplayedValues();
}

void UAuraAttributeDebugWidget::RefreshOwnedTags()
{
	FGameplayTagContainer OwnedTags;
	AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);
	CachedOwnedTagsText = FormatTagContainer(OwnedTags);
}

void UAuraAttributeDebugWidget::RefreshActiveGameplayEffects()
{
	TArray<FString> EffectLines;
	const TArray<FActiveGameplayEffectHandle> ActiveEffectHandles = AbilitySystemComponent->GetActiveEffects(
		FGameplayEffectQuery());
	const UWorld* World = GetWorld();
	const float WorldTime = IsValid(World) ? World->GetTimeSeconds() : 0.f;

	for (const FActiveGameplayEffectHandle& ActiveEffectHandle : ActiveEffectHandles)
	{
		const FActiveGameplayEffect* ActiveEffect = AbilitySystemComponent->GetActiveGameplayEffect(ActiveEffectHandle);
		if (!ActiveEffect)
		{
			continue;
		}

		const FGameplayEffectSpec& Spec = ActiveEffect->Spec;
		const UGameplayEffect* EffectDef = AbilitySystemComponent->GetGameplayEffectDefForHandle(ActiveEffectHandle);
		const FString EffectName = GetNameSafe(EffectDef);
		const int32 StackCount = AbilitySystemComponent->GetCurrentStackCount(ActiveEffectHandle);
		const float Duration = Spec.GetDuration();
		const float RemainingTime = IsValid(World) ? ActiveEffect->GetTimeRemaining(WorldTime) : Duration;
		const float Period = Spec.GetPeriod();

		FGameplayTagContainer GrantedTags;
		Spec.GetAllGrantedTags(GrantedTags);

		FString EffectLine = FString::Printf(TEXT("%s | Stacks:%d | Remaining:%s | Duration:%s | Period:%s"),
		                                     *EffectName,
		                                     StackCount,
		                                     *FormatDuration(RemainingTime),
		                                     *FormatDuration(Duration),
		                                     Period <= 0.f ? TEXT("None") : *FString::Printf(TEXT("%.2fs"), Period));

		if (!GrantedTags.IsEmpty())
		{
			EffectLine += FString::Printf(TEXT("\n    Tags: %s"), *GrantedTags.ToStringSimple());
		}

		EffectLines.Add(MoveTemp(EffectLine));
	}

	if (EffectLines.IsEmpty())
	{
		CachedActiveEffectsText = TEXT("<无 Active Gameplay Effect>");
		return;
	}

	EffectLines.Sort();
	CachedActiveEffectsText = FString::Join(EffectLines, TEXT("\n\n"));
}

void UAuraAttributeDebugWidget::RefreshActivatableAbilities()
{
	TArray<FString> AbilityLines;
	for (const FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		const FString AbilityName = GetNameSafe(AbilitySpec.Ability);
		const FString InputText = AbilitySpec.InputID == INDEX_NONE ? TEXT("-") : FString::FromInt(AbilitySpec.InputID);
		const FString ActiveText = AbilitySpec.IsActive() ? TEXT("Active") : TEXT("Idle");
		AbilityLines.Add(FString::Printf(TEXT("%s | Lv:%d | Input:%s | %s"),
		                                 *AbilityName,
		                                 AbilitySpec.Level,
		                                 *InputText,
		                                 *ActiveText));
	}

	if (AbilityLines.IsEmpty())
	{
		CachedAbilitiesText = TEXT("<无 Activatable Ability>");
		return;
	}

	AbilityLines.Sort();
	CachedAbilitiesText = FString::Join(AbilityLines, TEXT("\n"));
}

void UAuraAttributeDebugWidget::UpdateDisplayedValues() const
{
	if (TitleText)
	{
		TitleText->SetText(FText::FromString(TEXT("GAS 综合调试面板")));
	}

	if (AttributeHeaderText)
	{
		AttributeHeaderText->SetText(FText::FromString(TEXT("[Attributes]")));
	}

	if (HealthText)
	{
		HealthText->SetText(
			FText::FromString(FString::Printf(TEXT("Health    %7.1f / %7.1f"), CachedHealth, CachedMaxHealth)));
	}

	if (ManaText)
	{
		ManaText->SetText(
			FText::FromString(FString::Printf(TEXT("Mana      %7.1f / %7.1f"), CachedMana, CachedMaxMana)));
	}

	if (TagHeaderText)
	{
		TagHeaderText->SetText(FText::FromString(TEXT("[Owned Gameplay Tags]")));
	}

	if (TagValuesText)
	{
		TagValuesText->SetText(FText::FromString(CachedOwnedTagsText));
	}

	if (EffectHeaderText)
	{
		EffectHeaderText->SetText(FText::FromString(TEXT("[Active Gameplay Effects]")));
	}

	if (EffectValuesText)
	{
		EffectValuesText->SetText(FText::FromString(CachedActiveEffectsText));
	}

	if (AbilityHeaderText)
	{
		AbilityHeaderText->SetText(FText::FromString(TEXT("[Activatable Abilities]")));
	}

	if (AbilityValuesText)
	{
		AbilityValuesText->SetText(FText::FromString(CachedAbilitiesText));
	}

	if (HintText)
	{
		HintText->SetText(FText::FromString(TEXT("F12: 显示/隐藏面板（大字体，本地项目内 GAS 调试）")));
	}
}

FString UAuraAttributeDebugWidget::FormatTagContainer(const FGameplayTagContainer& TagContainer)
{
	if (TagContainer.IsEmpty())
	{
		return TEXT("<none>");
	}

	TArray<FString> TagNames;
	for (const FGameplayTag& Tag : TagContainer.GetGameplayTagArray())
	{
		TagNames.Add(Tag.ToString());
	}

	TagNames.Sort();
	return FString::Join(TagNames, TEXT("\n"));
}

FString UAuraAttributeDebugWidget::FormatDuration(const float Duration)
{
	if (Duration < 0.f || FMath::IsNearlyEqual(Duration, UGameplayEffect::INFINITE_DURATION))
	{
		return TEXT("∞");
	}

	if (FMath::IsNearlyEqual(Duration, UGameplayEffect::INSTANT_APPLICATION))
	{
		return TEXT("Instant");
	}

	return FString::Printf(TEXT("%.2fs"), Duration);
}

void UAuraAttributeDebugWidget::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CachedHealth = ChangeData.NewValue;
	UpdateDisplayedValues();
}

void UAuraAttributeDebugWidget::HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CachedMaxHealth = ChangeData.NewValue;
	UpdateDisplayedValues();
}

void UAuraAttributeDebugWidget::HandleManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CachedMana = ChangeData.NewValue;
	UpdateDisplayedValues();
}

void UAuraAttributeDebugWidget::HandleMaxManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CachedMaxMana = ChangeData.NewValue;
	UpdateDisplayedValues();
}

void UAuraAttributeDebugWidget::ConfigureTextBlock(UTextBlock* TextBlock, const int32 FontSize,
                                                   const FLinearColor& Color,
                                                   const bool bBold)
{
	if (!TextBlock)
	{
		return;
	}

	TextBlock->SetFont(FCoreStyle::GetDefaultFontStyle(bBold ? TEXT("Bold") : TEXT("Regular"), FontSize));
	TextBlock->SetColorAndOpacity(FSlateColor(Color));
	TextBlock->SetShadowOffset(FVector2D(1.5f, 1.5f));
	TextBlock->SetShadowColorAndOpacity(FLinearColor::Black);
}
