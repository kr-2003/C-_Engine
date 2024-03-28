#include <iostream>
#include <fstream>
#include <cassert>
struct ITCHMessage
{
    char messageType;
};

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

    // Overload << operator to print stock_directory_message
    friend std::ostream &operator<<(std::ostream &os, const stock_directory_message &msg)
    {
        os << "message_type: " << msg.message_type << std::endl;
        os << "stock_locate: " << msg.stock_locate << std::endl;
        os << "tracking_number: " << msg.tracking_number << std::endl;
        os << "timestamp: " << msg.timestamp << std::endl;
        os << "stock: " << msg.stock << std::endl;
        os << "market_category: " << msg.market_category << std::endl;
        os << "financial_status_indicator: " << msg.financial_status_indicator << std::endl;
        os << "round_lot_size: " << msg.round_lot_size << std::endl;
        os << "round_lots_only: " << msg.round_lots_only << std::endl;
        os << "issue_classification: " << msg.issue_classification << std::endl;
        os << "issue_subtype: " << msg.issue_subtype << std::endl;
        os << "authenticity: " << msg.authenticity << std::endl;
        os << "short_sale_threshold_indicator: " << msg.short_sale_threshold_indicator << std::endl;
        os << "ipo_flag: " << msg.ipo_flag << std::endl;
        os << "luld_reference_price_tier: " << msg.luld_reference_price_tier << std::endl;
        os << "etp_flag: " << msg.etp_flag << std::endl;
        os << "etp_leverage_factor: " << msg.etp_leverage_factor << std::endl;
        os << "inverse_indicator: " << msg.inverse_indicator << std::endl;

        return os;
    }
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

    friend std::ostream &operator<<(std::ostream &os, const stock_trading_action_message &msg)
    {
        os << "message_type: " << msg.message_type << std::endl;
        os << "stock_locate: " << msg.stock_locate << std::endl;
        os << "tracking_number: " << msg.tracking_number << std::endl;
        os << "timestamp: " << msg.timestamp << std::endl;
        os << "stock: " << msg.stock << std::endl;
        os << "trading_state : " << msg.trading_state << std::endl;
        os << "reserved : " << msg.reserved << std::endl;
        os << "reason : " << msg.reason << std::endl;

        return os;
    }
};

struct reg_sho_restriction_message
{
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char stock[9];
    char reg_sho_action;

    friend std::ostream &operator<<(std::ostream &os, const reg_sho_restriction_message &msg)
    {
        os << "message_type: " << msg.message_type << std::endl;
        os << "stock_locate: " << msg.stock_locate << std::endl;
        os << "tracking_number: " << msg.tracking_number << std::endl;
        os << "timestamp: " << msg.timestamp << std::endl;
        os << "stock: " << msg.stock << std::endl;
        os << "reg sho action: " << msg.reg_sho_action << std::endl;

        return os;
    }
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
    
    friend std::ostream &operator<<(std::ostream &os, const market_participant_position_message &msg)
    {
        os << "message_type: " << msg.message_type << std::endl;
        os << "stock_locate: " << msg.stock_locate << std::endl;
        os << "tracking_number: " << msg.tracking_number << std::endl;
        os << "timestamp: " << msg.timestamp << std::endl;
        os << "mpid: " << msg.mpid << std::endl;
        os << "stock: " << msg.stock << std::endl;
        os << "primary_market_manager: " << msg.primary_market_manager << std::endl;
        os << "market_manager_mode: " << msg.market_maker_mode << std::endl;
        os << "market_participant_state: " << msg.market_participant_state << std::endl;

        return os;
    }
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

struct unknown_message {
    char message_type;
};

size_t read_big_endian(const void *buffer, uint16_t &value)
{
    ((uint8_t *)&value)[0] = ((const uint8_t *)buffer)[1];
    ((uint8_t *)&value)[1] = ((const uint8_t *)buffer)[0];
    return 2;
}

size_t read_big_endian(const void *buffer, uint32_t &value)
{
    ((uint8_t *)&value)[0] = ((const uint8_t *)buffer)[3];
    ((uint8_t *)&value)[1] = ((const uint8_t *)buffer)[2];
    ((uint8_t *)&value)[2] = ((const uint8_t *)buffer)[1];
    ((uint8_t *)&value)[3] = ((const uint8_t *)buffer)[0];
    return 4;
}

size_t read_big_endian(const void *buffer, uint64_t &value)
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

size_t read_timestamp(const void *buffer, uint64_t &value)
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
size_t read_string(const void *buffer, char (&str)[N])
{
    // std::cout << N << std::endl;
    std::memcpy(str, buffer, N - 1); 
    str[N - 1] = '\0';  
    return N - 1;  
}

bool process_system_event_message(void *buffer, size_t size)
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
    return true;
}

