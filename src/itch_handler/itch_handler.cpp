#include <iostream>
#include <fstream>
#include <cassert>
#include <sys/time.h>
#include <vector>
#include <cstring>
#include <iomanip>
#include "itch_handler.h"
#include <_stdio.h>

namespace TradingEngine
{
    namespace ITCH
    {
        size_t ITCH_Handler::get_total_messages() 
        {
            return messages;
        }
        void ITCH_Handler::reset() {
            _size = 0;
            _cache.clear();
        }
        bool ITCH_Handler::process(void *buffer, size_t size)
        {
            size_t index = 0;
            uint8_t *data = (uint8_t *)buffer;

            while (index < size)
            {
                // std::cout << index << std::endl;
                if (_size == 0)
                {
                    size_t remaining = size - index;
                    if (((_cache.size() == 0) && (remaining < 3)) || (_cache.size() == 1))
                    {
                        _cache.push_back(data[index++]);
                        continue;
                    }
                    uint16_t message_size;
                    if (_cache.empty())
                    {
                        index += read_big_endian(&data[index], message_size);
                    }
                    else
                    {
                        read_big_endian(_cache.data(), message_size);
                        _cache.clear();
                    }
                    _size = message_size;
                }
                if (_size > 0)
                {
                    size_t remaining = size - index;
                    if (!_cache.empty())
                    {
                        size_t tail = _size - _cache.size();
                        if (tail > remaining)
                        {
                            tail = remaining;
                        }
                        _cache.insert(_cache.end(), &data[index], &data[index + tail]);
                        index += tail;
                        if (_cache.size() < _size)
                        {
                            continue;
                        }
                    }
                    else if (_size > remaining)
                    {
                        _cache.reserve(_size);
                        _cache.insert(_cache.end(), &data[index], &data[index + remaining]);
                        index += remaining;
                        continue;
                    }
                    if (_cache.empty())
                    {
                        if (!process_message(&data[index], _size))
                            return false;
                        index += _size;
                    }
                    else
                    {
                        if (!process_message(_cache.data(), _size))
                            return false;
                        _cache.clear();
                    }
                    _size = 0;
                }
            }
            return true;
        }
        bool ITCH_Handler::process_without_cache(void *buffer, size_t size) 
        {
            size_t index = 0;
            uint8_t *data = (uint8_t *)buffer;

            while (index < size)
            {
                uint16_t message_size;
                index += read_big_endian(&data[index], message_size);
                if (index + message_size > size)
                {
                    return false;
                }

                // Process message
                if (!process_message(&data[index], message_size))
                {
                    return false;
                }
                index += message_size;
            }

            return true;
        }
        bool ITCH_Handler::process_message(void *buffer, size_t size) 
        {
            if (size == 0)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            switch (*data)
            {
            case 'S':
                return process_system_event_message(data, size);
            case 'R':
                return process_stock_directory_message(data, size);
            case 'H':
                return process_stock_trading_action_message(data, size);
            case 'Y':
                return process_reg_sho_restriction_message(data, size);
            case 'L':
                return process_market_participant_position_message(data, size);
            case 'V':
                return process_mwcb_decline_level_message(data, size);
            case 'W':
                return process_mwcb_breach_message(data, size);
            case 'K':
                return process_ipo_quoting_period_update_message(data, size);
            case 'A':
                return process_add_order_message(data, size);
            case 'F':
                return process_add_order_mpid_attribution_message(data, size);
            case 'E':
                return process_order_executed_message(data, size);
            case 'C':
                return process_order_executed_with_price_message(data, size);
            case 'X':
                return process_order_cancel_message(data, size);
            case 'D':
                return process_order_delete_message(data, size);
            case 'U':
                return process_order_replace_message(data, size);
            case 'P':
                return process_non_cross_trade_message(data, size);
            case 'Q':
                return process_cross_trade_message(data, size);
            case 'B':
                return process_broken_trade_message(data, size);
            case 'I':
                return process_noii_message(data, size);
            case 'N':
                return process_rpii_message(data, size);
            case 'J':
                return process_luld_auction_collar_message(data, size);
            default:
                return process_unknown_message(data, size);
            }
        }
        bool ITCH_Handler::process_system_event_message(void *buffer, size_t size)
        {
            assert((size == 12) && "Invalid size of System Event Message!");
            if (size != 12)
            {
                return false;
            }
            system_event_message message;
            uint8_t *data = (uint8_t *)buffer;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            message.event_code = *data++;
            // std::cout << "Message type: " << message.message_type << std::endl;
            // std::cout << "stock locate: " << message.stock_locate << std::endl;
            // std::cout << "tracking_number: " << message.tracking_number << std::endl;
            // std::cout << "timestamp: " << message.timestamp << std::endl;
            // std::cout << "event_code: " << message.event_code << std::endl;
            // exit(0);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_stock_directory_message(void *buffer, size_t size)
        {
            assert((size == 39) && "Invalid size of Stock Directory Message!");
            if (size != 39)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            stock_directory_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_string(data, message.stock);
            message.market_category = *data++;
            message.financial_status_indicator = *data++;
            data += read_big_endian(data, message.round_lot_size);
            message.round_lots_only = *data++;
            message.issue_classification = *data++;
            data += read_string(data, message.issue_subtype);
            message.authenticity = *data++;
            message.short_sale_threshold_indicator = *data++;
            message.ipo_flag = *data++;
            message.luld_reference_price_tier = *data++;
            message.etp_flag = *data++;
            data += read_big_endian(data, message.etp_leverage_factor);
            message.inverse_indicator = *data++;
            messages++;
            return true;
        }

        bool ITCH_Handler::process_stock_trading_action_message(void *buffer, size_t size)
        {
            assert((size == 25) && "Invalid size of Stock Trading Action Message!");
            if (size != 25)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            stock_trading_action_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_string(data, message.stock);
            message.trading_state = *data++;
            message.reserved = *data++;
            data += read_string(data, message.reason);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_reg_sho_restriction_message(void *buffer, size_t size)
        {
            assert((size == 20) && "Invalid size of Reg Sho Restriction Message!");
            if (size != 20)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            reg_sho_restriction_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_string(data, message.stock);
            message.reg_sho_action = *data++;
            messages++;
            return true;
        }

        bool ITCH_Handler::process_market_participant_position_message(void *buffer, size_t size)
        {
            assert((size == 26) && "Invalid size of Market Participant Position Message!");
            if (size != 26)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            market_participant_position_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_string(data, message.mpid);
            data += read_string(data, message.stock);
            message.primary_market_manager = *data++;
            message.market_maker_mode = *data++;
            message.market_participant_state = *data++;
            messages++;
            return true;
        }

        bool ITCH_Handler::process_mwcb_decline_level_message(void *buffer, size_t size)
        {
            assert((size == 35) && "Invalid size of mwcb Message!");
            if (size != 35)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            mwcb_decline_level_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.level_1);
            data += read_big_endian(data, message.level_2);
            data += read_big_endian(data, message.level_3);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_mwcb_breach_message(void *buffer, size_t size)
        {
            assert((size == 12) && "Invalid size of mwcb breach Message!");
            if (size != 12)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            mwcb_status_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            message.breached_level = *data++;
            messages++;
            return true;
        }

        bool ITCH_Handler::process_ipo_quoting_period_update_message(void *buffer, size_t size)
        {
            assert((size == 28) && "Invalid size of ipo quoting period update msg");
            if (size != 28)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            ipo_quoted_period_update message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_string(data, message.stock);
            data += read_big_endian(data, message.ipo_quotation_release_time);
            message.ipo_quotation_release_qualifier = *data++;
            data += read_big_endian(data, message.ipo_price);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_add_order_message(void *buffer, size_t size)
        {
            assert((size == 36) && "Invalid size of add order message");
            if (size != 36)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            add_order_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.order_reference_number);
            message.buy_sell_indicator = *data++;
            data += read_big_endian(data, message.shares);
            data += read_string(data, message.stock);
            data += read_big_endian(data, message.price);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_add_order_mpid_attribution_message(void *buffer, size_t size)
        {
            assert((size == 40) && "invalid size of add order mpid message");
            if (size != 40)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            add_order_with_mpid_attribution_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.order_reference_number);
            message.buy_sell_indicator = *data++;
            data += read_big_endian(data, message.shares);
            data += read_string(data, message.stock);
            data += read_big_endian(data, message.price);
            data += read_string(data, message.attribution);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_order_executed_message(void *buffer, size_t size)
        {
            assert((size == 31) && "invalid size of order executed message");
            if (size != 31)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            order_executed_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.order_reference_number);
            data += read_big_endian(data, message.executed_shares);
            data += read_big_endian(data, message.match_number);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_order_executed_with_price_message(void *buffer, size_t size)
        {
            assert((size == 36) && "invalid size of order ex message");
            if (size != 36)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            order_executed_with_price_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.order_reference_number);
            data += read_big_endian(data, message.executed_shares);
            data += read_big_endian(data, message.match_number);
            message.printable = *data++;
            data += read_big_endian(data, message.execution_price);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_order_cancel_message(void *buffer, size_t size)
        {
            assert((size == 23) && "invalid order cancel msg");
            if (size != 23)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            order_cancel_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.order_reference_number);
            data += read_big_endian(data, message.canceled_shares);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_order_delete_message(void *buffer, size_t size)
        {
            assert((size == 19) && "invalid order delete msg");
            if (size != 19)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            order_delete_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.order_reference_number);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_order_replace_message(void *buffer, size_t size)
        {
            assert((size == 35) && "invalid order replace msg");
            if (size != 35)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            order_replace_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.order_reference_number);
            data += read_big_endian(data, message.new_order_reference_number);
            data += read_big_endian(data, message.shares);
            data += read_big_endian(data, message.price);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_non_cross_trade_message(void *buffer, size_t size)
        {
            assert((size == 44) && "invalid non cross trade msg");
            if (size != 44)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            non_cross_trade_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.order_reference_number);
            message.buy_sell_indicator = *data++;
            data += read_big_endian(data, message.shares);
            data += read_string(data, message.stock);
            data += read_big_endian(data, message.price);
            data += read_big_endian(data, message.matching_number);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_cross_trade_message(void *buffer, size_t size)
        {
            assert((size == 40) && "invalid cross trade message");
            if (size != 40)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            cross_trade_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.shares);
            data += read_string(data, message.stock);
            data += read_big_endian(data, message.cross_price);
            data += read_big_endian(data, message.matching_number);
            message.cross_type = *data++;
            messages++;
            return true;
        }

        bool ITCH_Handler::process_broken_trade_message(void *buffer, size_t size)
        {
            assert((size == 19) && "invalid broken trade message");
            if (size != 19)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            broken_trade_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_big_endian(data, message.matching_number);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_noii_message(void *buffer, size_t size)
        {
            assert((size == 50) && "invalid noii message");
            if (size != 50)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            noii_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_timestamp(data, message.paired_shares);
            data += read_timestamp(data, message.imbalance_shares);
            message.imbalance_direction = *data++;
            data += read_string(data, message.stock);
            data += read_big_endian(data, message.far_price);
            data += read_big_endian(data, message.near_price);
            data += read_big_endian(data, message.current_reference_price);
            message.cross_type = *data++;
            message.price_variation_indicator = *data++;
            messages++;
            return true;
        }

        bool ITCH_Handler::process_rpii_message(void *buffer, size_t size)
        {
            assert((size == 20) && "invalid rpii message");
            if (size != 20)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            rpii_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_string(data, message.stock);
            message.interest_flag = *data++;
            messages++;
            return true;
        }

        bool ITCH_Handler::process_luld_auction_collar_message(void *buffer, size_t size)
        {
            assert((size == 35) && "invalid luld auction collar msg");
            if (size != 35)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            luld_auction_collar_message message;
            message.message_type = *data++;
            data += read_big_endian(data, message.stock_locate);
            data += read_big_endian(data, message.tracking_number);
            data += read_timestamp(data, message.timestamp);
            data += read_string(data, message.stock);
            data += read_big_endian(data, message.auction_collar_reference_price);
            data += read_big_endian(data, message.upper_auction_collar_price);
            data += read_big_endian(data, message.lower_auction_collar_price);
            data += read_big_endian(data, message.auction_collar_extension);
            messages++;
            return true;
        }

        bool ITCH_Handler::process_unknown_message(void *buffer, size_t size)
        {
            assert((size > 0) && "Invalid size of the unknown ITCH message!");
            if (size == 0)
            {
                return false;
            }
            uint8_t *data = (uint8_t *)buffer;
            unknown_message message;
            message.message_type = *data;
            return true;
        }

        size_t ITCH_Handler::read_big_endian(void *buffer, uint16_t &value)
        {
            ((uint8_t *)&value)[0] = ((const uint8_t *)buffer)[1];
            ((uint8_t *)&value)[1] = ((const uint8_t *)buffer)[0];
            return 2;
        }

        size_t ITCH_Handler::read_big_endian(void *buffer, uint32_t &value)
        {
            ((uint8_t *)&value)[0] = ((const uint8_t *)buffer)[3];
            ((uint8_t *)&value)[1] = ((const uint8_t *)buffer)[2];
            ((uint8_t *)&value)[2] = ((const uint8_t *)buffer)[1];
            ((uint8_t *)&value)[3] = ((const uint8_t *)buffer)[0];
            return 4;
        }

        size_t ITCH_Handler::read_big_endian(void *buffer, uint64_t &value)
        {
            ((uint8_t *)&value)[0] = ((const uint8_t *)buffer)[7];
            ((uint8_t *)&value)[1] = ((const uint8_t *)buffer)[6];
            ((uint8_t *)&value)[2] = ((const uint8_t *)buffer)[5];
            ((uint8_t *)&value)[3] = ((const uint8_t *)buffer)[4];
            ((uint8_t *)&value)[4] = ((const uint8_t *)buffer)[3];
            ((uint8_t *)&value)[5] = ((const uint8_t *)buffer)[2];
            ((uint8_t *)&value)[6] = ((const uint8_t *)buffer)[1];
            ((uint8_t *)&value)[7] = ((const uint8_t *)buffer)[0];
            return 8;
        }

        size_t ITCH_Handler::read_timestamp(void *buffer, uint64_t &value)
        {
            ((uint8_t *)&value)[0] = ((const uint8_t *)buffer)[2];
            ((uint8_t *)&value)[1] = ((const uint8_t *)buffer)[1];
            ((uint8_t *)&value)[2] = ((const uint8_t *)buffer)[0];
            ((uint8_t *)&value)[3] = 0;
            ((uint8_t *)&value)[4] = 0;
            ((uint8_t *)&value)[5] = 0;
            ((uint8_t *)&value)[6] = 0;
            ((uint8_t *)&value)[7] = 0;
            return 6;
        }

        template <size_t N>
        size_t ITCH_Handler::read_string(void *buffer, char (&str)[N])
        {
            // std::cout << N << std::endl;
            std::memcpy(str, buffer, N - 1);
            str[N - 1] = '\0';
            return N - 1;
        }
    }

}

