// InventoryGridWidget.cpp
// Grid container for inventory slots

#include "UI/InventoryGridWidget.h"
#include "UI/InventorySlotWidget.h"
#include "Core/InventoryManagerSubsystem.h"
#include "Components/UniformGridPanel.h"
#include "Components/ScrollBox.h"

// ----------------------------------------
// Lifecycle
// ----------------------------------------

UInventoryGridWidget::UInventoryGridWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Grid needs to respond to inventory changes
	bAutoRefresh = true;
	bBindEventsOnConstruct = true;
}

void UInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CreateSlots();
	PopulateGrid();
}

void UInventoryGridWidget::NativeDestruct()
{
	ClearAllSlots();
	PooledSlots.Empty();
	
	Super::NativeDestruct();
}

// ----------------------------------------
// Refresh & Events
// ----------------------------------------

void UInventoryGridWidget::RefreshWidget_Implementation()
{
	PopulateGrid();
}

void UInventoryGridWidget::OnInventoryChanged_Implementation()
{
	PopulateGrid();
}

void UInventoryGridWidget::OnItemAdded_Implementation(UInventoryItemData* AddedItem)
{
	PopulateGrid();
}

void UInventoryGridWidget::OnItemRemoved_Implementation(FGuid ItemGUID)
{
	// If removed item was selected, clear selection
	if (SelectedItem && SelectedItem->GetItemGUID() == ItemGUID)
	{
		ClearSelection();
	}
	
	PopulateGrid();
}

void UInventoryGridWidget::OnItemStackChanged_Implementation(FGuid ItemGUID, int32 NewStackSize)
{
	// Find and update just that slot
	UInventorySlotWidget* Slot = FindSlotForGUID(ItemGUID);
	if (Slot)
	{
		Slot->RefreshWidget();
	}
}

// ----------------------------------------
// Slot Creation & Pooling
// ----------------------------------------

void UInventoryGridWidget::CreateSlots_Implementation()
{
	if (!SlotGrid) return;
	if (!SlotWidgetClass) return;
	
	// Create Initial Slots
	for (int32 i = 0; i < TotalSlots; i++)
	{
		UInventorySlotWidget* Slot = GetOrCreateSlot();
		if (!Slot) continue;
		
		Slot -> SetSlotIndex(i);
		
		// Bind Events
		Slot -> OnSlotClicked.AddDynamic(this, &UInventoryGridWidget::HandleSlotClicked);
		Slot -> OnSlotHovered.AddDynamic(this, &UInventoryGridWidget::HandleSlotHovered);
		
		// Add to grid
		int32 Row = i / GridColumns;
		int32 Column = i % GridColumns;
		SlotGrid -> AddChildToUniformGrid(Slot, Row, Column);
		
		ActiveSlots.Add(Slot);
	}
}

UInventorySlotWidget* UInventoryGridWidget::GetOrCreateSlot()
{
	// Reuse from pool if available
	if (PooledSlots.Num() > 0)
	{
		UInventorySlotWidget* Slot = PooledSlots.Pop();
		Slot -> SetVisibility(ESlateVisibility::Visible);
		return Slot;
	}
	
	// Create new slot
	if (!SlotWidgetClass) return nullptr;
	
	UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
	return NewSlot;
}

void UInventoryGridWidget::ReturnSlotToPool(UInventorySlotWidget* Slot)
{
	if (!Slot) return;
	
	Slot -> ClearSlot();
	Slot -> SetVisibility(ESlateVisibility::Collapsed);
	Slot -> RemoveFromParent();
	
	PooledSlots.Add(Slot);
}

void UInventoryGridWidget::ClearAllSlots()
{
	for (UInventorySlotWidget* Slot : ActiveSlots)
	{
		if (Slot)
		{
			Slot -> OnSlotClicked.RemoveDynamic(this, &UInventoryGridWidget::HandleSlotClicked);
			Slot -> OnSlotHovered.RemoveDynamic(this, &UInventoryGridWidget::HandleSlotHovered);
			ReturnSlotToPool(Slot);
		}
	}
	ActiveSlots.Empty();
}

// ----------------------------------------
// Grid Population
// ----------------------------------------

