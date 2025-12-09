# Adaptive Inventory System

A learning project exploring performance-optimized UI systems in Unreal Engine 5, built with C++ and Blueprints.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.7-blue?logo=unrealengine)
![C++](https://img.shields.io/badge/C++-17-00599C?logo=cplusplus)
![License](https://img.shields.io/badge/License-MIT-green)
![Status](https://img.shields.io/badge/Status-In%20Development-yellow)

---

## Overview

This project is my hands-on exploration of Epic's recommended patterns for UMG development. I'm learning how to build game UI systems by implementing a fully functional inventory system. The focus is on:

- **Clean Architecture** — Understanding MVVM pattern and data/logic/UI separation
- **Performance** — Exploring event-driven updates instead of tick-based polling
- **Scalability** — Learning the subsystem pattern for persistent game systems
- **Blueprint Integration** — Making systems designer-friendly

**Why inventory?** Inventory systems are in almost every game, and optimizing them well is surprisingly challenging. It's a perfect way to learn UMG performance, profiling tools, and clean architecture.

---

## What I'm Learning

### Technical Skills
- ✅ C++ subsystem architecture in UE5
- ✅ Data-driven design patterns
- ✅ Event-driven UI updates (avoiding Tick)
- 🚧 Widget base classes and Blueprint integration
- 📋 Drag-and-drop interactions
- 📋 Performance profiling and optimization
- 📋 Cross-platform UI considerations

### Current Progress
**Week 1 - Foundation** ✅
- [x] Core data structures (UInventoryItemData)
- [x] Inventory subsystem with CRUD operations
- [x] Auto-stacking logic with overflow handling
- [x] Event system for UI binding
- [x] Blueprint function library

**Week 2 - UI Implementation** 🚧
- [ ] Widget base classes (C++)
- [ ] Inventory grid widget
- [ ] Item slot widget with rarity colors
- [ ] Detail panel
- [ ] Drag and drop

**Week 3 - Polish & Optimization** 📋
- [ ] Performance profiling
- [ ] Widget pooling
- [ ] Platform-adaptive layouts

---

## Features

### Core Systems

- **Item Management** — Add, remove, stack, and query items
- **Auto-Stacking** — Intelligent distribution across multiple stacks when one fills up
- **Filtering** — Search by category, rarity, or partial name match
- **Event Broadcasting** — Widgets update only when inventory actually changes (no per-frame checks)

### Item Properties

| Property | Description |
|----------|-------------|
| Name / Description | Localized FText for display |
| Category | Weapon, Consumable, Material, Equipment, Quest |
| Rarity | Common, Uncommon, Rare, Epic, Legendary |
| Stack Size | Current and maximum stack capacity |
| Stats | Damage, speed, durability, weight |
| GUID | Unique identifier per item instance |

---

## Architecture

I'm following Epic's UMG best practices with a three-layer architecture:

```
┌─────────────────────────────────────────────────────────────┐
│                        PRESENTATION                         │
│                   (Blueprint Widgets - WIP)                 │
└─────────────────────────────────────────────────────────────┘
                              │
                              │ Binds to Events
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      BUSINESS LOGIC                         │
│               UInventoryManagerSubsystem                    │
│                                                             │
│  • AddItem()           • GetItemsByCategory()               │
│  • RemoveItem()        • GetItemsByRarity()                 │
│  • TryStackItem()      • SearchItemsByName()                │
│                                                             │
│  Events: OnInventoryChanged, OnItemAdded, OnItemRemoved     │
└─────────────────────────────────────────────────────────────┘
                              │
                              │ Manages
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                          DATA                               │
│                   UInventoryItemData                        │
│                                                             │
│  • Item properties (name, rarity, category, stats)          │
│  • Stack management (AddToStack, RemoveFromStack)           │
│  • GUID generation                                          │
└─────────────────────────────────────────────────────────────┘
```

---

## Quick Start

### 1. Access the Inventory Manager

```cpp
// C++ - Get the subsystem
UInventoryManagerSubsystem* Inventory = 
    GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
```

```
// Blueprint - Right-click in Event Graph
Search: "Get Inventory Manager Subsystem"
```

### 2. Create and Add Items

```cpp
// C++ Example
UInventoryItemData* Sword = NewObject<UInventoryItemData>(GetGameInstance());
Sword->ItemName = FText::FromString("Iron Sword");
Sword->ItemCategory = EItemCategory::Weapon;
Sword->ItemRarity = EItemRarity::Uncommon;
Sword->MinDamage = 10.0f;
Sword->MaxDamage = 20.0f;

if (Inventory->AddItem(Sword))
{
    UE_LOG(LogTemp, Log, TEXT("Added sword to inventory"));
}
```

### 3. Listen for Changes

```cpp
// C++ - Bind to events (no Tick needed!)
Inventory->OnItemAdded.AddDynamic(this, &UMyWidget::HandleItemAdded);
Inventory->OnInventoryChanged.AddDynamic(this, &UMyWidget::RefreshDisplay);
```

```
// Blueprint
Bind Event to OnInventoryChanged → Refresh UI function
```

---

## File Structure

```
Source/AdaptiveInventory/
├── Public/Core/
│   ├── InventoryItemData.h              # Item data class
│   └── InventoryManagerSubsystem.h      # Inventory business logic
├── Private/Core/
│   ├── InventoryItemData.cpp
│   └── InventoryManagerSubsystem.cpp

Content/AdaptiveInventory/
├── Core/
│   ├── Data/                            # Test item blueprints
│   └── Blueprints/                      # Test actors
├── UI/Widgets/                          # UMG widgets (coming soon)
├── Items/Icons/                         # Item textures (coming soon)
└── Testing/                             # Debug/test blueprints
```

---

## Technical Decisions & Learning

### Why GameInstanceSubsystem?

I chose this pattern because:
- Persists across level loads automatically
- No singleton boilerplate needed
- Built-in Blueprint exposure via `UFUNCTION`
- Epic recommends it for game-wide systems

**What I learned:** Subsystems handle their own lifecycle, which eliminates a lot of manual setup code.

### Why Event-Driven Architecture?

Instead of checking every frame if inventory changed (Tick):
- UI binds to events (OnInventoryChanged, OnItemAdded, etc.)
- Updates only fire when something actually changes
- Zero CPU cost when inventory is idle

**What I learned:** This is how Fortnite's UI works - it scales to complex UIs without performance hits.

### Why Separate Data Classes?

Following Epic's UMG best practices:
- Data (UInventoryItemData) lives independently of UI
- Multiple widgets can show the same data
- Can test business logic without any UI

**What I learned:** This is the "MVVM" pattern - keeps code clean and testable.

---

## Stacking System

One of the trickier parts was implementing smart auto-stacking:

```
Adding 80 Iron Ore (max stack: 99)
├── Existing stack: 50/99
├── After add: 99/99 (added 49)
└── Overflow: Creates new stack 31/99

Result: Two stacks totaling 130 items
```

Items match by **Name** + **Category**. The system distributes items across all partial stacks before creating new slots.

**Challenge solved:** Handling overflow when a stack is almost full. The system now splits automatically.

---

## Performance Goals

Learning to optimize for these targets:

| Metric | Target | Why |
|--------|--------|-----|
| Frame Rate | 60 FPS | Console requirement |
| Active Widgets | < 100 | Widget pooling practice |
| Draw Calls | < 20/frame | Reduce GPU overhead |
| UI Memory | < 180 MB | Mobile/Switch consideration |
| CPU Time | < 2ms/frame | UI budget in AAA games |

---

## Built With

- **Unreal Engine 5.7** — Game engine
- **C++17** — Core logic implementation
- **UMG (Unreal Motion Graphics)** — UI framework
- **Blueprint Visual Scripting** — Designer workflow

---

## Challenges & Solutions

### Challenge 1: Items Not Stacking
**Problem:** Created two "Health Potions" but they didn't combine.  
**Solution:** Item names are case-sensitive! Also need to match category.  
**Lesson:** String comparison in C++ requires exact matches.

### Challenge 2: Compilation Errors
**Problem:** "Cannot resolve symbol" errors in Visual Studio.  
**Solution:** Every function in .cpp needs declaration in .h file.  
**Lesson:** Header/implementation files must stay in sync.

### Challenge 3: Subsystem Not Showing in Blueprint
**Problem:** Couldn't find subsystem in Blueprint search.  
**Solution:** Needed to compile C++ and restart editor.  
**Lesson:** Blueprint reflection requires editor restart after C++ changes.

---

## What's Next?

**Immediate:**
- Creating widget base classes in C++
- Building the grid layout widget
- Implementing item slot visuals

**Soon:**
- Drag-and-drop functionality
- Performance profiling dashboard
- Widget pooling for optimization

**Eventually:**
- Touch controls for mobile
- Controller navigation for console
- Accessibility features (colorblind modes, text scaling)

---

## Resources I'm Using

- [Epic's Programming Subsystems Docs](https://docs.unrealengine.com/5.0/en-US/programming-subsystems-in-unreal-engine/)
- [Epic's UMG Best Practices Blog](https://www.unrealengine.com/en-US/blog/umg-best-practices)
- [Unreal Slackers Discord](https://unrealslackers.org/) — Community help
- [Ben UI's Unreal Garden](https://unreal-garden.com/) — UMG tutorials
- Fortnite's inventory as a reference

---

## About Me

**Rachel Bazelais** — UI Designer & Engineer

I'm a UI Engineer with experience in games and web, now deepening my Unreal Engine 5 skills and exploring technical UI design at a systems level. 
This project is my way of learning UE5's architecture patterns, C++ best practices, and performance optimization techniques used in AAA games.

**Background:**
- UI implementation for video games and websites
- Currently expanding: UE5 C++ systems programming and technical UI architecture
- Interested in: Technical UI Designer roles that bridge design and engineering

**Connect:**
- GitHub: [Your GitHub](https://github.com/RBazelais)
- Portfolio: [Your Website](https://rbazelais.itch.io)
- LinkedIn: [Your LinkedIn](https://wwww.linkedin.com/in/RBazelais)

---

## License

MIT License - Feel free to use this as a learning resource!

---

## Acknowledgments

- **Epic Games** for comprehensive UE5 documentation
- **Unreal Slackers community** for answering my many questions
- **Fortnite's UI** as design inspiration and reference
- Everyone who's shared UMG tutorials and best practices

---

**Note:** This is a learning project and work in progress. Code quality improves as I learn more about Unreal Engine development. Feedback and suggestions are always welcome!
