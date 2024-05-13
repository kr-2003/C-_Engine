#ifndef TRADING_ENGINE_ITCH_HANDLER_H
#define TRADING_ENGINE_ITCH_HANDLER_H

#include <limits>
#include <algorithm>
#include <cstdint>
#include <list>
#include "CppCommon/include/containers/list.h"
#endif // TRADING_ENGINE_ITCH_HANDLER_H

namespace TradingEngine
{
    namespace Matching
    {
        // ORDER SIDE:
        // BID: Buy
        // ASK: Sell
        enum class OrderSide
        {
            BID,
            ASK
        };

        template <class T_Output_Stream>
        T_Output_Stream &operator<<(T_Output_Stream &os, OrderSide order_side)
        {
            switch (orderSide)
            {
            case OrderSide::BID:
                os << "BID";
                break;
            case OrderSide::ASK:
                os << "ASK";
                break;
            default:
                os << "UNKNOWN ORDER SIDE";
                break;
            }
            return os;
        }

        enum class OrderType
        {
            MARKET,
            LIMIT,
            STOP,
            STOP_LIMIT,
            TRAILING_STOP,
            TRAILING_STOP_LIMIT
        };

        template <class T_Output_Stream>
        T_Output_Stream *operator<<(T_Output_Stream *os, OrderType order_type)
        {
            switch (order_type)
            {
            case OrderType::MARKET:
                os << "MARKET";
                break;
            case OrderType::LIMIT:
                os << "LIMIT";
                break;
            case OrderType::STOP:
                os << "STOP";
                break;
            case OrderType::STOP_LIMIT:
                os << "STOP_LIMIT";
                break;
            case OrderType::TRAILING_STOP:
                os << "TRAILING_STOP";
                break;
            case OrderType::TRAILING_STOP_LIMIT:
                os << "TRAILING_STOP_LIMIT";
                break;
            default:
                os << "UNKNOWN ORDER TYPE";
                break;
            }
        }

        enum class OrderTimeInForce
        {
            GTC, // Good Till Cancel
            FOK, // Fill or Kill
            AON, // All or None
            IOC, // Immediate or Cancel
        };

        template <class T_Output_Stream>
        T_Output_Stream *operator<<(T_Output_Stream *os, OrderTimeInForce order_time_in_force)
        {
            switch (order_time_in_force)
            {
            case OrderTimeInForce::GTC:
                os << "GTC";
                break;
            case OrderTimeInForce::FOK:
                os << "FOK";
                break;
            case OrderTimeInForce::AON:
                os << "AON";
                break;
            case OrderTimeInForce::IOC:
                os << "IOC";
                break;
            default:
                os << "UNKNOWN ORDER TIME IN FORCE";
                break;
            }
        }

        struct Order
        {
            uint64_t id;
            uint32_t symbol_id;
            OrderType type;
            OrderSide side;
            uint64_t price;
            uint64_t stop_price;
            uint64_t quantity;
            uint64_t filled_quantity;
            uint64_t leaves_quantity;
            OrderTimeInForce time_in_force;
            uint64_t max_visible_quantity;
            uint64_t hidden_quantity() const
            {
                if (leaves_quantity < max_visible_quantity)
                {
                    return 0;
                }
                else
                {
                    return leaves_quantity - max_visible_quantity;
                }
            }
            uint64_t visible_quantity() const
            {
                return std::min(leaves_quantity, max_visible_quantity);
            }
            uint64_t slippage;
            int64_t trailing_distance;
            int64_t trailing_step;

            Order() = default;
            Order(uint64_t id, uint32_t symbol_id, OrderType type, OrderSide side, uint64_t price,
                  uint64_t stop_price, uint64_t quantity, OrderTimeInForce time_in_force, uint64_t max_visible_quantity,
                  uint64_t slippage, int64_t trailing_distance, int64_t trailing_step)
                : id(id), symbol_id(symbol_id), type(type), side(side), price(price), stop_price(stop_price),
                  quantity(quantity), filled_quantity(0), leaves_quantity(quantity), time_in_force(time_in_force),
                  max_visible_quantity(max_visible_quantity), slippage(slippage), trailing_distance(trailing_distance),
                  trailing_step(trailing_step) {}
            Order(const Order &) = default;
            Order(Order &&) = default;
            ~Order() = default;

