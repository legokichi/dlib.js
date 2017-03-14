cd dlib
mkdir build
cd build
OPTS='-O2 --llvm-lto 1'
emcmake cmake \
  -DDLIB_ISO_CPP_ONLY=ON \
  -DDLIB_NO_GUI_SUPPORT=ON \
  -DCMAKE_CXX_FLAGS=$OPTS \
  -DCMAKE_EXE_LINKER_FLAGS=$OPTS \
  -DCMAKE_CXX_FLAGS_DEBUG=$OPTS \
  -DCMAKE_CXX_FLAGS_RELWITHDEBINFO=$OPTS \
  -DCMAKE_C_FLAGS_RELWITHDEBINFO=$OPTS \
  -DCMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO=$OPTS \
  -DCMAKE_MODULE_LINKER_FLAGS_RELEASE=$OPTS \
  -DCMAKE_MODULE_LINKER_FLAGS_DEBUG=$OPTS \
  -DCMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO=$OPTS \
  -DCMAKE_SHARED_LINKER_FLAGS_RELEASE=$OPTS \
  -DCMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO=$OPTS \
  -DCMAKE_SHARED_LINKER_FLAGS_DEBUG=$OPTS \
  ..
# -DDLIB_ISO_CPP_ONLY=ON \
# -DDLIB_NO_GUI_SUPPORT=ON \
# -DDLIB_USE_CUDA=OFF \
# options: ref
# http://dlib.net/compile.html
# https://github.com/davisking/dlib/blob/master/dlib/CMakeLists.txt
# http://qiita.com/SousukeShimoyama/items/a6b853a1575623f603cf

emmake make

# llvm-nm ./dlib/libdlib.a
# llvm-nm
# http://llvm.org/docs/CommandGuide/llvm-nm.html

#echo binding...
emcc \
  -I../ \
  ../../iface.cpp \
  -std=c++1z \
  -DDLIB_JPEG_SUPPORT \
  -O2  --llvm-lto 1 \
  -o ./iface.bc
# MUST need "-DDLIB_JPEG_SUPPORT" option - https://sourceforge.net/p/dclib/discussion/442518/thread/93deb3e9/#178a/3589/57e7/8d47

echo preload files
wget http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
bzip2 -d shape_predictor_68_face_landmarks.dat.bz2
wget http://dlib.net/files/mmod_human_face_detector.dat.bz2
bzip2 -d mmod_human_face_detector.dat.bz2
cp ../examples/faces/2008_002470.jpg ./2008_002470.jpg

echo emitting to js
# http://dlib.net/files/
# http://blog.dlib.net/2014/08/real-time-face-pose-estimation.html
# http://dlib.net/face_landmark_detection_ex.cpp.html

emcc \
  ./iface.bc \
  ./dlib/libdlib.a \
  --preload-file ./2008_002470.jpg \
  --preload-file ./shape_predictor_68_face_landmarks.dat \
  --preload-file ./mmod_human_face_detector.dat \
  -O2 --llvm-lto 3 \
  -s ASSERTIONS=0 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s DISABLE_EXCEPTION_CATCHING=0 \
  -o ../../dlib.html
