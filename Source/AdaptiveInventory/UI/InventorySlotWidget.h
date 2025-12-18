// InventorySlotWidget.h
// Widget representing a single inventory slot
// Displays item icon, rarity, stack count, and handles selection/hover states

#pragma once

#include "CoreMinimal.h"
#include "UI/InventoryWidgetBase.h"
#include "UI/InventoryStyleTypes.h"
#include "Core/InventoryItemData.h"
#include "InventorySlotWidget.generated.h"

// Forward declarations
class UImage;
class UTextBlock;
class UBorder;
class UInventoryStyleData;

// Delegate for slot interactions
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UInventorySlotWidget*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotHovered, UInventorySlotWidget*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUnhovered, UInventorySlotWidget*, Slot);

/**
 * Widget representing a single inventory slot
 * Can display an item or be empty
 * Handles visual states: empty, filled, hovered, selected
 */
UCLASS(Blueprintable)
class ADAPTIVEINVENTORY_API UInventorySlotWidget : public UInventoryWidgetBase
{
	GENERATED_BODY()

public:
	UInventorySlotWidget(const FObjectInitializer& ObjectInitializer);

	//~ Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//~ End UUserWidget Interface

	//~ Begin UInventoryWidgetBase Interface
	virtual void RefreshWidget_Implementation() override;
	//~ End UInventoryWidgetBase Interface

	// SLOT DATA

	/**
	 * Set the item this slot displays
	 * @param NewItem - Item to display, or nullptr for empty slot
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void SetItem(UInventoryItemData* NewItem);

	/**
	 * Get the item this slot displays
	 * @return Current item, or nullptr if empty
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	UInventoryItemData* GetItem() const { return CurrentItem; }

	/**
	 * Check if this slot has an item
	 * @return True if slot contains an item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	bool HasItem() const { return CurrentItem != nullptr; }

	/**
	 * Clear the slot (remove item reference)
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void ClearSlot();

	// SLOT STATE

	/**
	 * Set the selected state of this slot
	 * @param bNewSelected - Whether slot should appear selected
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void SetSelected(bool bNewSelected);

	/**
	 * Check if this slot is selected
	 * @return True if selected
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	bool IsSlotSelected() const { return bIsSelected; }

	/**
	 * Set the slot index (position in grid)
	 * @param NewIndex - Grid index
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void SetSlotIndex(int32 NewIndex) { SlotIndex = NewIndex; }

	/**
	 * Get the slot index
	 * @return Grid index
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	int32 GetSlotIndex() const { return SlotIndex; }

	// EVENTS

	/** Fired when slot is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Inventory Slot|Events")
	FOnSlotClicked OnSlotClicked;

	/** Fired when mouse enters slot */
	UPROPERTY(BlueprintAssignable, Category = "Inventory Slot|Events")
	FOnSlotHovered OnSlotHovered;

	/** Fired when mouse leaves slot */
	UPROPERTY(BlueprintAssignable, Category = "Inventory Slot|Events")
	FOnSlotUnhovered OnSlotUnhovered;

protected:
	// ----------------------------------------
	// Style Configuration
	// ----------------------------------------

	/** Style data asset (optional - use for preset themes) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot|Style")
	TObjectPtr<UInventoryStyleData> StyleData;

	/** Inline style (used if StyleData is null) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot|Style")
	FInventorySlotStyle SlotStyle;

	/** Get active style (from StyleData or inline) */
	const FInventorySlotStyle& GetSlotStyle() const;

	/** Get rarity colors from active style */
	FLinearColor GetRarityColor(EItemRarity Rarity) const;

	// WIDGET BINDINGS (set in Blueprint)

	/** Background border */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
	TObjectPtr<UBorder> SlotBorder;

	/** Item icon image */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
	TObjectPtr<UImage> ItemIcon;

	/** Stack count text */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
	TObjectPtr<UTextBlock> StackCountText;

	/** Rarity indicator bar */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
	TObjectPtr<UImage> RarityBar;

	/** Selection highlight overlay */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
	TObjectPtr<UImage> SelectionHighlight;

	// VISUAL UPDATE METHODS (Blueprint overridable)

	/** Update all visuals based on current item */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
	void UpdateVisuals();
	virtual void UpdateVisuals_Implementation();

	/** Update the item icon display */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
	void UpdateIcon();
	virtual void UpdateIcon_Implementation();

	/** Update the stack count display */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
	void UpdateStackCount();
	virtual void UpdateStackCount_Implementation();

	/** Update the rarity indicator */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
	void UpdateRarityDisplay();
	virtual void UpdateRarityDisplay_Implementation();

	/** Update selection/hover visuals */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
	void UpdateSelectionState();
	virtual void UpdateSelectionState_Implementation();

	/** Get the color for a given rarity */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot|Visuals")
	FLinearColor GetRarityColor(EItemRarity Rarity) const;

private:
	/** The item currently displayed in this slot */
	UPROPERTY()
	TObjectPtr<UInventoryItemData> CurrentItem;

	/** Whether this slot is currently selected */
	bool bIsSelected = false;

	/** Whether mouse is currently over this slot */
	bool bIsHovered = false;

	/** Index of this slot in the inventory grid */
	int32 SlotIndex = -1;

	// --- Display Data Pattern ---
	// Gather all display info once, pass to update functions.
	// This centralizes null-checks and makes update functions simple.

	struct FSlotDisplayData
	{
		bool bHasItem = false;
		bool bIsStackable = false;
		bool bIsFullStack = false;
		int32 StackCount = 0;
		int32 MaxStack = 1;
		EItemRarity Rarity = EItemRarity::Common;
		UTexture2D* Icon = nullptr;
	};

	/** Gather all item data needed for display in one place */
	FSlotDisplayData GatherDisplayData() const;

	/** Calculate border color based on current state */
	FLinearColor CalculateBorderColor(const FSlotDisplayData& Data) const;
};