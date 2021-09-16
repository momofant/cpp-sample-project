#include <functional>
#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include "examples.h"


using namespace std::chrono_literals;

class AddIntegersTask {
  int _value;
  int const *_pstart, *_pend;

public:
  AddIntegersTask(int const* pstart, int const* pend) : 
    _pstart(pstart), _pend(pend) {}

  int operator () () {
    int sum = 0;
    for (int const* p = _pstart; p < _pend; ++ p) {
      sum += *p;
    }
    _value = sum;
    return _value;
  }

  inline int value() const { return _value; }
};

class AddIntegersTasks {  
  int const *_datastart, *_dataend;
  int _tasksz;
  int _result = 0;

public:

  using value_type = int;
  using task = AddIntegersTask;

  AddIntegersTasks(int const* data, int nvalues, int tasksz) : 
    _datastart(data), _dataend(data + nvalues), _tasksz(tasksz) {}

  class iterator {
    AddIntegersTasks const* _tasks;
    int _cur;

  public:
    iterator(AddIntegersTasks const* tasks, int cur) : 
      _tasks(tasks), _cur(cur) {}

    iterator(iterator const& from) = default;

    inline iterator operator ++() {
      ++ _cur;
      return *this;
    }
    inline AddIntegersTask operator *() const {
      auto pstart = _tasks->_datastart + _cur * _tasks->_tasksz;
      auto pend = std::min(pstart + _tasks->_tasksz, _tasks->_dataend);
      return AddIntegersTask(pstart, pend);
    }
    inline bool operator != (iterator const& other) const {
      return _cur != other._cur;
    }
    inline bool operator == (iterator const& other) const {
      return _cur == other._cur;
    }
  };

  inline iterator begin() const {
    return iterator(this, 0);
  }
  inline iterator end() const {
    int ntasks = int(_dataend - _datastart + _tasksz - 1) / _tasksz;
    return iterator(this, ntasks);
  }
  inline void aggregate(AddIntegersTask const& task) {
    _result += task.value();
  }
  inline int value() const {
    return _result;
  }
};

template <typename Tasks>
class ThreadPool {
  Tasks& _tasks;
  typename Tasks::iterator _iterator;
  std::mutex _iterator_mutex, _aggragete_mutex;
  std::vector<std::thread> _threads;

  inline auto get_task() {
    std::lock_guard<std::mutex> lock(_iterator_mutex);
    auto cur = _iterator;
    ++ _iterator;
    return *cur;
  }

  inline void aggregate(typename Tasks::task const& task) {
    std::lock_guard<std::mutex> lock(_aggragete_mutex);
    _tasks.aggregate(task);
  }

  void loop() {
    auto end = _tasks.end();
    while (_iterator != end) {
      auto task = get_task();
      task();
      aggregate(task);
    }
  }

public:
  ThreadPool(Tasks& tasks) : 
    _tasks(tasks), _iterator(_tasks.begin()) {}
  ~ThreadPool() {}

  auto process() {
    int nthreads = std::thread::hardware_concurrency();
    _threads.clear();
    _threads.reserve(nthreads);
    for (int i = 0; i < nthreads; ++ i)
      _threads.emplace_back(&ThreadPool::loop, this);
    for (int i = 0; i < nthreads; ++ i)
      _threads[i].join();
    _threads.clear();
  }
};

int calc_sum(int const* pstart, int sz) {
  int const* pend = pstart + sz;
  int sum = 0;
  for (int const* p = pstart; p < pend; ++ p)
    sum += *p;
  return sum;
}

inline int64_t epoch_usec() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

