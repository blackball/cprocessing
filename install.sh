#!/bin/bash

#libcprocessing linux install script

version="0.1"   #increment this if testing to spot regressions
libDest="/usr/local/lib"
incDest="/usr/local/include"

make || { echo 'make failed' ; exit 1; }

sudo cp cprocessing.hpp $incDest/cprocessing.hpp || { echo 'cp cprocessing.hpp $incDest/cprocessing.hpp failed'; exit 2; }

cd objects || { echo 'cd objects failed, run mkdir objects'; exit 3; }
gcc -shared -fPIC -o libcprocessing.so.$version *.o || { echo 'building .so failed'; exit 4; }
sudo cp libcprocessing.so.$version $libDest/libcprocessing.so.$version || { echo 'cp libcprocessing.so.$version $libDest/libcprocessing.so.$version failed'; exit 5; }

cd $libDest || { echo 'cd $libDest failed'; exit 6; }
if [ -e "libcprocessing.so" ]
    then
        echo "removing old libcprocessing symbolic link" &&
        sudo rm libcprocessing.so
fi

echo "linking"
sudo ln libcprocessing.so.$version libcprocessing.so || { echo 'linking libcprocessing failed'; exit 7; }
echo "processing ldconfig"
sudo ldconfig || { echo 'running ldconfig failed'; exit 8; }
echo "Done"
