// 版权没有，拷贝自由。


#include "UI/AttributeMenu/AttributeMenu.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAttributeMenu::WidgetControllerSet_Implementation()
{
	//Super::WidgetControllerSet_Implementation();
	auto AttrMenuWidgetController = UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(this);
	if (!AttrMenuWidgetController) return;
}
