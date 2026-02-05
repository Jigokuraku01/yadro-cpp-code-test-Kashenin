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
        cur_repo.free_table_from_user(get_user_name(), get_time());
    }

    if (cur_repo.is_user_waiting(get_user_name())) {
        // Я могу глубоко ошибаться, но вроде сюда никогда не зайдёт код просто из логики продукта
        // Это случай, когда мы хотим посадить человека за стол, но он в очереди на ожидание
        // Если это был свободный стол, то была бы ошибка что есть очередь с пустым столом
        // Если за этом столом кто-то сидел, то на этот стол должен сесть первый человек в очереди
        // Без этого все операции делаются за O(log n) из-за того, что я использую priority_queue для очереди ожидания и set, map вместо их unordered версий
        cur_repo.remove_waiting_user_by_name(get_user_name());
    }
    cur_repo.add_user_table(get_user_name(), get_table_id());
    TableInfo& table_info = cur_repo.get_tables().at(get_table_id());
    table_info.set_user_name(get_user_name());
    cur_repo.mark_table_occupied(get_table_id());
    table_info.set_last_occupied_start_time(get_time());
}

void Type3RowInfo::do_step(Repository& cur_repo) {
    cur_repo.add_history_entry(std::make_shared<Type3RowInfo>(*this));

    if (!cur_repo.is_user_currently_in(get_user_name())) {
        cur_repo.add_history_entry(
            IInputRowInfo::generate_exception_row(get_time(), "ClientUnknown"));
        return;
    }
    if (cur_repo.has_free_tables()) {
        cur_repo.add_history_entry(IInputRowInfo::generate_exception_row(
            get_time(), "ICanWaitNoLonger!"));
        return;
    }

    if (cur_repo.has_user_table(get_user_name())) {
        //Вот тут я не знаю что делать и какую ошибку кидать.
        // Наверное логично было бы ICanWaitNoLonger, но это вроде не специфицировано
        // Поэтому я буду выдавать ошибку ClientAlreadyHasTable
        cur_repo.add_history_entry(IInputRowInfo::generate_exception_row(
            get_time(), "ClientAlreadyHasTable"));
        return;
    }
    if (cur_repo.is_queue_full()) {
        cur_repo.add_history_entry(
            std::make_shared<Type11RowInfo>(get_time(), get_user_name()));
        cur_repo.remove_current_user(get_user_name());

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
    cur_repo.free_table_from_user(get_user_name(), get_time());
    cur_repo.remove_current_user(get_user_name());

    if (cur_repo.has_waiting_users()) {
        std::string next_user = cur_repo.get_next_waiting_user();
        cur_repo.remove_waiting_user();
        cur_repo.add_user_table(next_user, table_id);
        table_info.set_user_name(next_user);
        cur_repo.mark_table_occupied(table_id);
        table_info.set_last_occupied_start_time(get_time());

        cur_repo.add_history_entry(
            std::make_shared<Type12RowInfo>(get_time(), next_user, table_id));
    }
    cur_repo.remove_current_user(get_user_name());
}