EXAMPLE_FUNCTION(thread_pool_example, 0)
{
  const int nvalues = 10'000'000;
  std::vector<int> values;

  values.resize(nvalues);
  int* pstart = values.data();
  int* pend = pstart + values.size();

  for (int *p = pstart; p < pend; ++ p)
    *p = std::rand();

  // solution 1
  auto t1 = epoch_usec();
  int value = calc_sum(values.data(), values.size());
  auto t2 = epoch_usec();
  std::cout << value << " by " << t2 - t1 << " usec" << std::endl;

  // solution 2: thread pool
  t1 = epoch_usec();
  AddIntegersTasks tasks(values.data(), values.size(), 4096);
  int sum = 0;
  for (AddIntegersTask&& task : tasks) {
    sum += task();
  }
  t2 = epoch_usec();
  std::cout << sum << " by " << t2 - t1 << " usec" << std::endl;

  // solution 3: thread pool
  t1 = epoch_usec();
  ThreadPool pool(tasks);
  pool.process();
  t2 = epoch_usec();
  std::cout << tasks.value() << " by " << t2 - t1 << " usec" << std::endl;
}

EXAMPLE_FUNCTION(thread_create, 0)
{
  auto f = []() {
    std::cout << "thread 2 start" << std::endl;
    std::this_thread::sleep_for(2000ms);
    std::cout << "thread 2 end" << std::endl;
  };

  std::thread thread(f);
  std::cout << "thread 1 start" << std::endl;
  thread.join(); // wait for thread 2
  std::cout << "thread 1 end" << std::endl;
}

EXAMPLE_FUNCTION(thread_sync, 0)
{
  static std::atomic<float> counter = 0;
  // static float counter = 0;

  auto f = []() {
    for (int i = 0; i < 10'000; ++ i)
      counter += 1;
  };

  std::thread thread(f);

  for (int i = 0; i < 10'000; ++ i)
    counter += 1;
  
  thread.join();
  std::cout << counter << std::endl; 

}

EXAMPLE_FUNCTION(mutex_demo, 0)
{
  std::mutex mutex;

  auto f = [&mutex](char const* s) {
    for (int i = 0; i < 1000; ++ i)
    {
      std::lock_guard<std::mutex> lock(mutex);
      std::cout << s << std::endl;
    }
  };

  std::thread thread1(f, "aaaaaaaaaaaaaaaaaaaaaaaa");
  std::thread thread2(f, "bbbbbbbbbbbbbbbbbbbbbbbb");
  
  thread1.join();
  thread2.join();
}

namespace conditional_variable {
  std::mutex mtx;
  std::condition_variable cv;
  bool ready = false;

  void print_num(int id) {
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    std::cout << "thread " << id << '\n';
  }
  void go() {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
  }
  EXAMPLE_FUNCTION(conditional_variable, 0)
  {
    std::array<std::thread, 10> threads;

    for (int i = 0; i < threads.size(); ++i)
      threads[i] = std::thread(print_num, i);

    std::cout << "10 threads ready to race...\n";
    go();

    for (auto& th : threads)
      th.join();
  }
}

EXAMPLE_FUNCTION(deadlock, 0)
{
  std::mutex m1, m2;

  auto f1 = [&m1, &m2]() {
    m1.lock();
    m2.lock();
  };
  auto f2 = [&m1, &m2]() {
    m2.lock();
    m1.lock();
  };
  std::thread t1(f1);
  std::thread t2(f2);
  t1.join();
  t2.join();
}

EXAMPLE_FUNCTION(async, 0)
{
  auto f = []() {
    for (int i= 0; i < 1000; ++ i)
      std::cout << '|';
  };
  auto ff = std::async(std::launch::async, f);
  for (int i= 0; i < 1000; ++ i)
    std::cout << '-';
  ff.wait();
  std::cout << std::endl;
}

EXAMPLE_FUNCTION(async_2, 0)
{
  auto f = [](int i) {
    std::cout << i << " " << std::this_thread::get_id() << std::endl;
    return 0;
  };
  std::vector<std::future<int>> values;
  values.reserve(10);
  for (int i = 0; i < 10; ++ i)
    values.push_back(std::async(f, i));  
  
  for (int i = 0; i < 10; ++ i)
    values[i].wait();
}
