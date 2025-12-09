// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/InventoryManagerSubsystem.h"
#include "Core/InventoryItemData.h"

// Initialize the subsystem
void UInventoryManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    
	UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Initialized"));
    
	// Initialize the inventory array
	Items.Empty();
}

// Clean up on shutdown
void UInventoryManagerSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Shutting Down"));
	
	// Clean up inventory data
	ClearInventory();
	
	Super::Deinitialize();
}

// Add an item to the inventory
bool UInventoryManagerSubsystem::AddItem(UInventoryItemData* Item)
{
	// Validation
	if (!IsItemValid(Item))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManagerSubsystem: Attempted to add invalid item"));
		return false;
	}
	
	// Check if inventory is full
	if (!HasRoomForItem() && !Item->CanStack())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManagerSubsystem: Inventory is full"));
		return false;
	}
	
	// Try to stack with existing items if enabled
	if (bAutoStack && Item->CanStack())
	{
		if (TryStackItem(Item))
		{
			UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Stacked item %s"), *Item->GetItemName().ToString());
			
			// Broadcast stack changed events
			OnItemStackChanged.Broadcast(Item->GetItemGUID(), Item->GetCurrentStackSize());
			OnInventoryChanged.Broadcast();
			
			return true;
		}
	}
	
	// Check capacity again for new slot
	if (!HasRoomForItem())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManagerSubsystem: No room for new item"));
		return false;
	}
	
	// Add new item to inventory
	Items.Add(Item);
	
	UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Added new item %s (Total items: %d)"), 
		*Item->GetItemName().ToString(), Items.Num());
	
	// Broadcast item added events
	OnItemAdded.Broadcast(Item);
	OnInventoryChanged.Broadcast();
	
	return true;
}

// Remove an item completely from inventory
bool UInventoryManagerSubsystem::RemoveItem(FGuid ItemGUID)
{
	// Find the item
	UInventoryItemData* FoundItem = FindItemByGUID(ItemGUID);
	
	if (!FoundItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManagerSubsystem: Item not found for removal"));
		return false;
	}
	
	// Remove from the array
	Items.Remove(FoundItem);
	
	UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Removed item %s (Remaining: %d)"),
		*FoundItem->GetItemName().ToString(), Items.Num());
	
	// Broadcast item removed events
	OnItemRemoved.Broadcast(ItemGUID);
	OnInventoryChanged.Broadcast();
	
	return true;
}

// Remove a specific quantity from a stack
bool UInventoryManagerSubsystem::RemoveItemQuantity(FGuid ItemGUID, int32 Quantity)
{
	if (Quantity <= 0)
	{
		return false;
	}
	
	UInventoryItemData* FoundItem = FindItemByGUID(ItemGUID);
	if (!FoundItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManagerSubsystem: Item not found for quantity removal"));
		return false;
	}
	
	// If removing entire stack or more, just remove the item
	if (Quantity >= FoundItem->GetCurrentStackSize())
	{
		return RemoveItem(ItemGUID);
	}
	
	// Remove from stack
	if (FoundItem->RemoveFromStack(Quantity))
	{
		UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Removed %d from stack of %s"),
			Quantity, *FoundItem->GetItemName().ToString());
		
		// Broadcast stack changed events
		OnItemStackChanged.Broadcast(ItemGUID, FoundItem->GetCurrentStackSize());
		OnInventoryChanged.Broadcast();
		
		return true;
	}
	
	return false;
}

// Find an item by its GUID
UInventoryItemData* UInventoryManagerSubsystem::FindItemByGUID(FGuid ItemGUID)
{
	for (UInventoryItemData* Item : Items)
	{
		if (Item && Item->GetItemGUID() == ItemGUID)
		{
			return Item;
		}
	}
	
	return nullptr;
}

// Clear all items from inventory
void UInventoryManagerSubsystem::ClearInventory()
{
	int32 PreviousCount = Items.Num();
	Items.Empty();
	
	UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Cleared %d items from inventory"), PreviousCount);
	
	// Broadcast inventory cleared event
	OnInventoryChanged.Broadcast();
}

// Get items filtered by category
TArray<UInventoryItemData*> UInventoryManagerSubsystem::GetItemsByCategory(EItemCategory Category) const
{
	TArray<UInventoryItemData*> FilteredItems;
	
	for (UInventoryItemData* Item : Items)
	{
		if (Item && Item->GetItemCategory() == Category)
		{
			FilteredItems.Add(Item);
		}
	}
	
	return FilteredItems;
}

// Get items filtered by rarity
TArray<UInventoryItemData*> UInventoryManagerSubsystem::GetItemsByRarity(EItemRarity Rarity) const
{
	TArray<UInventoryItemData*> FilteredItems;

	for (UInventoryItemData* Item : Items)
	{
		if (Item && Item->GetItemRarity() == Rarity)
		{
			FilteredItems.Add(Item);
		}
	}

	return FilteredItems;
}

