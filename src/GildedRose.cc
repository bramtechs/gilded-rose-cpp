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
    }
};

void GildedRose::updateQuality()
{
    for (Item& item : items) {

        if (isAgedBrie(item)) {
            AgedBrieQualityUpdater().execute(item);
            item.sellIn--;
            continue;
        }

        if (!isBackstagePass(item)) {
            if (item.quality > 0 && !isHandOfRagnaros(item)) {
                decreaseItemQuality(item);
            }
        } else if (item.quality < 50) {
            item.quality++;

            if (isBackstagePass(item) && item.quality < 50) {
                if (item.sellIn <= 10) {
                    item.quality++;
                }

                if (item.sellIn <= 5) {
                    item.quality++;
                }
            }
        }

        if (!isHandOfRagnaros(item)) {
            item.sellIn--;
        }

        if (item.sellIn < 0) {
            if (isBackstagePass(item)) {
                item.quality = 0;
            } else if (item.quality > 0 && !isHandOfRagnaros(item)) {
                decreaseItemQuality(item);
            }
        }
    }
}
