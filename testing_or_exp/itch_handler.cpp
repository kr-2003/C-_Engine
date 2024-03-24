#include <iostream>
#include <fstream>

struct ITCHMessage
{
    char messageType;
};

struct system_event_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char event_code;
};

struct stock_directory_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char stock[8];
    char market_category;
    char financial_status_indicator;
    uint16_t round_lot_size;
    char round_lots_only;
    char issue_classification;
    char issue_subtype[2];
    char authenticity;
    char short_sale_threshold_indicator;
    char ipo_flag;
    char luld_reference_price_tier;
    char etp_flag;
    uint16_t etp_leverage_factor;
    char inverse_indicator;
};

struct stock_trading_action_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char stock[8];
    char trading_state;
    char reserved;
    char reason[4];
};

struct reg_sho_restriction_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char stock[8];
    char reg_sho_action;
};

struct market_participant_position_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char mpid[4];
    char stock[8];
    char primary_market_manager;
    char market_maker_mode;
    char market_participant_state;
};

struct mwcb_decline_level_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t level_1;
    uint64_t level_2;
    uint64_t level_3;
};


struct mwcb_status_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char breached_level;
};

struct ipo_quoted_period_update {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char stock[8];
    uint32_t ipo_quotation_release_time;
    char ipo_quotation_release_qualifier;
    uint32_t ipo_price;
};


struct add_order_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    char buy_sell_indicator;
    uint32_t shares;
    char stock[8];
    uint32_t price;
};

struct add_order_with_mpid_attribution_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    char buy_sell_indicator;
    uint32_t shares;
    char stock[8];
    uint32_t price;
    char attribution[4];
};

struct order_executed_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t executed_shares;
    uint64_t match_number;
};

struct order_executed_with_price_message {
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

struct order_cancel_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t canceled_shares;
};

struct order_delete_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
};

struct order_replace_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint64_t new_order_reference_number;
    uint32_t shares;
    uint32_t price;
};

struct non_cross_trade_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    char buy_sell_indicator;
    uint32_t shares;
    char stock[8];
    uint32_t price;
    uint64_t matching_number;
};

struct cross_trade_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint32_t shares;
    char stock[8];
    uint32_t cross_price;
    uint64_t matching_number;
    char cross_type;
};

struct broken_trade_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t matching_number;
};

struct noii_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t paired_shares;
    uint64_t imbalance_shares;
    char imbalance_direction;
    char stock[8];
    uint32_t far_price;
    uint32_t near_price;
    uint32_t current_reference_price;
    char cross_type;
    char price_variation_indicator;
};

struct rpii_message {
    char message_type;
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char stock[8];
    char interest_flag;
}

void process_itch_messages(std::vector<char> &buffer)
{
    size_t offset = 0;
    while (offset < buffer.size())
    {
        ITCHMessage message;
        message.messageType = buffer[offset];
        offset += sizeof(char);
        switch (message.messageType)
        {
        case 'S':
            std::cout << "Processing System Event Message!" << std::endl;
            break;
        case 'R':
            std::cout << "ProcessStockDirectoryMessage(data, size)" << std::endl;
            break;
        case 'H':
            std::cout << "ProcessStockTradingActionMessage(data, size)" << std::endl;
            break;
        case 'Y':
            std::cout << "ProcessRegSHOMessage(data, size)" << std::endl;
            break;
        case 'L':
            std::cout << "ProcessMarketParticipantPositionMessage(data, size)" << std::endl;
            break;
        case 'V':
            std::cout << "ProcessMWCBDeclineMessage(data, size)" << std::endl;
            break;
        case 'W':
            std::cout << "ProcessMWCBStatusMessage(data, size)" << std::endl;
            break;
        case 'K':
            std::cout << "ProcessIPOQuotingMessage(data, size)" << std::endl;
        case 'A':
            std::cout << "ProcessAddOrderMessage(data, size)" << std::endl;
            break;
        case 'F':
            std::cout << "ProcessAddOrderMPIDMessage(data, size)" << std::endl;
            break;
        case 'E':
            std::cout << "ProcessOrderExecutedMessage(data, size)" << std::endl;
            break;
        case 'C':
            std::cout << "ProcessOrderExecutedWithPriceMessage(data, size)" << std::endl;
            break;
        case 'X':
            std::cout << "ProcessOrderCancelMessage(data, size)" << std::endl;
            break;
        case 'D':
            std::cout << "ProcessOrderDeleteMessage(data, size)" << std::endl;
            break;
        case 'U':
            std::cout << "ProcessOrderReplaceMessage(data, size)" << std::endl;
            break;
        case 'P':
            std::cout << "ProcessTradeMessage(data, size)" << std::endl;
            break;
        case 'Q':
            std::cout << "ProcessCrossTradeMessage(data, size)" << std::endl;
            break;
        case 'B':
            std::cout << "ProcessBrokenTradeMessage(data, size)" << std::endl;
            break;
        case 'I':
            std::cout << "ProcessNOIIMessage(data, size)" << std::endl;
            break;
        case 'N':
            std::cout << "ProcessRPIIMessage(data, size)" << std::endl;
            break;
        case 'J':
            std::cout << "ProcessLULDAuctionCollarMessage(data, size)" << std::endl;
            break;
        default:
            std::cout << "ProcessUnknownMessage(data, size);" << std::endl;
            break;
        }
    }
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

    std::vector<char> buffer(file_size);
    if (!file.read(buffer.data(), file_size))
    {
        std::cerr << "Failed to read file!" << std::endl;
        return 1;
    }
    file.close();
    process_itch_messages(buffer);
    return 0;
}