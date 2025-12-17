// InventoryGridWidget.h
// Grid container for inventory slots
// Handles slot pooling, filtering, and selection management

#pragma once

#include "CoreMinimal.h"
#include "UI/InventoryWidgetBase.h"
#include "Core/InventoryItemData.h"
#include "InventoryGridWidget.generated.h"

// Forward declarations
class UInventorySlotWidget;
class UUniformGridPanel;
class UScrollBox;

// Delegate for item selection
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelected, UInventoryItemData*, SelectedItem);

/**
 * Grid widget that displays inventory slots
 * Supports filtering, slot pooling, and selection management
 */
UCLASS(Blueprintable)
class ADAPTIVEINVENTORY_API UInventoryGridWidget : public UInventoryWidgetBase
{
	GENERATED_BODY()

public:
	UInventoryGridWidget(const FObjectInitializer& ObjectInitializer);

	//~ Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~ End UUserWidget Interface

	//~ Begin UInventoryWidgetBase Interface
	virtual void RefreshWidget_Implementation() override;
	virtual void OnInventoryChanged_Implementation() override;
	virtual void OnItemAdded_Implementation(UInventoryItemData* AddedItem) override;
	virtual void OnItemRemoved_Implementation(FGuid ItemGUID) override;
	virtual void OnItemStackChanged_Implementation(FGuid ItemGUID, int32 NewStackSize) override;
	//~ End UInventoryWidgetBase Interface

	// ----------------------------------------
	// Filtering
	// ----------------------------------------

	/** Filter by item category */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid|Filtering")
	void SetCategoryFilter(EItemCategory NewCategory);

	/** Filter by search text (matches item name) */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid|Filtering")
	void SetSearchFilter(const FString& SearchText);

	/** Clear all active filters */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid|Filtering")
	void ClearAllFilters();

	/** Check if any filter is active */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid|Filtering")
	bool HasActiveFilter() const;

	// ----------------------------------------
	// Selection
	// ----------------------------------------

	/** Select an item (highlights slot, broadcasts event) */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid|Selection")
	void SelectItem(UInventoryItemData* Item);

	/** Select slot by grid index */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid|Selection")
	void SelectSlotByIndex(int32 SlotIndex);

	/** Clear current selection */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid|Selection")
	void ClearSelection();

	/** Get currently selected item */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid|Selection")
	UInventoryItemData* GetSelectedItem() const { return SelectedItem; }

	/** Fired when an item is selected */
	UPROPERTY(BlueprintAssignable, Category = "Inventory Grid|Events")
	FOnItemSelected OnItemSelected;

	// ----------------------------------------
	// Grid Access
	// ----------------------------------------

	/** Get all items currently displayed (after filtering) */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid")
	TArray<UInventoryItemData*> GetDisplayedItems() const;

	/** Get slot widget at index */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid")
	UInventorySlotWidget* GetSlotAtIndex(int32 Index) const;

protected:
	// ----------------------------------------
	// Configuration
	// ----------------------------------------

	/** Number of columns in the grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Grid|Config")
	int32 GridColumns = 5;

	/** Total number of slots to display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Grid|Config")
	int32 TotalSlots = 20;

	/** Show empty slots when inventory has fewer items */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Grid|Config")
	bool bShowEmptySlots = true;

	/** Slot widget class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Grid|Config")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	// ----------------------------------------
	// Widget Bindings
	// ----------------------------------------

	/** Grid panel for slot layout */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Grid|Widgets")
	TObjectPtr<UUniformGridPanel> SlotGrid;

	/** Optional scroll box wrapper */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Grid|Widgets")
	TObjectPtr<UScrollBox> ScrollBox;

	// ----------------------------------------
	// Grid Management
	// ----------------------------------------

	/** Create or retrieve slots for the grid */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Grid")
	void CreateSlots();
	virtual void CreateSlots_Implementation();

	/** Populate slots with inventory items */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Grid")
	void PopulateGrid();
	virtual void PopulateGrid_Implementation();

	/** Get filtered list of items to display */
	UFUNCTION(BlueprintCallable, Category = "Inventory Grid")
	TArray<UInventoryItemData*> GetFilteredItems() const;

	/** Handle slot click event */
	UFUNCTION()
	void HandleSlotClicked(UInventorySlotWidget* ClickedSlot);

	/** Handle slot hover event */
	UFUNCTION()
	void HandleSlotHovered(UInventorySlotWidget* HoveredSlot);

	// ----------------------------------------
	// Slot Pooling
	// ----------------------------------------

	/** Get or create a slot widget */
	UInventorySlotWidget* GetOrCreateSlot();

	/** Return slot to pool for reuse */
	void ReturnSlotToPool(UInventorySlotWidget* Slot);

	/** Clear all slots back to pool */
	void ClearAllSlots();

private:
	/** Currently active slots in the grid */
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotWidget>> ActiveSlots;

	/** Pooled slots available for reuse */
	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotWidget>> PooledSlots;

	/** Currently selected item */
	UPROPERTY()
	TObjectPtr<UInventoryItemData> SelectedItem;

	/** Currently selected slot widget */
	UPROPERTY()
	TObjectPtr<UInventorySlotWidget> SelectedSlot;

	/** Active category filter (None = no filter) */
	TOptional<EItemCategory> CategoryFilter;

	/** Active search filter */
	FString SearchFilter;

	/** Find slot displaying a specific item */
	UInventorySlotWidget* FindSlotForItem(UInventoryItemData* Item) const;

	/** Find slot displaying item with GUID */
	UInventorySlotWidget* FindSlotForGUID(const FGuid& ItemGUID) const;
};