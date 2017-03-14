#include <emscripten/bind.h>
#include <dlib/image_processing/object_detector.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

using emscripten::function;
using emscripten::value_object;
using emscripten::allow_raw_pointers;
using emscripten::allow_raw_pointer;
using emscripten::ret_val;
using emscripten::class_;
using emscripten::select_overload;
using emscripten::select_const;
using namespace dlib;

EMSCRIPTEN_BINDINGS(dlib) {
  function("get_frontal_face_detector", &get_frontal_face_detector);

  class_<frontal_face_detector>("frontal_face_detector")
    .constructor<>()
    //.function("opcall", select_overload<std::vector<rectangle>(array2d<rgb_pixel>, double), frontal_face_detector>(&frontal_face_detector::operator() ) )
    ;

}