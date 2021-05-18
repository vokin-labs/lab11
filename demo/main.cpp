// Copyright 2021 MIX-1 <danilonil1@yandex.ru>

#include <header.hpp>

int main(int argc, const char *argv[]) {
  try
  {
    po::options_description desc{"Options"};
    po::variables_map vm;
    Builder::create_program_options(desc, vm, argc, argv);

    if(vm.count("help"))
      cout << desc;
    else if(argc > 1){
      Builder builder{};
      builder.start(vm);
    } else
      throw po::error(error_mes);
  } catch (const po::error &ex)
  {
    BOOST_LOG_TRIVIAL(error) << ex.what() << '\n';
  }
}