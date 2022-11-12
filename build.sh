if [ -d build ]; then
    echo "build dir has been created"
else
    echo "build dir has not been created"
    echo "create build dir"
    mkdir build
fi

cd build
cmake ..
make clean && make
echo "[++++] build finished"