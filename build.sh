pushd server
cog.py -r src/*
clang-format -i src/*
./build.sh
popd

pushd client
cog.py -r src/*
clang-format -i src/*
./build.sh
popd