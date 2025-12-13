// InventorySlotWidget.cpp
// Single inventory slot - displays one item or empty state

#include "UI/InventorySlotWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Input/Reply.h"

// ----------------------------------------
// Lifecycle
// ----------------------------------------

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Parent grid handles events and refresh, not individual slots
	bAutoRefresh = false;
	bBindEventsOnConstruct = false;
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateVisuals();
}

// ----------------------------------------
// Input Handling
// ----------------------------------------

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	bIsHovered = true;
	UpdateSelectionState();
	OnSlotHovered.Broadcast(this);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	bIsHovered = false;
	UpdateSelectionState();
	OnSlotUnhovered.Broadcast(this);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnSlotClicked.Broadcast(this);
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

// ----------------------------------------
// Public API
// ----------------------------------------

void UInventorySlotWidget::SetItem(UInventoryItemData* NewItem)
{
	CurrentItem = NewItem;
	UpdateVisuals();
}

void UInventorySlotWidget::ClearSlot()
{
	CurrentItem = nullptr;
	bIsSelected = false;
	UpdateVisuals();
}

void UInventorySlotWidget::SetSelected(bool bNewSelected)
{
	if (bIsSelected == bNewSelected) return;
	
	bIsSelected = bNewSelected;
	UpdateSelectionState();
}

void UInventorySlotWidget::RefreshWidget_Implementation()
{
	UpdateVisuals();
}

// ----------------------------------------
// Display Data
// ----------------------------------------

UInventorySlotWidget::FSlotDisplayData UInventorySlotWidget::GatherDisplayData() const
{
	FSlotDisplayData Data;
	
	if (!CurrentItem) return Data;
	
	Data.bHasItem = true;
	Data.Icon = CurrentItem->GetItemIcon();
	Data.Rarity = CurrentItem->GetItemRarity();
	Data.MaxStack = CurrentItem->GetMaxStackSize();
	Data.StackCount = CurrentItem->GetCurrentStackSize();
	Data.bIsStackable = Data.MaxStack > 1;
	Data.bIsFullStack = Data.StackCount >= Data.MaxStack;
	
	return Data;
}

FLinearColor UInventorySlotWidget::CalculateBorderColor(const FSlotDisplayData& Data) const
{
	// Priority: Selected > Hovered > Rarity > Empty
	
	if (bIsSelected)
	{
		return SelectedColor * 0.3f;
	}
	
	if (bIsHovered && Data.bHasItem)
	{
		FLinearColor HoverColor = GetRarityColor(Data.Rarity) * 0.4f;
		HoverColor.A = 0.8f;
		return HoverColor;
	}
	
	if (Data.bHasItem)
	{
		FLinearColor BgColor = GetRarityColor(Data.Rarity) * 0.15f;
		BgColor.A = 0.6f;
		return BgColor;
	}
	
	return EmptySlotColor;
}

FLinearColor UInventorySlotWidget::GetRarityColor(EItemRarity Rarity) const
{
	switch (Rarity)
	{
	case EItemRarity::Common:     return CommonColor;
	case EItemRarity::Uncommon:   return UncommonColor;
	case EItemRarity::Rare:       return RareColor;
	case EItemRarity::Epic:       return EpicColor;
	case EItemRarity::Legendary:  return LegendaryColor;
	default:                      return CommonColor;
	}
}

// ----------------------------------------
// Visual Updates
// ----------------------------------------

void UInventorySlotWidget::UpdateVisuals_Implementation()
{
	FSlotDisplayData Data = GatherDisplayData();
	
	UpdateIcon();
	UpdateStackCount();
	UpdateRarityDisplay();
	UpdateSelectionState();
}

void UInventorySlotWidget::UpdateIcon_Implementation()
{
	if (!ItemIcon) return;
	
	FSlotDisplayData Data = GatherDisplayData();
	
	// Empty slot
	if (!Data.bHasItem)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	// Has item with icon
	if (Data.Icon)
	{
		ItemIcon->SetBrushFromTexture(Data.Icon);
		ItemIcon->SetColorAndOpacity(FLinearColor::White);
		ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		return;
	}
	
	// Has item, no icon (placeholder)
	ItemIcon->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
	ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInventorySlotWidget::UpdateStackCount_Implementation()
{
	if (!StackCountText) return;
	
	FSlotDisplayData Data = GatherDisplayData();
	
	// Only show count for stackable items
	if (!Data.bHasItem || !Data.bIsStackable)
	{
		StackCountText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	StackCountText->SetText(FText::AsNumber(Data.StackCount));
	StackCountText->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	// Green when full, white otherwise
	FLinearColor TextColor = Data.bIsFullStack ? UncommonColor : FLinearColor::White;
	StackCountText->SetColorAndOpacity(FSlateColor(TextColor));
}

void UInventorySlotWidget::UpdateRarityDisplay_Implementation()
{
	FSlotDisplayData Data = GatherDisplayData();
	
	// Rarity bar
	if (RarityBar)
	{
		if (Data.bHasItem)
		{
			RarityBar->SetColorAndOpacity(GetRarityColor(Data.Rarity));
			RarityBar->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			RarityBar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	// Border background color
	if (SlotBorder)
	{
		SlotBorder->SetBrushColor(CalculateBorderColor(Data));
	}
}

void UInventorySlotWidget::UpdateSelectionState_Implementation()
{
	FSlotDisplayData Data = GatherDisplayData();
	
	// Selection highlight overlay
	if (SelectionHighlight)
	{
		if (bIsSelected)
		{
			SelectionHighlight->SetColorAndOpacity(SelectedColor);
			SelectionHighlight->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SelectionHighlight->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	// Border color (handles selected, hovered, rarity, empty states)
	if (SlotBorder)
	{
		SlotBorder->SetBrushColor(CalculateBorderColor(Data));
	}
	
	// Hover scale
	float Scale = (bIsHovered && Data.bHasItem) ? HoverScale : 1.0f;
	SetRenderScale(FVector2D(Scale, Scale));
}