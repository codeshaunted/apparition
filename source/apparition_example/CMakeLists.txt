# codeshaunted - apparition
# source/apparition_example/CMakeLists.txt
# apparition_example source CMake file
# Copyright 2024 codeshaunted
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(APPARITION_EXAMPLE_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/main.cc")

set(APPARITION_EXAMPLE_INCLUDE_DIRECTORIES
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/apparition_example")

set(APPARITION_EXAMPLE_LINK_LIBRARIES
	apparition)

set(APPARITION_EXAMPLE_COMPILE_DEFINITIONS)

add_executable(apparition_example ${APPARITION_EXAMPLE_SOURCE_FILES})

target_include_directories(apparition_example PUBLIC ${APPARITION_EXAMPLE_INCLUDE_DIRECTORIES})

target_link_libraries(apparition_example PUBLIC ${APPARITION_EXAMPLE_LINK_LIBRARIES})

target_compile_definitions(apparition_example PUBLIC ${APPARITION_EXAMPLE_COMPILE_DEFINITIONS})