            Order &operator=(const Order &) noexcept = default;
            Order &operator=(Order &&) noexcept = default;

            template <class T_Output_Stream>
            T_Output_Stream &print(T_Output_Stream &os) const
            {
                os << "Order ID: " << id << ", Symbol ID: " << symbol_id << ", Type: " << type << ", Side: " << side
                   << ", Price: " << price << ", Stop Price: " << stop_price << ", Quantity: " << quantity
                   << ", Filled Quantity: " << filled_quantity << ", Leaves Quantity: " << leaves_quantity
                   << ", Time in Force: " << time_in_force << ", Max Visible Quantity: " << max_visible_quantity
                   << ", Hidden Quantity: " << hidden_quantity() << ", Visible Quantity: " << visible_quantity()
                   << ", Slippage: " << slippage << ", Trailing Distance: " << trailing_distance
                   << ", Trailing Step: " << trailing_step;
                return os;
            }

            bool is_market() { return type == OrderType::MARKET; }
            bool is_limit() { return type == OrderType::LIMIT; }
            bool is_stop() { return type == OrderType::STOP; }
            bool is_stop_limit() { return type == OrderType::STOP_LIMIT; }
            bool is_trailing_stop() { return type == OrderType::TRAILING_STOP; }
            bool is_trailing_stop_limit() { return type == OrderType::TRAILING_STOP_LIMIT; }

            bool is_bid() { return side == OrderSide::BID; }
            bool is_ask() { return side == OrderSide::ASK; }

            bool is_gtc() { return time_in_force == OrderTimeInForce::GTC; }
            bool is_fok() { return time_in_force == OrderTimeInForce::FOK; }
            bool is_aon() { return time_in_force == OrderTimeInForce::AON; }
            bool is_ioc() { return time_in_force == OrderTimeInForce::IOC; }

            bool is_hidden() { return max_visible_quantity == 0; }
            bool is_iceberg() { return max_visible_quantity < std::numeric_limits<uint64_t>::max(); }

            bool is_slippage() { return slippage < std::numeric_limits<uint64_t>::max(); }

