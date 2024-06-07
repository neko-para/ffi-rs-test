#include <chrono>
#include <format>
#include <iostream>
#include <thread>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

static int (*saved_logger)() = nullptr;

EXPORT extern "C" void call_cb(int (*func)(), int (*logger)()) {
  std::cout << "start call_cb" << std::endl;
  logger();
  saved_logger = logger;
  std::thread([func]() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    std::cout << "start call callback" << std::endl;
    int ret = func();
    std::cout << std::format("end call callback with {}", ret) << std::endl;
  }).join();
  std::cout << "end call_cb" << std::endl;
}

EXPORT extern "C" int simple_add(int a, int b) {
  int sum = a + b;
  if (saved_logger) {
    sum += saved_logger();
  }
  return sum;
}