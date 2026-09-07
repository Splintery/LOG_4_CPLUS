path=$PWD
cd ../
if [ -d "build" ]; then
    ./build/bin/main
else
    echo "Nothing to run, build folder doesn't exist."
fi
cd $path