bool process_stock_directory_message(void *buffer, size_t size)
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
    return true;
}

bool process_stock_trading_action_message(void* buffer, size_t size) {
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
    return true;
}

bool process_reg_sho_restriction_message(void* buffer, size_t size) {
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
    return true;
}

bool process_market_participant_position_message(void* buffer, size_t size) {
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
    return true;
}

bool process_mwcb_decline_level_message(const void* buffer, size_t size) {
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
    return true;
}

bool process_mwcb_breach_message(const void* buffer, size_t size) {
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
    return true;
}

bool process_ipo_quoting_period_update_message(const void* buffer, size_t size) {
    assert((size == 28) && "Invalid size of ipo quoting period update msg");
    if(size != 28) 
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
    return true;
}

bool process_add_order_message(const void* buffer, size_t size) {
    assert((size == 36) && "Invalid size of add order message");
    if(size != 36) 
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
    return true;
}

bool process_add_order_mpid_attribution_message(const void* buffer, size_t size) {
    assert((size == 40) && "invalid size of add order mpid message");
    if(size != 40)
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
    return true;
} 

bool process_order_executed_message(const void *buffer, size_t size) {
    assert((size == 31) && "invalid size of order executed message");
    if(size != 31) 
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
    return true;
}

bool process_order_executed_with_price_message(const void* buffer, size_t size) {
    assert((size == 36) && "invalid size of order ex message");
    if(size != 36)
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
    return true;
}

bool process_order_cancel_message(const void* buffer, size_t size) {
    assert((size == 23) && "invalid order cancel msg");
    if(size != 23)
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
    return true;
}

bool process_order_delete_message(const void* buffer, size_t size) {
    assert((size == 19) && "invalid order delete msg");
    if(size != 19)
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
    return true;
}

bool process_order_replace_message(const void* buffer, size_t size) {
    assert((size == 35) && "invalid order replace msg");
    if(size != 35)
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
    return true;
}

bool process_non_cross_trade_message(const void* buffer, size_t size) {
    assert((size == 44) && "invalid non cross trade msg");
    if(size != 44)
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
    return true;
}

bool process_cross_trade_message(const void* buffer, size_t size) {
    assert((size == 40) && "invalid cross trade message");
    if(size != 40)
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
    return true;
}

bool process_broken_trade_message(const void* buffer, size_t size) {
    assert((size == 19) && "invalid broken trade message");
    if(size != 19)
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
    return true;
}

bool process_noii_message(const void* buffer, size_t size) {
    assert((size == 50) && "invalid noii message");
    if(size != 50)
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
    return true;
}

bool process_rpii_message(const void* buffer, size_t size) {
    assert((size == 20) && "invalid rpii message");
    if(size != 20)
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
    return true;
}

bool process_luld_auction_collar_message(void* buffer, size_t size) {
    assert((size == 35) && "invalid luld auction collar msg");
    if(size != 35)
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
    return true;
}

