// InventoryBlueprintLibrary.h
// Helper functions for creating and testing inventory items in Blueprints

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryItemData.h"
#include "InventoryBlueprintLibrary.generated.h"

/**
 * Blueprint Function Library for Inventory System
 * Provides easy-to-use functions for creating items and testing
 */
UCLASS()
class ADAPTIVEINVENTORY_API UInventoryBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * Create a new inventory item with specified properties
	 * @param WorldContextObject - World context (automatically provided in Blueprints)
	 * @param ItemName - Display name of the item
	 * @param ItemDescription - Description text
	 * @param Category - Item category
	 * @param Rarity - Item rarity
	 * @param bStackable - Whether this item can stack
	 * @param MaxStackSize - Maximum stack size (ignored if not stackable)
	 * @param InitialStackSize - Starting stack size
	 * @return The created item data object
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Creation", 
	meta = (WorldContext = "WorldContextObject", Keywords = "make create spawn"))
		static UInventoryItemData* CreateInventoryItem(
			UObject* WorldContextObject,
			FText ItemName,
			FText ItemDescription,
			EItemCategory Category,
			EItemRarity Rarity,
			bool bStackable = false,
			int32 MaxStackSize = 1,
			int32 InitialStackSize = 1);
	
	/**
	 * Create a simple stackable material item (convenience function)
	 * @param WorldContextObject - World context
	 * @param ItemName - Display name
	 * @param StackSize - How many in this stack
	 * @param MaxStackSize - Maximum stack capacity
	 * @return The created item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Creation",
		meta = (WorldContext = "WorldContextObject"))
	static UInventoryItemData* CreateStackableMaterial(
		UObject* WorldContextObject,
		FText ItemName,
		int32 StackSize = 1,
		int32 MaxStackSize = 99);
	
	/**
	 * Create a weapon item with damage stats
	 * @param WorldContextObject - World context
	 * @param ItemName - Display name
	 * @param MinDamage - Minimum damage value
	 * @param MaxDamage - Maximum damage value
	 * @param AttackSpeed - Attack speed multiplier
	 * @param Rarity - Item rarity
	 * @return The created weapon item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Creation",
		meta = (WorldContext = "WorldContextObject"))
	static UInventoryItemData* CreateWeaponItem(
		UObject* WorldContextObject,
		FText ItemName,
		float MinDamage,
		float MaxDamage,
		float AttackSpeed = 1.0f,
		EItemRarity Rarity = EItemRarity::Common);
	
	/**
	 * Create a consumable item
	 * @param WorldContextObject - World context
	 * @param ItemName - Display name
	 * @param StackSize - How many in this stack
	 * @param MaxStackSize - Maximum stack capacity
	 * @param Rarity - Item rarity
	 * @return The created consumable item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Creation",
		meta = (WorldContext = "WorldContextObject"))
	static UInventoryItemData* CreateConsumableItem(
		UObject* WorldContextObject,
		FText ItemName,
		int32 StackSize = 1,
		int32 MaxStackSize = 20,
		EItemRarity Rarity = EItemRarity::Common);
	
	/**
	 * Get the Inventory Manager Subsystem
	 * @param WorldContextObject - World context
	 * @return The inventory manager subsystem, or nullptr if not available
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory",
	   meta = (WorldContext = "WorldContextObject"))
	static class UInventoryManagerSubsystem* GetInventoryManager(UObject* WorldContextObject);
	
	/**
	 * Debug: Print all items in inventory to the log
	 * @param WorldContextObject - World context
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Debug",
		meta = (WorldContext = "WorldContextObject"))
	static void DebugPrintInventory(UObject* WorldContextObject);
	
	/**
	 * Debug: Add test items to inventory for testing
	 * @param WorldContextObject - World context
	 * @param NumMaterials - Number of stackable materials to add
	 * @param NumWeapons - Number of weapons to add
	 * @param NumConsumables - Number of consumables to add
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Debug",
		meta = (WorldContext = "WorldContextObject"))
	static void AddTestItemsToInventory(
		UObject* WorldContextObject,
		int32 NumMaterials = 5,
		int32 NumWeapons = 3,
		int32 NumConsumables = 5);
};
