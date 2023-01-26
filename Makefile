NSPACE="harrijin"
APP="restful-polygon"
VER="latest"
PROJECT_DIR=$(shell pwd)

list-targets:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$'

bind-polygon:
	mkdir -p build
	g++ -O3 -Wall -shared -std=c++11 -fPIC \
	$$(python3 -m pybind11 --includes) \
	$(PROJECT_DIR)/src/cpp/polygon_py.cpp $(PROJECT_DIR)/src/cpp/polygon.cpp \
	-o $(PROJECT_DIR)/build/polygon3d$$(python3-config --extension-suffix)

test-polygon:
	mkdir -p build
	g++ -std=c++11 \
	-I/usr/include/gtest -I$(PROJECT_DIR)/src/cpp \
	-L/usr/lib -pthread \
	$(PROJECT_DIR)/test/cpp/test_polygon.cpp $(PROJECT_DIR)/src/cpp/polygon.cpp \
	-o $(PROJECT_DIR)/build/test_polygon.out \
	-lgtest -lgtest_main
	$(PROJECT_DIR)/build/test_polygon.out

test-api: bind-polygon
	PYTHONPATH="$(PROJECT_DIR)/build":$$PYTHONPATH;\
	export PYTHONPATH;\
	flask --app $(PROJECT_DIR)/src/api.py run & sleep 5; python3 $(PROJECT_DIR)/test/test_api.py; sleep 1; killall flask

test-all: test-polygon test-api

start-api: bind-polygon
	PYTHONPATH="$(PROJECT_DIR)/build":$$PYTHONPATH;\
	export PYTHONPATH;\
	flask --app $(PROJECT_DIR)/src/api.py run --host=0.0.0.0

build-api-container:
	sudo docker build -t ${NSPACE}/${APP}:${VER} .

push-api-container: build-api-container
	sudo docker push ${NSPACE}/${APP}:${VER}

k8s-apply:
	kubectl apply -f kubernetes/deployment.yaml
	kubectl apply -f kubernetes/service.yaml
	kubectl apply -f kubernetes/ingress.yaml

clean-k8s:
	kubectl delete svc restful-polygon-service
	kubectl delete deployment restful-polygon-deployment
	kubectl delete ing restful-polygon-ingress