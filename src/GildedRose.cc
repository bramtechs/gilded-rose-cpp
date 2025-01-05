#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item>& items)
    : items(items)
{
}

bool GildedRose::isAgedBrie(const Item& item) const
{
    return item.name == "Aged Brie";
}

void GildedRose::updateQuality()
{
    for (Item& item : items) {
        if (!isAgedBrie(item) && item.name != "Backstage passes to a TAFKAL80ETC concert") {
            if (item.quality > 0) {
                if (item.name != "Sulfuras, Hand of Ragnaros") {
                    item.quality = item.quality - 1;
                }
            }
        } else {
            if (item.quality < 50) {
                item.quality = item.quality + 1;

                if (item.name == "Backstage passes to a TAFKAL80ETC concert") {
                    if (item.sellIn < 11) {
                        if (item.quality < 50) {
                            item.quality = item.quality + 1;
                        }
                    }

                    if (item.sellIn < 6) {
                        if (item.quality < 50) {
                            item.quality = item.quality + 1;
                        }
                    }
                }
            }
        }

        if (item.name != "Sulfuras, Hand of Ragnaros") {
            item.sellIn = item.sellIn - 1;
        }

        if (item.sellIn < 0) {
            if (!isAgedBrie(item)) {
                if (item.name != "Backstage passes to a TAFKAL80ETC concert") {
                    if (item.quality > 0) {
                        if (item.name != "Sulfuras, Hand of Ragnaros") {
                            item.quality = item.quality - 1;
                        }
                    }
                } else {
                    item.quality = item.quality - item.quality;
                }
            } else {
                if (item.quality < 50) {
                    item.quality = item.quality + 1;
                }
            }
        }
    }
}