// Search items by name (partial match, case insensitive)
TArray<UInventoryItemData*> UInventoryManagerSubsystem::SearchItemsByName(const FString& SearchText) const
{
	TArray<UInventoryItemData*> MatchingItems;

	if (SearchText.IsEmpty())
	{
		return Items; // Return all if search is empty
	}
	
	FString LowerSearchText = SearchText.ToLower();
	
	for (UInventoryItemData* Item : Items)
	{
		if (Item)
		{
			FString ItemName = Item->GetItemName().ToString().ToLower();
			
			// Partial match search
			if (ItemName.Contains(LowerSearchText))
			{
				MatchingItems.Add(Item);
			}
		}
	}
	
	return MatchingItems;
}

// Calculate total quantity of all items in inventory (including stack sizes)
int32 UInventoryManagerSubsystem::GetTotalItemQuantity() const
{
	int32 TotalQuantity = 0;

	for (UInventoryItemData* Item : Items)
	{
		if (Item)
		{
			TotalQuantity += Item->GetCurrentStackSize();
		}
	}

	return TotalQuantity;
}

// Calculate how full the inventory is as a percentage
float UInventoryManagerSubsystem::GetInventoryFillPercentage() const
{
	if (MaxInventorySlots <= 0)
	{
		return 0.0f;
	}

	return static_cast<float>(Items.Num()) / static_cast<float>(MaxInventorySlots);
}

// Set the maximum number of inventory slots (Never less than 1)
void UInventoryManagerSubsystem::SetMaxInventorySlots(int32 NewMax)
{
	MaxInventorySlots = FMath::Max(1, NewMax); // Ensure at least 1 slot
    
	UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Max inventory slots set to %d"), MaxInventorySlots);
}

// TODO: Consider additional properties for stacking (e.g., durability, unique IDs, modifications, etc.)
// Attempt to stack a new item with existing items in inventory
bool UInventoryManagerSubsystem::TryStackItem(UInventoryItemData* NewItem)
{
	// Ensure Item is valid and stackable
	if (!NewItem || !NewItem->CanStack())
	{
		return false;
	}
	
	int32 RemainingToStack = NewItem->GetCurrentStackSize();
	
	// Look for matching items to stack with
	for (UInventoryItemData* ExistingItem : Items)
	{
		if (!ExistingItem || RemainingToStack <= 0)
		{
			continue;
		}
		
		// Check if items match (same name and category for simple matching)
		bool bItemsMatch = ExistingItem->GetItemName().EqualTo(NewItem->GetItemName()) &&
						  ExistingItem->GetItemCategory() == NewItem->GetItemCategory() &&
						  ExistingItem->CanStack() &&
						  !ExistingItem->IsStackFull();
		
		if (bItemsMatch)
		{
			// Calculate how much space is available in this stack
			int32 SpaceAvailable = ExistingItem->GetMaxStackSize() - ExistingItem->GetCurrentStackSize();
			int32 AmountToAdd = FMath::Min(SpaceAvailable, RemainingToStack);
			
			if (AmountToAdd > 0 && ExistingItem->AddToStack(AmountToAdd))
			{
				RemainingToStack -= AmountToAdd;
				
				UE_LOG(LogTemp, Log, TEXT("InventoryManagerSubsystem: Stacked %d items, %d remaining"), 
					AmountToAdd, RemainingToStack);
			}
		}
	}
	
	// Update NewItem's stack size to whatever couldn't be stacked
	if (RemainingToStack < NewItem->GetCurrentStackSize())
	{
		if (RemainingToStack > 0)
		{
			// Some items remain - update the new item's stack size
			NewItem->SetStackSize(RemainingToStack);
			return false; // Still need to add as new slot
		}
		else
		{
			// Everything was stacked
			return true;
		}
	}
	
	return false; // Couldn't stack anything
}

// Validate that an item is acceptable to add to inventory
bool UInventoryManagerSubsystem::IsItemValid(UInventoryItemData* Item) const
{
	if (!Item)
	{
		return false;
	}
	
	// Check if GUID is valid
	if (!Item->GetItemGUID().IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManagerSubsystem: Item has invalid GUID"));
		return false;
	}
	
	// Check if item name is empty
	if (Item->GetItemName().IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManagerSubsystem: Item has empty name"));
		return false;
	}
	
	// Check if stack size is valid
	if (Item->GetCurrentStackSize() <= 0 || Item->GetCurrentStackSize() > Item->GetMaxStackSize())
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryManagerSubsystem: Item has invalid stack size"));
		return false;
	}
	
	// All validations passed
	return true;
}