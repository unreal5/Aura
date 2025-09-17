// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AttributeMenu.generated.h"

class UTextValueRow;
class UAuraButton;
/**
 * 
 */
UCLASS()
class AURA_API UAttributeMenu : public UAuraUserWidget
{
	GENERATED_BODY()

public:
	virtual void WidgetControllerSet_Implementation() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> ArmorRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> ArmorPenetrationRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> BlockChanceRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> CriticalHitChanceRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> CriticalHitDamageRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> CriticalHitResistanceRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> HealthRegenerationRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> ManaRegenerationRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> MaxHealthRow;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> MaxManaRow;

	// close button
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UAuraButton> CloseButton;
};
