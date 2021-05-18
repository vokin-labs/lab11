// Copyright 2021 MIX-1 <danilonil1@yandex.ru>

#include <header.hpp>
#include <utility>
#include <string>

#ifndef INCLUDE_PROCESS_HPP_
#define INCLUDE_PROCESS_HPP_

class Process{
 public:
  const std::string BUILD_TARGET = "_builds";
  const std::string INSTALL_TARGET = "_install";

  Process(std::string config_, bool install_, bool pack_, int time_)
      : config(std::move(config_)),
        install(install_),
        pack(pack_),
        time(time_) {}

  bool get_pack() const {return pack;}

  bool get_install() const {return install;}

  int get_time() const {return time;}

  std::string get_command(const std::string& target){
    if (target == "config") {
      return (" -B" + BUILD_TARGET + " -DCMAKE_INSTALL_PREFIX=" +
              INSTALL_TARGET + " -DCMAKE_BUILD_TYPE=" + config);
    } else {
      return ("--build " + BUILD_TARGET +
              (target != "build" ? (" --target " + target) : ""));
    }
  };

 private:
  std::string config;
  bool install, pack;
  int time = 0;
};

struct Process_info{
  Process_info(bool term, boost::process::child&& child) {
    terminated.store(term);
    current_child = std::move(child);
  }

  void set_bool(bool term) { terminated.store(term); }
  void set_child(boost::process::child&& child) {
    current_child = std::move(child);
  }

  std::atomic_bool terminated = false;
  boost::process::child current_child;
};

class timer{
 private:
  std::thread _timer_thread;

 public:
  timer(timer&& t) { _timer_thread = std::move(t._timer_thread); }

  timer(std::chrono::seconds delay,
        std::function<void(Process_info&)> callback_obj,
        Process_info& pdata)
      : _timer_thread([delay, callback_obj, &pdata]() {
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    callback_obj(pdata);
  }) {}
  ~timer() {
    if (_timer_thread.joinable()) _timer_thread.detach();
  }
};
#endif  // INCLUDE_PROCESS_HPP_
