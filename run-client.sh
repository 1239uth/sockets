HOST=${1:-localhost}

if [ -d "build" ]; then
    cd build
    if [ -f "client" ]; then
        ./client $HOST
    else
        echo "Run ./build.sh first"
    fi
else
    echo "Run ./build.sh first"
fi