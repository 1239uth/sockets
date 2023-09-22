if [ -d "build" ]; then
    cd build
    if [ -f "server" ]; then
        ./server
    else
        echo "Run ./build.sh first"
    fi
else
    echo "Run ./build.sh first"
fi