            static Order Market(uint64_t id, uint32_t symbol, OrderSide side, uint64_t quantity, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::MARKET, side, 0, 0, quantity, OrderTimeInForce::IOC, std::numeric_limits<uint64_t>::max(), slippage, 0, 0);
            }
            static Order BuyMarket(uint64_t id, uint32_t symbol, uint64_t quantity, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::MARKET, OrderSide::BID, 0, 0, quantity, OrderTimeInForce::IOC, std::numeric_limits<uint64_t>::max(), slippage, 0, 0);
            }
            static Order SellMarket(uint64_t id, uint32_t symbol, uint64_t quantity, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::MARKET, OrderSide::ASK, 0, 0, quantity, OrderTimeInForce::IOC, std::numeric_limits<uint64_t>::max(), slippage, 0, 0);
            }
            static Order Limit(uint64_t id, uint32_t symbol, OrderSide side, uint64_t price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::LIMIT, side, price, 0, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), 0, 0);
            }
            static Order BuyLimit(uint64_t id, uint32_t symbol, uint64_t price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::LIMIT, OrderSide::BID, price, 0, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), 0, 0);
            }
            static Order SellLimit(uint64_t id, uint32_t symbol, uint64_t price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::LIMIT, OrderSide::ASK, price, 0, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), 0, 0);
            }
            static Order Stop(uint64_t id, uint32_t symbol, OrderSide side, uint64_t stop_price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::STOP, side, 0, stop_price, quantity, tif, std::numeric_limits<uint64_t>::max(), slippage, 0, 0);
            }
            static Order BuyStop(uint64_t id, uint32_t symbol, uint64_t stop_price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::STOP, OrderSide::BID, 0, stop_price, quantity, tif, std::numeric_limits<uint64_t>::max(), slippage, 0, 0);
            }
            static Order SellStop(uint64_t id, uint32_t symbol, uint64_t stop_price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::STOP, OrderSide::ASK, 0, stop_price, quantity, tif, std::numeric_limits<uint64_t>::max(), slippage, 0, 0);
            }
            static Order StopLimit(uint64_t id, uint32_t symbol, OrderSide side, uint64_t stop_price, uint64_t price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::STOP_LIMIT, side, price, stop_price, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), 0, 0);
            }
            static Order BuyStopLimit(uint64_t id, uint32_t symbol, uint64_t stop_price, uint64_t price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::STOP_LIMIT, OrderSide::BID, price, stop_price, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), 0, 0);
            }
            static Order SellStopLimit(uint64_t id, uint32_t symbol, uint64_t stop_price, uint64_t price, uint64_t quantity, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::STOP_LIMIT, OrderSide::ASK, price, stop_price, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), 0, 0);
            }
            static Order TrailingStop(uint64_t id, uint32_t symbol, OrderSide side, uint64_t stop_price, uint64_t quantity, int64_t trailing_distance, int64_t trailing_step = 0, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::TRAILING_STOP, side, 0, stop_price, quantity, tif, std::numeric_limits<uint64_t>::max(), slippage, trailing_distance, trailing_step);
            }
            static Order TrailingBuyStop(uint64_t id, uint32_t symbol, uint64_t stop_price, uint64_t quantity, int64_t trailing_distance, int64_t trailing_step = 0, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::TRAILING_STOP, OrderSide::BID, 0, stop_price, quantity, tif, std::numeric_limits<uint64_t>::max(), slippage, trailing_distance, trailing_step);
            }
            static Order TrailingSellStop(uint64_t id, uint32_t symbol, uint64_t stop_price, uint64_t quantity, int64_t trailing_distance, int64_t trailing_step = 0, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t slippage = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::TRAILING_STOP, OrderSide::ASK, 0, stop_price, quantity, tif, std::numeric_limits<uint64_t>::max(), slippage, trailing_distance, trailing_step);
            }
            static Order TrailingStopLimit(uint64_t id, uint32_t symbol, OrderSide side, uint64_t stop_price, uint64_t price, uint64_t quantity, int64_t trailing_distance, int64_t trailing_step = 0, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::TRAILING_STOP_LIMIT, side, price, stop_price, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), trailing_distance, trailing_step);
            }
            static Order TrailingBuyStopLimit(uint64_t id, uint32_t symbol, uint64_t stop_price, uint64_t price, uint64_t quantity, int64_t trailing_distance, int64_t trailing_step = 0, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::TRAILING_STOP_LIMIT, OrderSide::BID, price, stop_price, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), trailing_distance, trailing_step);
            }
            static Order TrailingSellStopLimit(uint64_t id, uint32_t symbol, uint64_t stop_price, uint64_t price, uint64_t quantity, int64_t trailing_distance, int64_t trailing_step = 0, OrderTimeInForce tif = OrderTimeInForce::GTC, uint64_t max_visible_quantity = std::numeric_limits<uint64_t>::max()) {
                return Order(id, symbol, OrderType::TRAILING_STOP_LIMIT, OrderSide::ASK, price, stop_price, quantity, tif, max_visible_quantity, std::numeric_limits<uint64_t>::max(), trailing_distance, trailing_step);
            }
        };

        struct LevelNode;

        //! Order node
        struct OrderNode : public Order, public CppCommon::List<OrderNode>::Node
        {
            LevelNode *Level;

            OrderNode(const Order &order) : Order(order), Level(nullptr) {}
            OrderNode(const OrderNode &) noexcept = default;
            OrderNode(OrderNode &&) noexcept = default;
            ~OrderNode() noexcept = default;

            OrderNode &operator=(const Order &order) {
                Order::operator=(order);
                Level = nullptr;
                return *this;
            }
            OrderNode &operator=(const OrderNode &) noexcept = default;
            OrderNode &operator=(OrderNode &&) noexcept = default;
        };
    }
}