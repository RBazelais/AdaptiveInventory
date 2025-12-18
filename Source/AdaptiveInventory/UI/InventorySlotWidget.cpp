// InventorySlotWidget.cpp
// Implementation for inventory slot widget

#include "UI/InventorySlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "UI/InventoryStyleData.h"

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bAutoRefresh = false;
    bBindEventsOnConstruct = false;
}

void UInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UpdateVisuals();
}

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

void UInventorySlotWidget::RefreshWidget_Implementation()
{
    UpdateVisuals();
}

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
    bIsSelected = bNewSelected;
    UpdateSelectionState();
}

const FInventorySlotStyle& UInventorySlotWidget::GetSlotStyle() const
{
    if (StyleData)
    {
        return StyleData->GetSlotStyle();
    }
    return SlotStyle;
}

FLinearColor UInventorySlotWidget::GetRarityColor(EItemRarity Rarity) const
{
    const FInventorySlotStyle& Style = GetSlotStyle();
    
    switch (Rarity)
    {
        case EItemRarity::Common:
            return Style.CommonColors.Primary;
        case EItemRarity::Uncommon:
            return Style.UncommonColors.Primary;
        case EItemRarity::Rare:
            return Style.RareColors.Primary;
        case EItemRarity::Epic:
            return Style.EpicColors.Primary;
        case EItemRarity::Legendary:
            return Style.LegendaryColors.Primary;
        default:
            return FLinearColor::White;
    }
}

void UInventorySlotWidget::UpdateVisuals_Implementation()
{
    UpdateIcon();
    UpdateStackCount();
    UpdateRarityDisplay();
    UpdateSelectionState();
}

void UInventorySlotWidget::UpdateIcon_Implementation()
{
    if (!ItemIcon) return;

    if (CurrentItem && CurrentItem->GetItemIcon())
    {
        ItemIcon->SetBrushFromTexture(CurrentItem->GetItemIcon());
        ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
        ItemIcon->SetColorAndOpacity(FLinearColor::White);
    }
    else if (CurrentItem)
    {
        ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
        ItemIcon->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
    }
    else
    {
        ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UInventorySlotWidget::UpdateStackCount_Implementation()
{
    if (!StackCountText) return;

    if (CurrentItem && CurrentItem->GetMaxStackSize() > 1)
    {
        int32 StackSize = CurrentItem->GetCurrentStackSize();
        int32 MaxStack = CurrentItem->GetMaxStackSize();
        
        StackCountText->SetText(FText::AsNumber(StackSize));
        StackCountText->SetVisibility(ESlateVisibility::HitTestInvisible);
        
        if (StackSize >= MaxStack)
        {
            StackCountText->SetColorAndOpacity(FSlateColor(GetRarityColor(EItemRarity::Uncommon)));
        }
        else
        {
            StackCountText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
        }
    }
    else
    {
        StackCountText->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UInventorySlotWidget::UpdateRarityDisplay_Implementation()
{
    if (!RarityBar) return;

    if (CurrentItem)
    {
        FLinearColor RarityColor = GetRarityColor(CurrentItem->GetItemRarity());
        RarityBar->SetColorAndOpacity(RarityColor);
        RarityBar->SetVisibility(ESlateVisibility::HitTestInvisible);
    }
    else
    {
        RarityBar->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UInventorySlotWidget::UpdateSelectionState_Implementation()
{
    const FInventorySlotStyle& Style = GetSlotStyle();
    
    if (SelectionHighlight)
    {
        if (bIsSelected)
        {
            SelectionHighlight->SetVisibility(ESlateVisibility::HitTestInvisible);
            SelectionHighlight->SetColorAndOpacity(Style.SelectedBorder);
        }
        else
        {
            SelectionHighlight->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    if (SlotBorder)
    {
        if (bIsSelected)
        {
            SlotBorder->SetBrushColor(Style.SelectedBorder * 0.3f);
        }
        else if (bIsHovered && CurrentItem)
        {
            FLinearColor RarityColor = GetRarityColor(CurrentItem->GetItemRarity());
            FLinearColor HoverColor = RarityColor * 0.4f;
            HoverColor.A = 0.8f;
            SlotBorder->SetBrushColor(HoverColor);
        }
        else if (CurrentItem)
        {
            FLinearColor RarityColor = GetRarityColor(CurrentItem->GetItemRarity());
            FLinearColor BgColor = RarityColor * 0.15f;
            BgColor.A = 0.6f;
            SlotBorder->SetBrushColor(BgColor);
        }
        else
        {
            SlotBorder->SetBrushColor(Style.EmptyBorder);
        }
    }

    if (bIsHovered && CurrentItem)
    {
        SetRenderScale(FVector2D(Style.HoverScale, Style.HoverScale));
    }
    else
    {
        SetRenderScale(FVector2D(1.0f, 1.0f));
    }
}

UInventorySlotWidget::FSlotDisplayData UInventorySlotWidget::GatherDisplayData() const
{
    FSlotDisplayData Data;
    
    if (CurrentItem)
    {
        Data.bHasItem = true;
        Data.bIsStackable = CurrentItem->GetMaxStackSize() > 1;
        Data.StackCount = CurrentItem->GetCurrentStackSize();
        Data.MaxStack = CurrentItem->GetMaxStackSize();
        Data.bIsFullStack = Data.StackCount >= Data.MaxStack;
        Data.Rarity = CurrentItem->GetItemRarity();
        Data.Icon = CurrentItem->GetItemIcon();
    }
    
    return Data;
}

FLinearColor UInventorySlotWidget::CalculateBorderColor(const FSlotDisplayData& Data) const
{
    const FInventorySlotStyle& Style = GetSlotStyle();
    
    if (bIsSelected)
    {
        return Style.SelectedBorder * 0.3f;
    }
    else if (bIsHovered && Data.bHasItem)
    {
        FLinearColor RarityColor = GetRarityColor(Data.Rarity);
        FLinearColor HoverColor = RarityColor * 0.4f;
        HoverColor.A = 0.8f;
        return HoverColor;
    }
    else if (Data.bHasItem)
    {
        FLinearColor RarityColor = GetRarityColor(Data.Rarity);
        FLinearColor BgColor = RarityColor * 0.15f;
        BgColor.A = 0.6f;
        return BgColor;
    }
    
    return Style.EmptyBorder;
}
