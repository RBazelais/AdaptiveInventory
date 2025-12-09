// InventoryBlueprintLibrary.cpp

#include "Core/InventoryBlueprintLibrary.h"
#include "Core/InventoryManagerSubsystem.h"
#include "Core/InventoryItemData.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

UInventoryItemData* UInventoryBlueprintLibrary::CreateInventoryItem(
	UObject* WorldContextObject,
	FText ItemName,
	FText ItemDescription,
	EItemCategory Category,
	EItemRarity Rarity,
	bool bStackable,
	int32 MaxStackSize,
	int32 InitialStackSize)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateInventoryItem: Invalid world context"));
		return nullptr;
	}
		
	// Create the item as an outer of the GameInstance so it persists
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	UInventoryItemData* NewItem = NewObject<UInventoryItemData>(GameInstance);

	if (NewItem)
	{
		NewItem -> ItemName = ItemName;
		NewItem -> ItemDescription = ItemDescription;
		NewItem -> ItemCategory = Category;
		NewItem -> ItemRarity = Rarity;
		NewItem -> bIsStackable = bStackable;
		NewItem -> MaxStackSize = bStackable ? FMath::Max(1, MaxStackSize) : 1;
		NewItem -> CurrentStackSize = FMath::Clamp(InitialStackSize, 1, NewItem->MaxStackSize);

		UE_LOG(LogTemp, Log, TEXT("Created item: %s (Stack: %d/%d)"), 
			*ItemName.ToString(), NewItem->CurrentStackSize, NewItem->MaxStackSize);
	}
	return NewItem;
}

UInventoryItemData* UInventoryBlueprintLibrary::CreateStackableMaterial(
	UObject* WorldContextObject,
	FText ItemName,
	int32 StackSize,
	int32 MaxStackSize)
{
	return CreateInventoryItem(
		WorldContextObject,
		ItemName,
		FText::FromString(FString::Printf(TEXT("A crafting material: %s"), *ItemName.ToString())),
		EItemCategory::Material,
		EItemRarity::Common,
		true,
		MaxStackSize,
		StackSize
	);
}

UInventoryItemData* UInventoryBlueprintLibrary::CreateWeaponItem(
	UObject* WorldContextObject,
	FText ItemName,
	float MinDamage,
	float MaxDamage,
	float AttackSpeed,
	EItemRarity Rarity)
{
	UInventoryItemData* Weapon = CreateInventoryItem(
		WorldContextObject,
		ItemName,
		FText::FromString(FString::Printf(TEXT("Damage: %.0f-%.0f"), MinDamage, MaxDamage)),
		EItemCategory::Weapon,
		Rarity,
		false,  // Weapons don't stack
		1,
		1
	);

	if (Weapon)
	{
		Weapon->MinDamage = MinDamage;
		Weapon->MaxDamage = MaxDamage;
		Weapon->AttackSpeed = AttackSpeed;
	}
	return Weapon;
}

UInventoryItemData* UInventoryBlueprintLibrary::CreateConsumableItem(
	UObject* WorldContextObject,
	FText ItemName,
	int32 StackSize,
	int32 MaxStackSize,
	EItemRarity Rarity)
{
	return CreateInventoryItem(
		WorldContextObject,
		ItemName,
		FText::FromString(FString::Printf(TEXT("Consumable item: %s"), *ItemName.ToString())),
		EItemCategory::Consumable,
		Rarity,
		true,
		MaxStackSize,
		StackSize
	);
}

UInventoryManagerSubsystem* UInventoryBlueprintLibrary::GetInventoryManager(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	if (!GameInstance)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<UInventoryManagerSubsystem>();
}

