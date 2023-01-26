# restful-polygon

This repo contains a fully functional, containerized RestFul service in Python + Flask with C++ Binding using Pybind. The service takes in an input 3D point and polygon, and returns `true` if the point is inside of the polygon (not including edges or vertices), and `false` otherwise. I used Ubuntu.

## Setup/Dependencies

### For building/deploying the container

Install the following:

- [Docker](https://docs.docker.com/engine/install/)
- [kubectl](https://kubernetes.io/docs/tasks/tools/) - Kubernetes command line tool
- [minikube](https://minikube.sigs.k8s.io/docs/start/) - For deploying a Kubernetes cluster locally

After starting `minikube`, make sure to enable ingress with

```
minikube addons enable ingress
```

### For running the API locally

Install `Flask` and `pybind11` using

```
pip3 install -r src/requirements.txt
```

### For running the unit tests (locally)

`gtest` is needed to run the C++ unit tests.

```
sudo apt-get install libgtest-dev
```

## Building and Deploying the API

### Deploying on a Kubernetes cluster

```
make k8s-apply
```

This will apply all of the configuration files in the `kubernetes/` directory, creating a deployment, service, and ingress. To send a request to the API, run `kubectl get ingress`. The request should be sent to the IP address listed under the `ADDRESS` field of `restful-polygon-ingress`. To delete all of the k8s resources, run

```
make clean-k8s
```

### Run the API locally

```
make start-api
```

This will run the API on `localhost`.

### Run the unit tests

```
make test-all
```

This will run all unit tests in the `test/` directory, which includes a unit test for the C++ polygon code as well as a unit test for the Flask API. 

## Endpoints

- `/` - (GET Request) - Returns the following help message.
```
    Check whether a 3-dimensional point lies within or outside of a 3-dimensional polygon.
    Points on an edge or vertex will return false. 
    Send a POST request to the /contains endpoint with the following format:
    {
        "point": [0.5, 0.5, 0.0],
        "polygon_vertices":
            [
                [0.0, 1.0, 0.0],
                [1.0, 1.0, 0.0],
                [1.0, 0.0, 0.0],
                [0.0, 0.0, 0.0]
            ]
    }
    The 'point' field should contain a 3-dimensional point, and the 'polygon_vertices' field
    should contain a list of at least three 3-dimensional points that form a valid polygon.
    Example successful response for the above request:
    {
        "point": [0.5, 0.5, 0.0],
        "polygon_contains_point": true,
        "polygon_vertices": [
            [0.0, 1.0, 0.0],
            [1.0, 1.0, 0.0],
            [1.0, 0.0, 0.0],
            [0.0, 0.0, 0.0]
        ]
    }
```

- `/contains` - (POST request) See above help message. To test this endpoint manually, you can use the following command to send a POST request with `test.json` as the payload. 

```
curl -X POST -H "Content-Type: application/json" -d @test.json <hostname>/contains
```
