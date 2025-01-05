#include <catch2/catch.hpp>

#include "GildedRose.h"

std::ostream& operator<<(std::ostream& os, const Item& obj)
{
    return os
        << "name: " << obj.name
        << ", sellIn: " << obj.sellIn
        << ", quality: " << obj.quality;
}

TEST_CASE("Check item name")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 0, 0));
    GildedRose app(items);
    REQUIRE("foo" == app.items[0].name);
}

TEST_CASE("UpdateQuality should decrease sellIn by one")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 0, 0));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(-1 == app.items[0].sellIn);
}

TEST_CASE("UpdateQualty should decrease quality by one")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 1, 1));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(0 == app.items[0].quality);
}

TEST_CASE("Negative sell by date, quality degrades twice as fast")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 0, 10));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(8 == app.items[0].quality);
}

TEST_CASE("Quality of an item is never negative")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 0, 0));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(0 == app.items[0].quality);
}

TEST_CASE("Aged Brie increases in quality the older it gets")
{
    std::vector<Item> items;
    items.push_back(Item("Aged Brie", 1, 0));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(1 == app.items[0].quality);
}

TEST_CASE("Quality of an item is never more than 50")
{
    std::vector<Item> items;
    items.push_back(Item("Aged Brie", 1, 50));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(50 == app.items[0].quality);
}

TEST_CASE("Sulfuras never has to be sold or decreases in quality")
{
    std::vector<Item> items;
    items.push_back(Item("Sulfuras, Hand of Ragnaros", 0, 80));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(80 == app.items[0].quality);
    REQUIRE(0 == app.items[0].sellIn);
}

TEST_CASE("Backstage passes increase in quality as sellIn approaches")
{
    std::vector<Item> items;
    items.push_back(Item("Backstage passes to a TAFKAL80ETC concert", 11, 0));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(1 == app.items[0].quality);
}

TEST_CASE("Backstage passes increase in quality by 2 when sellIn is 10 or less")
{
    std::vector<Item> items;
    items.push_back(Item("Backstage passes to a TAFKAL80ETC concert", 10, 0));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(2 == app.items[0].quality);
}

TEST_CASE("Backstage passes increase in quality by 3 when sellIn is 5 or less")
{
    std::vector<Item> items;
    items.push_back(Item("Backstage passes to a TAFKAL80ETC concert", 5, 0));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(3 == app.items[0].quality);
}

TEST_CASE("Backstage passes quality drops to 0 after the concert")
{
    std::vector<Item> items;
    items.push_back(Item("Backstage passes to a TAFKAL80ETC concert", 0, 10));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(0 == app.items[0].quality);
}
