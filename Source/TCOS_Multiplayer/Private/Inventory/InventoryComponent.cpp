// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Libraries/TCOSAbilitySystemLibrary.h"
#include "AbilitySystem/TCOSAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"


bool FPackagedInventory::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	SafeNetSerializeTArray_WithNetSerialize<100>(Ar, ItemTags, Map);
	SafeNetSerializeTArray_Default<100>(Ar, ItemQuantities);

	bOutSuccess = true;
	return true;
}

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, CachedInventory);
}

void UInventoryComponent::ServerUseItem_Implementation(const FGameplayTag& ItemTag, int32 NumItems)
{
	if (InventoryTagMap.Contains(ItemTag))
	{
		UseItem(ItemTag, NumItems);
	}
}

void UInventoryComponent::PackageInventory(FPackagedInventory& OutInventory)
{
	OutInventory.ItemTags.Empty();
	OutInventory.ItemQuantities.Empty();

	for (const auto& Pair : InventoryTagMap)
	{
		if (Pair.Value > 0)
		{
			OutInventory.ItemTags.Add(Pair.Key);
			OutInventory.ItemQuantities.Add(1);
		}
	}
}

void UInventoryComponent::ReconstructInventoryMap(const FPackagedInventory& Inventory)
{
	if (Inventory.ItemTags.Num() != Inventory.ItemQuantities.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: Inventory data corrupted!"));
		return;
	}

	InventoryTagMap.Empty();

	for (int32 i = 0; i < Inventory.ItemTags.Num(); ++i)
	{
		if (Inventory.ItemQuantities.IsValidIndex(i)) 
		{
			InventoryTagMap.Emplace(Inventory.ItemTags[i], Inventory.ItemQuantities[i]);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: Mismatch between ItemTags and ItemQuantities!"));
		}

		if (Inventory.ItemTags[i].IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
				FString::Printf(TEXT("Tag Added: %s // Quantity Added: %d"),
					*Inventory.ItemTags[i].ToString(), Inventory.ItemQuantities[i]));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: Invalid GameplayTag detected!"));
		}

	}

}

void UInventoryComponent::OnRep_CachedInventory()
{
	if (bOwnerLocallyController)
	{
		ReconstructInventoryMap(CachedInventory);
		InventoryPackagedDelegate.Broadcast(CachedInventory);
	}
}

void UInventoryComponent::AddItem(const FGameplayTag& ItemTag, int32 NumItems)
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	if (!Owner->HasAuthority())
	{
		ServerAddItem(ItemTag, NumItems);
		return;
	}

	if (InventoryTagMap.Contains(ItemTag))
	{
		InventoryTagMap[ItemTag] += NumItems;
	}
	else
	{
		InventoryTagMap.Emplace(ItemTag, NumItems);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			 FString::Printf(TEXT("Server Item Added To Inventory %s, qty: %d"), *ItemTag.ToString(), NumItems));

	PackageInventory(CachedInventory);
	InventoryPackagedDelegate.Broadcast(CachedInventory);
}

void UInventoryComponent::UseItem(const FGameplayTag& ItemTag, int32 NumItems)
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	if (!Owner->HasAuthority())
	{
		ServerUseItem(ItemTag, NumItems);
		return;
	}

	const FMasterItemDefinition Item = GetItemDefinitionByTag(ItemTag);

	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
	{
		if (IsValid(Item.ConsumableProps.ItemEffectClass))
		{
			const FGameplayEffectContextHandle ContextHandle = OwnerASC->MakeEffectContext();
			const FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(Item.ConsumableProps.ItemEffectClass,
						Item.ConsumableProps.ItemEffectLevel, ContextHandle);
			OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			AddItem(ItemTag, -1);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Server Item Used: %s"), *Item.ItemTag.ToString()));
		}
	}
}

FMasterItemDefinition UInventoryComponent::GetItemDefinitionByTag(const FGameplayTag& ItemTag) const
{
	checkf(InventoryDefinitions, TEXT("No Inventory Definitions Inside Component %s"), *GetNameSafe(this));

	for (const auto& Pair : InventoryDefinitions->TagsToTables)
	{
		if (ItemTag.MatchesTag(Pair.Key))
		{
			return *UTCOSAbilitySystemLibrary::GetDataTableRowByTag<FMasterItemDefinition>(Pair.Value, ItemTag);
		}
	}

	return FMasterItemDefinition();
}

TMap<FGameplayTag, int32> UInventoryComponent::GetInventoryTagMap()
{
	return InventoryTagMap;
}

void UInventoryComponent::ServerAddItem_Implementation(const FGameplayTag& ItemTag, int32 NumItems)
{
	AddItem(ItemTag, NumItems);
}