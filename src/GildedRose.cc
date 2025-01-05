#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item>& items)
    : items(items)
{
}

bool GildedRose::isAgedBrie(const Item& item) const
{
    return item.name == "Aged Brie";
}

bool GildedRose::isBackstagePass(const Item& item) const
{
    return item.name.find("Backstage pass") != std::string::npos;
}

bool GildedRose::isHandOfRagnaros(const Item& item) const
{
    return item.name == "Sulfuras, Hand of Ragnaros";
}

bool GildedRose::isConjured(const Item& item) const
{
    return item.name.find("Conjured") != std::string::npos;
}

void GildedRose::decreaseItemQuality(Item& item) const
{
    item.quality--;
    if (isConjured(item)) {
        item.quality--;
    }
}

class IQualityUpdater {
public:
    virtual ~IQualityUpdater() = default;
    virtual void execute(Item& item) = 0;
};

class AgedBrieQualityUpdater final : public IQualityUpdater {
public:
    void execute(Item& item) override
    {
        if (item.quality < 50) {
            item.quality++;
        }
        if (item.sellIn < 0 && item.quality < 50) {
            item.quality++;
        }
        item.sellIn--;
    }
};

class BackstagePassQualityUpdater final : public IQualityUpdater {
public:
    void execute(Item& item) override
    {
        item.quality++;

        if (item.quality < 50) {
            if (item.sellIn <= 10) {
                item.quality++;
            }

            if (item.sellIn <= 5) {
                item.quality++;
            }
        }
        item.sellIn--;
        if (item.sellIn < 0) {
            item.quality = 0;
        }
    }
};

class SulfurasQualityUpdater final : public IQualityUpdater {
public:
    void execute(Item& item) override
    {
        // Sulfuras never has to be sold or decreases in quality
    }
};

void GildedRose::updateQuality()
{
    for (Item& item : items) {

        if (isAgedBrie(item)) {
            AgedBrieQualityUpdater().execute(item);
            continue;
        } else if (isBackstagePass(item)) {
            BackstagePassQualityUpdater().execute(item);
            continue;
        } else if (isHandOfRagnaros(item)) {
            SulfurasQualityUpdater().execute(item);
            continue;
        }

        if (item.quality > 0) {
            decreaseItemQuality(item);
        }

        item.sellIn--;

        if (item.sellIn < 0 && item.quality > 0) {
            decreaseItemQuality(item);
        }
    }
}
