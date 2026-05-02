cmake_minimum_required(VERSION 3.30.5)
project(Remote-Buy-Online-Shopping-Platform)

set(CMAKE_CXX_STANDARD 26)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable( catalogue.cpp,  cart.cpp, Shoppingheader.h, player_management.cpp

)

target_link_libraries(ShoppingApp PRIVATE Qt6::Widgets)