// 版本没有，拷贝自由。


#include "AuraAssetManager.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine); // 用于确保GEngine不为空
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}
