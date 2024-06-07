#include <chrono>
#include <format>
#include <iostream>
#include <thread>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

static void (*saved_logger)() = nullptr;

EXPORT extern "C" void call_cb(int (*func)(), void (*logger)()) {
  saved_logger = logger;
  std::thread([func]() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    std::cout << "start call callback" << std::endl;
    int ret = func();
    std::cout << std::format("end call callback with {}", ret) << std::endl;
  }).join();
}

EXPORT extern "C" int simple_add(int a, int b) {
  if (saved_logger) {
    saved_logger();
  }
  return a + b;
}