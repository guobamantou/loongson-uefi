#

PROJECT_ROOT_DIR=`pwd`
PROJECT_FD_IMAGE_NAME=SIZE512KB.fd

cd ..

source edksetup.sh

PROJECT_TOOLS_SRC_DIR=$PROJECT_ROOT_DIR/Tools
PROJECT_PLATFORM_NAME=Fuloong6003
PROJECT_BUILD_DIR=$WORKSPACE/Build/$PROJECT_PLATFORM_NAME/DEBUG_MIPSELFGCC
PROJECT_BUILD_FV_DIR=$PROJECT_BUILD_DIR/FV
PROJECT_BUILD_TOOLS_DIR=$PROJECT_BUILD_DIR/TOOLS

if [ $1 = "clean" ]; then
    cd ../BaseTools/
    make clean;make
    cd -
    rm -rf ../Build
    build -t MIPSELFGCC -p ${PROJECT_PLATFORM_NAME}Pkg/${PROJECT_PLATFORM_NAME}Pkg.dsc -a MIPS64 clean
    cd $PROJECT_ROOT_DIR
    return
fi

if [ -d $PROJECT_BUILD_TOOLS_DIR ]; then
    echo $PROJECT_BUILD_TOOLS_DIR
else
    mkdir  $PROJECT_BUILD_TOOLS_DIR
fi

echo "Building tools ..."
cd $PROJECT_TOOLS_SRC_DIR/GeneratePad/
make
cp -f GeneratePad $PROJECT_BUILD_TOOLS_DIR
cd $PROJECT_TOOLS_SRC_DIR/RecalculateFvZeroVector
make
cp -f RecalculateFvZeroVector $PROJECT_BUILD_TOOLS_DIR

echo "Tools built."

cd $PROJECT_ROOT_DIR/Sec/Exception/
make
$PROJECT_BUILD_TOOLS_DIR/GeneratePad -d 0xff -f Exception.pad -s 0x18

echo "Standard build process start..."

cd $WORKSPACE

rm -f $PROJECT_BUILD_FV_DIR/${PROJECT_FD_IMAGE_NAME}.*

build -t MIPSELFGCC -p ${PROJECT_PLATFORM_NAME}Pkg/${PROJECT_PLATFORM_NAME}Pkg.dsc -a MIPS64

cd $PROJECT_BUILD_FV_DIR
$PROJECT_BUILD_TOOLS_DIR/RecalculateFvZeroVector $PROJECT_FD_IMAGE_NAME
cp -f $PROJECT_FD_IMAGE_NAME $PROJECT_ROOT_DIR
cd $PROJECT_ROOT_DIR

