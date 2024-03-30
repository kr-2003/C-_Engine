#include <iostream>
#include <fstream>
#include <cassert>
#include <sys/time.h>
#include <vector>
#include <cstring>
#include <iomanip>

#ifndef TRADING_ENGINE_ITCH_HANDLER_H
#define TRADING_ENGINE_ITCH_HANDLER_H

namespace TradingEngine
{
    namespace ITCH
    {
        struct system_event_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char event_code;
        };

        struct stock_directory_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char stock[9];
            char market_category;
            char financial_status_indicator;
            uint32_t round_lot_size;
            char round_lots_only;
            char issue_classification;
            char issue_subtype[3];
            char authenticity;
            char short_sale_threshold_indicator;
            char ipo_flag;
            char luld_reference_price_tier;
            char etp_flag;
            uint32_t etp_leverage_factor;
            char inverse_indicator;
        };

        struct stock_trading_action_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char stock[9];
            char trading_state;
            char reserved;
            char reason[5];
        };

        struct reg_sho_restriction_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char stock[9];
            char reg_sho_action;
        };

        struct market_participant_position_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char mpid[5];
            char stock[9];
            char primary_market_manager;
            char market_maker_mode;
            char market_participant_state;
        };

        struct mwcb_decline_level_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t level_1;
            uint64_t level_2;
            uint64_t level_3;
        };

        struct mwcb_status_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char breached_level;
        };

        struct ipo_quoted_period_update
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char stock[9];
            uint32_t ipo_quotation_release_time;
            char ipo_quotation_release_qualifier;
            uint32_t ipo_price;
        };

        struct add_order_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t order_reference_number;
            char buy_sell_indicator;
            uint32_t shares;
            char stock[9];
            uint32_t price;
        };

        struct add_order_with_mpid_attribution_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t order_reference_number;
            char buy_sell_indicator;
            uint32_t shares;
            char stock[9];
            uint32_t price;
            char attribution[5];
        };

        struct order_executed_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t order_reference_number;
            uint32_t executed_shares;
            uint64_t match_number;
        };

        struct order_executed_with_price_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t order_reference_number;
            uint32_t executed_shares;
            uint64_t match_number;
            char printable;
            uint64_t execution_price;
        };

        struct order_cancel_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t order_reference_number;
            uint32_t canceled_shares;
        };

        struct order_delete_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t order_reference_number;
        };

        struct order_replace_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t order_reference_number;
            uint64_t new_order_reference_number;
            uint32_t shares;
            uint32_t price;
        };

        struct non_cross_trade_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t order_reference_number;
            char buy_sell_indicator;
            uint32_t shares;
            char stock[9];
            uint32_t price;
            uint64_t matching_number;
        };

        struct cross_trade_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint32_t shares;
            char stock[9];
            uint32_t cross_price;
            uint64_t matching_number;
            char cross_type;
        };

        struct broken_trade_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t matching_number;
        };

        struct noii_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            uint64_t paired_shares;
            uint64_t imbalance_shares;
            char imbalance_direction;
            char stock[9];
            uint32_t far_price;
            uint32_t near_price;
            uint32_t current_reference_price;
            char cross_type;
            char price_variation_indicator;
        };

        struct rpii_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char stock[9];
            char interest_flag;
        };

        struct luld_auction_collar_message
        {
            char message_type;
            uint16_t stock_locate;
            uint16_t tracking_number;
            uint64_t timestamp;
            char stock[9];
            uint32_t auction_collar_reference_price;
            uint32_t upper_auction_collar_price;
            uint32_t lower_auction_collar_price;
            uint32_t auction_collar_extension;
        };

        struct unknown_message
        {
            char message_type;
        };

        class ITCH_Handler
        {
        public:
            ITCH_Handler() { reset(); }
            ITCH_Handler(const ITCH_Handler &) = delete;
            ITCH_Handler(ITCH_Handler &&) = delete;
            virtual ~ITCH_Handler() = default;
            ITCH_Handler &operator=(const ITCH_Handler &) = delete;
            ITCH_Handler &operator=(ITCH_Handler &&) = delete;

            void reset();
            bool process(void *buffer, size_t size);
            bool process_message(void *buffer, size_t size);
            bool process_without_cache(void *buffer, size_t size);
            size_t get_total_messages();
        private:
            size_t _size;
            std::vector<uint8_t> _cache;
            size_t messages;
            bool process_system_event_message(void *buffer, size_t size);
            bool process_stock_directory_message(void *buffer, size_t size);
            bool process_stock_trading_action_message(void *buffer, size_t size);
            bool process_reg_sho_restriction_message(void *buffer, size_t size);
            bool process_market_participant_position_message(void *buffer, size_t size);
            bool process_mwcb_decline_level_message(void *buffer, size_t size);
            bool process_mwcb_breach_message(void *buffer, size_t size);
            bool process_ipo_quoting_period_update_message(void *buffer, size_t size);
            bool process_add_order_message(void *buffer, size_t size);
            bool process_add_order_mpid_attribution_message(void *buffer, size_t size);
            bool process_order_executed_message(void *buffer, size_t size);
            bool process_order_executed_with_price_message(void *buffer, size_t size);
            bool process_order_cancel_message(void *buffer, size_t size);
            bool process_order_delete_message(void *buffer, size_t size);
            bool process_order_replace_message(void *buffer, size_t size);
            bool process_non_cross_trade_message(void *buffer, size_t size);
            bool process_cross_trade_message(void *buffer, size_t size);
            bool process_broken_trade_message(void *buffer, size_t size);
            bool process_noii_message(void *buffer, size_t size);
            bool process_rpii_message(void *buffer, size_t size);
            bool process_luld_auction_collar_message(void *buffer, size_t size);
            bool process_unknown_message(void *buffer, size_t size);

            template <size_t N>
            size_t read_string(void *buffer, char (&str)[N]);
            size_t read_big_endian(void *buffer, uint16_t &value);
            size_t read_big_endian(void *buffer, uint32_t &value);
            size_t read_big_endian(void *buffer, uint64_t &value);
            size_t read_timestamp(void *buffer, uint64_t &value);
        };
    }
}

#endif