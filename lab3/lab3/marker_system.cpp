#include "marker_system.h"
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std::chrono_literals;

namespace marker_lab {

    MarkerArray::MarkerArray(std::size_t size) : data(size, 0) {}

    bool MarkerArray::try_mark(std::size_t ind, int marker_id) {
        std::lock_guard<std::mutex> lk(mtx);
        if (ind >= data.size())
            return false;
        if (data[ind] == 0) {
            data[ind] = marker_id;
            return true;
        }
        return false;
    }

    void MarkerArray::clear_marks(int marker_id) {
        std::lock_guard<std::mutex> lk(mtx);
        for (auto& v : data)
            if (v == marker_id)
                v = 0;
    }

    std::vector<int> MarkerArray::copy() const {
        std::lock_guard<std::mutex> lk(mtx);
        return data;
    }

    std::size_t MarkerArray::size() const noexcept {
        return data.size();
    }

    std::size_t MarkerArray::count_marked_by(int marker_id) const {
        std::lock_guard<std::mutex> lk(mtx);
        std::size_t cnt = 0;
        for (int v : data)
            if (v == marker_id)
                ++cnt;
        return cnt;
    }

    int MarkerArray::get(std::size_t ind) const {
        std::lock_guard<std::mutex> lk(mtx);
        if (ind >= data.size()) 
            return -1;
        return data[ind];
    }

    MarkerController::MarkerController(std::size_t array_size) {
        if (array_size == 0)
            throw std::invalid_argument("array_size must be > 0");
        array = std::make_shared<MarkerArray>(array_size);
    }

    MarkerController::~MarkerController() {
        join_all();
    }

    void MarkerController::start_markers(std::size_t count) {
        if (count == 0) 
            return;
        std::lock_guard<std::mutex> lk(ctrl_mtx);
        markers.clear();
        markers.reserve(count);
        for (std::size_t i = 0; i < count; i++) {
            markers.push_back(std::make_unique<MarkerInfo>());
        }

        for (std::size_t i = 0; i < count; i++) {
            auto& mi = markers[i];
            mi->thread = std::thread(&MarkerController::marker_worker, this, i + 1);
        }
    }

    void MarkerController::signal_start_all() {
        {
            std::lock_guard<std::mutex> lk(start_mtx);
            started.store(true);
        }
        start_cv.notify_all();
    }

    void MarkerController::marker_worker(std::size_t id) {
        auto arr_size = array->size();

        std::uint64_t seed = static_cast<std::uint64_t>(id) ^
            std::chrono::steady_clock::now().time_since_epoch().count();
        std::mt19937 rng(static_cast<unsigned int>(seed));
        std::uniform_int_distribution<int> dist(0, static_cast<int>(arr_size) - 1);

        {
            std::unique_lock<std::mutex> lk(start_mtx);
            start_cv.wait(lk, [&] { return started.load(); });
        }

        while (true) {
            {
                std::lock_guard<std::mutex> lk(ctrl_mtx);
                if (markers[id - 1]->terminate.load()) break;
            }

            std::size_t ind = static_cast<std::size_t>(dist(rng));

            if (array->get(ind) == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(WRITE_SLEEP));
                if (array->try_mark(ind, static_cast<int>(id))) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(WRITE_SLEEP));
                    continue;
                }
            }

            auto marked = array->count_marked_by(static_cast<int>(id));
            {
                std::lock_guard<std::mutex> out_lk(ctrl_mtx);
                std::cout << "Marker " << id
                    << " cannot mark. Marked: " << marked
                    << ", index: " << ind << std::endl;
            }

            markers[id - 1]->waiting.store(true);
            ctrl_cv.notify_one();

            std::unique_lock<std::mutex> lk(markers[id - 1]->cv_m);
            markers[id - 1]->cv.wait(lk, [&] {
                return markers[id - 1]->terminate.load() || !markers[id - 1]->waiting.load();
                });

            if (markers[id - 1]->terminate.load()) 
                break;
        }

        zero_out_by(static_cast<int>(id));
        markers[id - 1]->alive.store(false);
        ctrl_cv.notify_one();
    }

    void MarkerController::wait_all_waiting() {
        std::unique_lock<std::mutex> lk(ctrl_mtx);
        ctrl_cv.wait(lk, [&] {
            std::size_t active = 0, waiting = 0;
            for (auto& m : markers) {
                if (m->alive.load()) {
                    ++active;
                    if (m->waiting.load()) ++waiting;
                }
            }
            return (active > 0) && (waiting == active);
            });
    }

    bool MarkerController::terminate_marker(std::size_t marker_id) {
        if (marker_id == 0 || marker_id > markers.size()) 
            return false;
        {
            std::lock_guard<std::mutex> lk(ctrl_mtx);
            if (!markers[marker_id - 1]->alive.load()) 
                return false;
            markers[marker_id - 1]->terminate.store(true);
            markers[marker_id - 1]->cv.notify_one();
        }

        if (markers[marker_id - 1]->thread.joinable()) {
            markers[marker_id - 1]->thread.join();
        }
        return true;
    }

    void MarkerController::signal_continue_all() {
        std::lock_guard<std::mutex> lk(ctrl_mtx);
        for (auto& m : markers) {
            if (m->waiting.load() && m->alive.load() && !m->terminate.load()) {
                m->waiting.store(false);
                m->cv.notify_one();
            }
        }
    }

    void MarkerController::print_array(std::ostream& os) const {
        auto copy = array->copy();
        for (std::size_t i = 0; i < copy.size(); i++) {
            os << copy[i];
            if (i + 1 < copy.size()) os << ", ";
        }
        os << std::endl;
    }

    bool MarkerController::has_active_markers() const {
        for (auto& m : markers) {
            if (m->alive.load()) return true;
        }
        return false;
    }

    void MarkerController::join_all() {
        std::lock_guard<std::mutex> lk(ctrl_mtx);
        for (auto& m : markers) {
            if (!m) continue;
            m->terminate.store(true);
            m->cv.notify_one();
        }
        for (auto& m : markers) {
            if (m && m->thread.joinable()) m->thread.join();
        }
        markers.clear();
    }

    void MarkerController::zero_out_by(int marker_id) {
        array->clear_marks(marker_id);
    }

    std::size_t MarkerController::count_marked_by(int marker_id) const {
        return array->count_marked_by(marker_id);
    }

} 
