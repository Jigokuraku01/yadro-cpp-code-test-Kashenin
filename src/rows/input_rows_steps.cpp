#include "input_rows.hpp"

#include "output_rows.hpp"
#include "repository.hpp"
#include <memory>

void Type1RowInfo::do_step(Repository& cur_repo) {
    cur_repo.add_history_entry(std::make_shared<Type1RowInfo>(*this));
    if (cur_repo.is_user_currently_in(get_user_name())) {
        cur_repo.add_history_entry(IInputRowInfo::generate_exception_row(
            get_time(), "YouShallNotPass"));
        return;
    }

    if (get_time() < cur_repo.get_start_info().get_start_time() ||
        get_time() >= cur_repo.get_start_info().get_end_time()) {
        cur_repo.add_history_entry(
            IInputRowInfo::generate_exception_row(get_time(), "NotOpenYet"));
        return;
    }

    cur_repo.add_current_user(get_user_name());
}

void Type2RowInfo::do_step(Repository& cur_repo) {
    cur_repo.add_history_entry(std::make_shared<Type2RowInfo>(*this));
    if (!cur_repo.is_user_currently_in(get_user_name())) {
        cur_repo.add_history_entry(
            IInputRowInfo::generate_exception_row(get_time(), "ClientUnknown"));
        return;
    }

    if (!cur_repo.get_tables().contains(get_table_id())) {
        cur_repo.add_history_entry(
            IInputRowInfo::generate_exception_row(get_time(), "UnknownTable"));
        return;
    }
    if (!cur_repo.is_table_free(get_table_id())) {
        cur_repo.add_history_entry(
            IInputRowInfo::generate_exception_row(get_time(), "PlaceIsBusy"));
        return;
    }

    if (cur_repo.has_user_table(get_user_name())) {
        std::uint32_t table_id = cur_repo.get_user_table_id(get_user_name());
        TableInfo& table_info = cur_repo.get_tables().at(table_id);
        table_info.set_occupied(false);
        std::uint32_t occupied_time =
            get_time() - table_info.get_last_occupied_start_time();
        table_info.add_occupied_time(occupied_time);
        table_info.add_money_spent(
            cur_repo.calculate_total_price(occupied_time));
        cur_repo.remove_user_table(get_user_name());
    }

    if (cur_repo.is_user_waiting(get_user_name())) {
        cur_repo.remove_waiting_user_by_name(get_user_name());
    }
    cur_repo.add_user_table(get_user_name(), get_table_id());
    TableInfo& table_info = cur_repo.get_tables().at(get_table_id());
    table_info.set_user_name(get_user_name());
    table_info.set_occupied(true);
    table_info.set_last_occupied_start_time(get_time());
}

void Type3RowInfo::do_step(Repository& cur_repo) {
    cur_repo.add_history_entry(std::make_shared<Type3RowInfo>(*this));

    if (!cur_repo.is_user_currently_in(get_user_name())) {
        cur_repo.add_history_entry(
            IInputRowInfo::generate_exception_row(get_time(), "ClientUnknown"));
        return;
    }
    if (cur_repo.has_free_tables() ||
        cur_repo.has_user_table(get_user_name())) {
        cur_repo.add_history_entry(IInputRowInfo::generate_exception_row(
            get_time(), "ICanWaitNoLonger!"));
        return;
    }
    if (cur_repo.is_queue_full()) {
        cur_repo.add_history_entry(
            std::make_shared<Type11RowInfo>(get_time(), get_user_name()));
        return;
    }
    if (!cur_repo.is_user_waiting(get_user_name())) {
        cur_repo.add_waiting_user(get_user_name());
    }
}

void Type4RowInfo::do_step(Repository& cur_repo) {
    cur_repo.add_history_entry(std::make_shared<Type4RowInfo>(*this));

    if (!cur_repo.is_user_currently_in(get_user_name())) {
        cur_repo.add_history_entry(
            IInputRowInfo::generate_exception_row(get_time(), "ClientUnknown"));
        return;
    }
    if (!cur_repo.has_user_table(get_user_name())) {
        cur_repo.add_history_entry(IInputRowInfo::generate_exception_row(
            get_time(), "ClientNotAtTable"));
        return;
    }

    std::uint32_t table_id = cur_repo.get_user_table_id(get_user_name());
    TableInfo& table_info = cur_repo.get_tables().at(table_id);
    table_info.set_occupied(false);
    std::uint32_t occupied_time =
        get_time() - table_info.get_last_occupied_start_time();
    table_info.add_occupied_time(occupied_time);
    table_info.add_money_spent(cur_repo.calculate_total_price(occupied_time));
    cur_repo.remove_user_table(get_user_name());

    if (cur_repo.has_waiting_users()) {
        std::string next_user = cur_repo.get_next_waiting_user();
        cur_repo.remove_waiting_user();
        cur_repo.add_user_table(next_user, table_id);
        table_info.set_user_name(next_user);
        table_info.set_occupied(true);
        table_info.set_last_occupied_start_time(get_time());

        cur_repo.add_history_entry(
            std::make_shared<Type12RowInfo>(get_time(), next_user, table_id));
    }
    cur_repo.remove_current_user(get_user_name());
}
