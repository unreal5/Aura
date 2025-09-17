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

	// Primary Attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> StrengthRow;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> IntelligenceRow;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> ResilienceRow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> VigorRow;

	// Secondary Attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> ArmorRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> ArmorPenetrationRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> BlockChanceRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> CriticalHitChanceRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> CriticalHitDamageRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> CriticalHitResistanceRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> HealthRegenerationRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> ManaRegenerationRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> MaxHealthRow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UTextValueRow> MaxManaRow;

	// close button
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttributeMenu", meta=(BindWidget))
	TObjectPtr<UAuraButton> CloseButton;
};
