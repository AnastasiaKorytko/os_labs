#pragma once 
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <ostream>

namespace marker_lab {

    const std::size_t WRITE_SLEEP = 5;
    const char ARRAY_SIZE[] = "Enter array size: ";
    const char MARKER_COUNT[] = "Enter number of marker threads: ";

    class MarkerArray {
    public:
        explicit MarkerArray(std::size_t size);
        ~MarkerArray() = default;

        bool try_mark(std::size_t index, int marker_id);
        void clear_marks(int marker_id);
        std::vector<int> copy() const;
        std::size_t size() const noexcept;
        std::size_t count_marked_by(int marker_id) const;
        int get(std::size_t ind) const;

    private:
        std::vector<int> data;
        mutable std::mutex mtx;
    };

    class MarkerController {
    public:
        explicit MarkerController(std::size_t array_size);
        ~MarkerController();

        void start_markers(std::size_t count);
        void signal_start_all();
        void wait_all_waiting();
        bool terminate_marker(std::size_t marker_id);
        void signal_continue_all();
        void print_array(std::ostream& os) const;
        bool has_active_markers() const;
        void join_all();

    private:
        void marker_worker(std::size_t id);
        void zero_out_by(int marker_id);
        std::size_t count_marked_by(int marker_id) const;
        std::shared_ptr<MarkerArray> array;

        struct MarkerInfo {
            std::thread thread;
            std::atomic<bool> waiting{ false };
            std::atomic<bool> terminate{ false };
            std::atomic<bool> alive{ true };
            std::mutex cv_m;
            std::condition_variable cv;
        };
        std::vector<std::unique_ptr<MarkerInfo>> markers;
        mutable std::mutex ctrl_mtx;
        std::condition_variable ctrl_cv;
        std::atomic<bool> started{ false };
        std::mutex start_mtx;
        std::condition_variable start_cv;
    };

}
