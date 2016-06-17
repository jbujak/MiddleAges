cmake_minimum_required(VERSION 2.8)
project(middle_ages C)

find_library (CMOCKA cmocka)

if (NOT CMOCKA)
	message (FATAL_ERROR "Could not find cmocka.")
endif ()

enable_testing ()

# deklarujemy opcję DEBUG, domyślnie wyłączoną (opcję można włączyć przez argument -DDEBUG=ON)
option (DEBUG OFF)

# jeśli DEBUG == ON, ustawiamy zmienną specyfikującą typ kompilacji na wartość DEBUG, wpp. na release
if (DEBUG)
	set(CMAKE_BUILD_TYPE DEBUG)
else (DEBUG)
	set(CMAKE_BUILD_TYPE RELEASE)
endif (DEBUG)

# ustawiamy flagi kompilacji w wersji debug i release
set(CMAKE_C_FLAGS_DEBUG "-std=gnu99 -Wall -Wextra -pedantic -g")
set(CMAKE_C_FLAGS_RELEASE "-std=gnu99 -O2")

set(SOURCE_FILES
	src/engine.c
	src/engine.h
	src/middle_ages.c
	src/parse.c
	src/parse.h
	src/list.h
	src/list.c
	src/pawn.h
	src/pawn.c
	src/preview.h
	src/preview.c
	src/ai.h
	src/ai.c
	src/global.h
	src/command.h
	src/command.c
	)

add_executable(middle_ages ${SOURCE_FILES})


set(TEST_FILES
	${SOURCE_FILES}
	src/utils.h
	src/move_test.c
	)

add_executable(middle_ages_tests ${TEST_FILES})
set_target_properties (
	middle_ages_tests
	PROPERTIES
	COMPILE_DEFINITIONS UNIT_TESTING=1
	COMPILE_FLAGS "-Wall -g -DUNIT_TESTING")
target_link_libraries (middle_ages_tests ${CMOCKA})
add_test (middle_ages_tests ${CMAKE_CURRENT_BINARY_DIR}/middle_ages_tests)

# dodajemy obsługę Doxygena: sprawdzamy, czy jest zainstalowany i jeśli tak:
find_package(Doxygen)
if(DOXYGEN_FOUND)
	# wskazujemy lokalizacją pliku konfiguracyjnego i podajemy jego docelową lokalizację w folderze, gdzie następuje kompilacja
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	# dodajemy cel doc: użycie make doc spowoduje wywołanie doxygena z odpowiednim plikiem konfiguracyjnym w folderze kompilacji.
	# Na wyjście zostanie wypisana odpowiedni komentarz.
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen"
		)
endif(DOXYGEN_FOUND)

