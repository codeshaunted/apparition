# codeshaunted - apparition
# source/apparition/CMakeLists.txt
# apparition source CMake file
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

set(APPARITION_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/math.cc")

set(APPARITION_INCLUDE_DIRECTORIES
	"${CMAKE_SOURCE_DIR}/include/apparition")

set(APPARITION_LINK_LIBRARIES)

set(APPARITION_COMPILE_DEFINITIONS)

add_library(apparition ${APPARITION_SOURCE_FILES})

target_include_directories(apparition PUBLIC ${APPARITION_INCLUDE_DIRECTORIES})

target_link_libraries(apparition PUBLIC ${APPARITION_LINK_LIBRARIES})

target_compile_definitions(apparition PUBLIC ${APPARITION_COMPILE_DEFINITIONS})