// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryItemData.h"
#include "InventoryManagerSubsystem.generated.h"

// Delegate declarations - these broadcast events when inventory changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, UInventoryItemData*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, FGuid, ItemGUID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemStackChanged, FGuid, ItemGUID, int32, NewStackSize);

/**
 * Subsystem that manages the player's inventory
 * Handles adding, removing, searching, and organizing items
 * Persists across level transitions
 */
UCLASS()
class ADAPTIVEINVENTORY_API UInventoryManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// SUBSYSTEM LIFE CYCLE
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	// EVENTS - UI widgets bind to these to know when inventory changes
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryChanged OnInventoryChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemAdded OnItemAdded;
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemRemoved OnItemRemoved;
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemStackChanged OnItemStackChanged;
	
	
	// INVENTORY OPERATIONS
	
	/**
	 * Add an item to the inventory
	 * If stackable and matching item exists, will stack them
	 * @param Item - The item data to add
	 * @return True if item was added successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UInventoryItemData* Item);
	
	/**
	 * Remove an item by its GUID
	 * @param ItemGUID - Unique identifier of the item
	 * @return True if item was found and removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FGuid ItemGUID);
	
	/**
	 * Remove a specific quantity from a stack
	 * @param ItemGUID - Unique identifier of the item
	 * @param Quantity - How many to remove
	 * @return True if successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemQuantity(FGuid ItemGUID, int32 Quantity);
	
	/**
	 * Find an item by its GUID
	 * @param ItemGUID - Unique identifier to search for
	 * @return The item data, or nullptr if not found
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryItemData* FindItemByGUID(FGuid ItemGUID);
	
	/**
	 * Clear all items from inventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();
	
	// QUERIES AND FILTERING - Read Only Const
	
	/**
	 * Get all items in the inventory
	 * @return Array of all item data objects
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UInventoryItemData*> GetAllItems() const { return Items; }
	
	/**
	 * Get items filtered by category
	 * @param Category - The category to filter by
	 * @return Array of matching items
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UInventoryItemData*> GetItemsByCategory(EItemCategory Category) const;
	
	/**
	 * Get items filtered by rarity
	 * @param Rarity - The rarity to filter by
	 * @return Array of matching items
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UInventoryItemData*> GetItemsByRarity(EItemRarity Rarity) const;
	
	/**
	 * Search items by name (partial match)
	 * @param SearchText - Text to search for in item names
	 * @return Array of matching items
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UInventoryItemData*> SearchItemsByName(const FString& SearchText) const;
	
	/**
	 * Get total number of items (counting stacks as 1 item)
	 * Will give you how many unique item entries are in the inventory, not counting how many are in each stack.
	 * @return Item count
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemCount() const { return Items.Num(); }
	
	/**
	 * Get total number of individual items (counting stack quantities)
	 * @return Total quantity of all items
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetTotalItemQuantity() const;
	
	/**
	 * Check if inventory has room for more items
	 * @return True if not at capacity
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasRoomForItem() const { return Items.Num() < MaxInventorySlots; }

	/**
	 * Get current inventory capacity usage
	 * @return Percentage (0.0 to 1.0)
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	float GetInventoryFillPercentage() const;
	
	// CONFIGURATION
	
	/**
	 * Set maximum inventory slots
	 * @param NewMax - New maximum slot count
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetMaxInventorySlots(int32 NewMax);
	
	/**
	 * Get maximum inventory slots
	 * @return Max slot count
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetMaxInventorySlots() const { return MaxInventorySlots; }
	
protected:
	
	// INTERNAL DATA
	
	// The Actual Inventory Storage
	UPROPERTY()
	TArray<UInventoryItemData*> Items;
	
	// Maximum number of item slots
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Config")
	int32 MaxInventorySlots = 100;

	// Whether to automatically stack items
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Config")
	bool bAutoStack = true;

private:
	// INTERNAL HELPERS
	
	/**
	 * Try to stack a new item with existing items
	 * Distributes items across multiple matching stacks if needed
	 * @param NewItem - The item to try stacking (stack size may be modified if partial)
	 * @return True if ALL items were stacked, false if some remain
	 */
	bool TryStackItem(UInventoryItemData* NewItem);

	/**
	 * Validate that an item is valid and can be added
	 * @param Item - Item to validate
	 * @return True if valid
	 */
	bool IsItemValid(UInventoryItemData* Item) const;
};