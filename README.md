# GrpcCppHelloWorld
## C++ gRPC client sample

Client for the [gRPC .Net server sample](https://github.com/seclerp/GrpcHelloWorld "seclerp/GrpcHelloWorld").

### Start server in Docker
1. Clone [seclerp/GrpcHelloWorld](https://github.com/seclerp/GrpcHelloWorld "seclerp/GrpcHelloWorld")
2. Open repo
3. Build Docker image: `docker build . --tag grpc-hw-server:latest`
4. Start: `docker run -p 80:80 --name "gRPC_HelloWorld_Server" -d grpc-hw-server`
5. Check status: `docker ps -a`

### Start client on Linux
1. Install gRPC using vcpkg or build from source
2. Clone [Alexander-Degtyar/GrpcCppHelloWorld](https://github.com/Alexander-Degtyar/GrpcCppHelloWorld "Alexander-Degtyar/GrpcCppHelloWorld")
3. Open repo
4. Create build dir: `mkdir build && cd build`
5. Generate build: `cmake ..` (or `cmake -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg.cmake>  ..` if the depencencies installed using vcpkg)
6. Build: `cmake --build .`
7. Start: `./client`