void UInventoryGridWidget::PopulateGrid_Implementation()
{
	// Items to display
	TArray<UInventoryItemData*> Items = GetFilteredItems();
	
	// Update each slot
	for (int32 i = 0; i < ActiveSlots.Num(); i++)
	{
		UInventorySlotWidget* Slot = ActiveSlots[i];
		if (!Slot) continue;
		
		if (i < Items.Num())
		{
			// Slot has an item
			Slot -> SetItem(Items[i]);
			Slot -> SetVisibility(ESlateVisibility::Visible);
		}
		else if (bShowEmptySlots)
		{
			// Empty Slot
			Slot -> ClearSlot();
			Slot -> SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			// Hide unused slots
			Slot -> ClearSlot();
			Slot -> SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

TArray<UInventoryItemData*> UInventoryGridWidget::GetFilteredItems() const
{
	UInventoryManagerSubsystem* Manager = GetInventoryManager();
	if (!Manager) return TArray<UInventoryItemData*>();
	
	TArray<UInventoryItemData*> Items = Manager -> GetAllItems();
	
	// Apply category filter
	if (CategoryFilter.IsSet())
	{
		Items = Items.FilterByPredicate([this](UInventoryItemData* Item)
		{
			return Item && Item -> GetItemCategory() == CategoryFilter.GetValue();
		});
	}
	
	// Apply search filter
	if (!SearchFilter.IsEmpty())
	{
		Items = Items.FilterByPredicate([this](UInventoryItemData* Item)
		{
			if (!Item) return false;
			FString ItemName = Item -> GetItemName().ToString().ToLower();
			return ItemName.Contains(SearchFilter.ToLower());
		});
	}
	return Items;
}

TArray<UInventoryItemData*> UInventoryGridWidget::GetDisplayedItems() const
{
	return GetFilteredItems();
}

// ----------------------------------------
// Filtering
// ----------------------------------------

void UInventoryGridWidget::SetCategoryFilter(EItemCategory NewCategory)
{
	CategoryFilter = NewCategory;
	PopulateGrid();
}

void UInventoryGridWidget::SetSearchFilter(const FString& SearchText)
{
	SearchFilter = SearchText;
	PopulateGrid();
}

void UInventoryGridWidget::ClearAllFilters()
{
	CategoryFilter.Reset();
	SearchFilter.Empty();
	PopulateGrid();
}

bool UInventoryGridWidget::HasActiveFilter() const
{
	return CategoryFilter.IsSet() || !SearchFilter.IsEmpty();
}

// ----------------------------------------
// Selection
// ----------------------------------------

void UInventoryGridWidget::SelectItem(UInventoryItemData* Item)
{
	// Deselect previous
	if (SelectedSlot)
	{
		SelectedSlot -> SetSelected(false);
	}
	
	SelectedItem = Item;
	SelectedSlot = nullptr;
	
	//  Find and select new slot
	if (Item)
	{
		SelectedSlot = FindSlotForItem(Item);
		if (SelectedSlot)
		{
			SelectedSlot -> SetSelected(true);
		}
	}
	
	// Broascast selection change
	OnItemSelected.Broadcast(SelectedItem);
}

void UInventoryGridWidget::SelectSlotByIndex(int32 SlotIndex)
{
	if (SlotIndex >= 0 && SlotIndex < ActiveSlots.Num())
	{
		UInventorySlotWidget* Slot = ActiveSlots[SlotIndex];
		if (Slot && Slot -> HasItem())
		{
			SelectItem(Slot -> GetItem());
		}
	}
}

void UInventoryGridWidget::ClearSelection()
{
	if (SelectedSlot)
	{
		SelectedSlot -> SetSelected(false);
	}
	SelectedItem = nullptr;
	SelectedSlot = nullptr;
	
	OnItemSelected.Broadcast(nullptr);
}

UInventorySlotWidget* UInventoryGridWidget::GetSlotAtIndex(int32 Index) const
{
	if (Index >= 0 && Index < ActiveSlots.Num())
	{
		return ActiveSlots[Index];
	}
	return nullptr;
}

// ----------------------------------------
// Event Handlers
// ----------------------------------------

void UInventoryGridWidget::HandleSlotClicked(UInventorySlotWidget* ClickedSlot)
{
	if (!ClickedSlot) return;
	
	if (ClickedSlot -> HasItem())
	{
		SelectItem(ClickedSlot -> GetItem());
	}
	else
	{
		ClearSelection();
	}
}

void UInventoryGridWidget::HandleSlotHovered(UInventorySlotWidget* HoveredSlot)
{
	// Can add tooltip logic here later
}

// ----------------------------------------
// Slot Lookup
// ----------------------------------------

UInventorySlotWidget* UInventoryGridWidget::FindSlotForItem(UInventoryItemData* Item) const
{
	if (!Item) return nullptr;
	
	for (UInventorySlotWidget* Slot : ActiveSlots)
	{
		if (Slot && Slot -> GetItem() == Item)
		{
			return Slot;
		}
	}
	return nullptr;
}

UInventorySlotWidget* UInventoryGridWidget::FindSlotForGUID(const FGuid& ItemGUID) const
{
	for (UInventorySlotWidget* Slot : ActiveSlots)
	{
		if (Slot && Slot->HasItem())
		{
			if (Slot->GetItem()->GetItemGUID() == ItemGUID)
			{
				return Slot;
			}
		}
	}
	
	return nullptr;
}
