CURRENT_DIR = $(shell pwd)

list-targets:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'

bind-polygon:
	g++ -O3 -Wall -shared -std=c++11 -fPIC \
	$$(python3 -m pybind11 --includes) \
	src/cpp/polygon_py.cpp src/cpp/polygon.cpp \
	-o build/polygon3d$$(python3-config --extension-suffix)

test-polygon:
	g++ -std=c++11 \
	-I/usr/include/gtest -I$(CURRENT_DIR)/src/cpp \
	-L/usr/lib -pthread \
	$(CURRENT_DIR)/test/cpp/test_polygon.cpp $(CURRENT_DIR)/src/cpp/polygon.cpp \
	-o $(CURRENT_DIR)/build/test_polygon.out \
	-lgtest -lgtest_main
	$(CURRENT_DIR)/build/test_polygon.out

test-api: bind-polygon
	PYTHONPATH="$(CURRENT_DIR)/build":$$PYTHONPATH;\
	export PYTHONPATH;\
	flask --app $(CURRENT_DIR)/src/api.py run & sleep 5; python3 $(CURRENT_DIR)/test/test_api.py; sleep 1; killall flask

test-all: test-polygon test-api

start-api: bind-polygon
	PYTHONPATH="$(CURRENT_DIR)/build":$$PYTHONPATH;\
	export PYTHONPATH;\
	flask --app $(CURRENT_DIR)/src/api.py run