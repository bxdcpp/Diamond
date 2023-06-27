cmake_minimum_required(VERSION 3.1.0)
project(Diamond)

set(Qt5_DIR "C:/Qt/Qt5.12.4/5.12.4/msvc2017_64/lib/cmake/Qt5")
set(VTK_DIR "S:/VTK-9.2.6-build") 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
# find_package(VTK 9.2.6 REQUIRED)
find_package(VTK  REQUIRED)

set(header_files
    AppMainWindow.h
    SliceWidget.h
    ThreeDWidget.h
    vtkImageDataIO.h
    ColorLookupTable.h
    CommonDef.h
)

set(source_files 
    SliceWidget.cpp
    AppMainWindow.cpp
    ThreeDWidget.cpp
    vtkImageDataIO.cxx
    ColorLookupTable.cpp
    main.cpp
)

set(resource_files
    Resources/UI/AppMainWindow.ui
)

set(qrc_files
    Resources/AppMainWindow.qrc
)

message(${header_files})
qt5_wrap_cpp(${PROJECT_NAME}_MOC_CXX ${header_files})
qt5_wrap_ui(${PROJECT_NAME}_UI_CXX ${resource_files})
qt5_add_resources(${PROJECT_NAME}_RCC_CXX ${qrc_files})


# Group files
source_group("Resources" FILES
    ${resource_files}
    ${qrc_files}
)

source_group("Generated" FILES
    ${${PROJECT_NAME}_MOC_CXX}
    ${${PROJECT_NAME}_UI_CXX}
    ${${PROJECT_NAME}_RCC_CXX}
)

source_group("Header Files" FILES
    ${header_files}
)


message(${source_files})
add_executable(${PROJECT_NAME}
  ${source_files}
  ${${PROJECT_NAME}_MOC_CXX} 
  ${${PROJECT_NAME}_UI_CXX}
  ${${PROJECT_NAME}_RCC_CXX}
  ${header_files}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets ${VTK_LIBRARIES})

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS ${PROJECT_NAME} 
  MODULES ${VTK_LIBRARIES}
)