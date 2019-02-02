pushd server
clang-format -i src/*
./build.sh
popd

pushd client
clang-format -i src/*
./build.sh
popd