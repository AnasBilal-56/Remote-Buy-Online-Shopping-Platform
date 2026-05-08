cmake_minimum_required(VERSION 3.30.5)
project(Remote-Buy-Online-Shopping-Platform)

set(CMAKE_CXX_STANDARD 26)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Remote-Buy-Online-Shopping-Platform
        catalogue.cpp cart.cpp Shoppingheader.h customer_managment.cpp admin.cpp Main.cpp

)

target_link_libraries(Remote-Buy-Online-Shopping-Platform PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)