bool process_unknown_message(const void* buffer, size_t size) {
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
bool process_message(void *buffer, size_t size)
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
        // std::cout << "ProcessStockDirectoryMessage(data, size)" << std::endl;
        return process_stock_directory_message(data, size);
    case 'H':
        // std::cout << "ProcessStockTradingActionMessage(data, size)" << std::endl;
        return process_stock_trading_action_message(data, size);
    case 'Y':
        // std::cout << "ProcessRegSHOMessage(data, size)" << std::endl;
        return process_reg_sho_restriction_message(data, size);
    case 'L':
        // std::cout << "ProcessMarketParticipantPositionMessage(data, size)" << std::endl;
        return process_market_participant_position_message(data, size);
    case 'V':
        // std::cout << "ProcessMWCBDeclineMessage(data, size)" << std::endl;
        return process_mwcb_decline_level_message(data, size);
    case 'W':
        // std::cout << "ProcessMWCBStatusMessage(data, size)" << std::endl;
        return process_mwcb_breach_message(data, size);
    case 'K':
        // std::cout << "ProcessIPOQuotingMessage(data, size)" << std::endl;

        return process_ipo_quoting_period_update_message(data, size);
    case 'A':
        // std::cout << "ProcessAddOrderMessage(data, size)" << std::endl;
        return process_add_order_message(data, size);
    case 'F':
        // std::cout << "ProcessAddOrderMPIDMessage(data, size)" << std::endl;
        return process_add_order_mpid_attribution_message(data, size);
    case 'E':
        // std::cout << "ProcessOrderExecutedMessage(data, size)" << std::endl;
        return process_order_executed_message(data, size);
    case 'C':
        // std::cout << "ProcessOrderExecutedWithPriceMessage(data, size)" << std::endl;
        return process_order_executed_with_price_message(data, size);
    case 'X':
        // std::cout << "ProcessOrderCancelMessage(data, size)" << std::endl;
        return process_order_cancel_message(data, size);
    case 'D':
        // std::cout << "ProcessOrderDeleteMessage(data, size)" << std::endl;
        return process_order_delete_message(data, size);
    case 'U':
        // std::cout << "ProcessOrderReplaceMessage(data, size)" << std::endl;
        return process_order_replace_message(data, size);
    case 'P':
        // std::cout << "ProcessTradeMessage(data, size)" << std::endl;
        return process_non_cross_trade_message(data, size);
    case 'Q':
        // std::cout << "ProcessCrossTradeMessage(data, size)" << std::endl;
        return process_cross_trade_message(data, size);
    case 'B':
        // std::cout << "ProcessBrokenTradeMessage(data, size)" << std::endl;
        return process_broken_trade_message(data, size);
    case 'I':
        // std::cout << "ProcessNOIIMessage(data, size)" << std::endl;
        return process_noii_message(data, size);
    case 'N':
        // std::cout << "ProcessRPIIMessage(data, size)" << std::endl;
        return process_rpii_message(data, size);
    case 'J':
        // std::cout << "ProcessLULDAuctionCollarMessage(data, size)" << std::endl;
        return process_luld_auction_collar_message(data, size);
    default:
        // std::cout << "ProcessUnknownMessage(data, size);" << std::endl;
        return process_unknown_message(data, size);
    }
}

bool process(void *buffer, size_t size)
{
    size_t index = 0;
    uint8_t *data = (uint8_t *)buffer;

    while (index < size)
    {
        // Read message size
        uint16_t message_size;
        index += read_big_endian(&data[index], message_size);
        // std::cout << message_size << std::endl;
        // Check remaining buffer size
        if (index + message_size > size)
        {
            // Incomplete message, return false
            return false;
        }

        // Process message
        if (!process_message(&data[index], message_size))
        {
            // Processing failed, return false
            return false;
        }

        // Move to next message
        index += message_size;
    }

    return true;
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    std::ifstream file("../data/itch/sample.itch", std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }
    file.seekg(0, std::ios::end);
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate memory for the buffer
    void *buffer = malloc(file_size);
    if (!file.read(static_cast<char *>(buffer), file_size))
    {
        std::cerr << "Failed to read file!" << std::endl;
        free(buffer); // Free allocated memory
        return 1;
    }
    file.close();
    // std::cout << file_size << std::endl;
    uint8_t *data = (uint8_t *)buffer;
    // std::cout << (*data) << std::endl;
    process(buffer, file_size);

    // Don't forget to free the allocated memory
    free(buffer);
    return 0;
}