void UInventoryBlueprintLibrary::DebugPrintInventory(UObject* WorldContextObject)
{
	UInventoryManagerSubsystem* InventoryManager = GetInventoryManager(WorldContextObject);
	if (!InventoryManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugPrintInventory: Could not get Inventory Manager"));
		return;
	}

	TArray<UInventoryItemData*> AllItems = InventoryManager->GetAllItems();
	
	UE_LOG(LogTemp, Log, TEXT("========== INVENTORY DEBUG =========="));
	UE_LOG(LogTemp, Log, TEXT("Total Slots Used: %d / %d"), 
		InventoryManager->GetItemCount(), InventoryManager->GetMaxInventorySlots());
	UE_LOG(LogTemp, Log, TEXT("Total Item Quantity: %d"), InventoryManager->GetTotalItemQuantity());
	UE_LOG(LogTemp, Log, TEXT("--------------------------------------"));

	for (int32 i = 0; i < AllItems.Num(); i++)
	{
		UInventoryItemData* Item = AllItems[i];
		if (Item)
		{
			UE_LOG(LogTemp, Log, TEXT("[%d] %s - Stack: %d/%d - Category: %d - Rarity: %d"),
				i,
				*Item->GetItemName().ToString(),
				Item->GetCurrentStackSize(),
				Item->GetMaxStackSize(),
				static_cast<int32>(Item->GetItemCategory()),
				static_cast<int32>(Item->GetItemRarity())
			);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("======================================"));
}

void UInventoryBlueprintLibrary::AddTestItemsToInventory(
	UObject* WorldContextObject,
	int32 NumMaterials,
	int32 NumWeapons,
	int32 NumConsumables)
{
	UInventoryManagerSubsystem* InventoryManager = GetInventoryManager(WorldContextObject);
	if (!InventoryManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddTestItemsToInventory: Could not get Inventory Manager"));
		return;
	}

	// Material names for testing
	TArray<FString> MaterialNames = {
		TEXT("Iron Ore"),
		TEXT("Wood"),
		TEXT("Stone"),
		TEXT("Gold Nugget"),
		TEXT("Crystal Shard"),
		TEXT("Leather"),
		TEXT("Cloth"),
		TEXT("Bone Fragment")
	};

	// Weapon names for testing
	TArray<FString> WeaponNames = {
		TEXT("Iron Sword"),
		TEXT("Steel Axe"),
		TEXT("Magic Staff"),
		TEXT("Longbow"),
		TEXT("Dagger"),
		TEXT("War Hammer")
	};

	// Consumable names for testing
	TArray<FString> ConsumableNames = {
		TEXT("Health Potion"),
		TEXT("Mana Potion"),
		TEXT("Stamina Elixir"),
		TEXT("Antidote"),
		TEXT("Bread"),
		TEXT("Cooked Meat")
	};

	// Add materials
	for (int32 i = 0; i < NumMaterials; i++)
	{
		FString Name = MaterialNames[FMath::RandRange(0, MaterialNames.Num() - 1)];
		int32 StackSize = FMath::RandRange(1, 50);
		
		UInventoryItemData* Material = CreateStackableMaterial(
			WorldContextObject,
			FText::FromString(Name),
			StackSize,
			99
		);
		
		if (Material)
		{
			InventoryManager->AddItem(Material);
		}
	}

	// Add weapons
	TArray<EItemRarity> Rarities = {
		EItemRarity::Common,
		EItemRarity::Uncommon,
		EItemRarity::Rare,
		EItemRarity::Epic,
		EItemRarity::Legendary
	};

	for (int32 i = 0; i < NumWeapons; i++)
	{
		FString Name = WeaponNames[FMath::RandRange(0, WeaponNames.Num() - 1)];
		float MinDmg = FMath::RandRange(5.0f, 20.0f);
		float MaxDmg = MinDmg + FMath::RandRange(5.0f, 30.0f);
		EItemRarity Rarity = Rarities[FMath::RandRange(0, Rarities.Num() - 1)];
		
		UInventoryItemData* Weapon = CreateWeaponItem(
			WorldContextObject,
			FText::FromString(Name),
			MinDmg,
			MaxDmg,
			FMath::RandRange(0.8f, 1.5f),
			Rarity
		);
		
		if (Weapon)
		{
			InventoryManager->AddItem(Weapon);
		}
	}

	// Add consumables
	for (int32 i = 0; i < NumConsumables; i++)
	{
		FString Name = ConsumableNames[FMath::RandRange(0, ConsumableNames.Num() - 1)];
		int32 StackSize = FMath::RandRange(1, 15);
		
		UInventoryItemData* Consumable = CreateConsumableItem(
			WorldContextObject,
			FText::FromString(Name),
			StackSize,
			20,
			Rarities[FMath::RandRange(0, 2)] // Common to Rare only
		);
		
		if (Consumable)
		{
			InventoryManager->AddItem(Consumable);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Added %d materials, %d weapons, %d consumables to inventory"),
		NumMaterials, NumWeapons, NumConsumables);
	
	// Print inventory state
	DebugPrintInventory(WorldContextObject);
}


