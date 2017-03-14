#include <dlib/image_processing/object_detector.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <iostream>
#include <emscripten/bind.h>

using std::cout;
using std::endl;
using namespace dlib;

int hog_face_detector(){ try{
  // http://dlib.net/face_landmark_detection_ex.cpp.html
  frontal_face_detector detector = get_frontal_face_detector();
  cout << "detector" << endl;

  shape_predictor sp;
  deserialize("./shape_predictor_68_face_landmarks.dat") >> sp;
  cout << "sp" << endl;

  array2d<rgb_pixel> img;
  load_image(img, "./2008_002470.jpg");
  pyramid_up(img);
  cout << "img" << endl;

  std::vector<rectangle> dets = detector(img);
  std::vector<full_object_detection> shapes;
  cout << "dets" << endl;

  for (unsigned long j = 0; j < dets.size(); ++j){
    full_object_detection shape = sp(img, dets[j]);
    cout << "number of parts: "<< shape.num_parts() << endl;
    cout << "pixel position of first part:  " << shape.part(0) << endl;
    cout << "pixel position of second part: " << shape.part(1) << endl;
    shapes.push_back(shape);
  }
  return 0;
}catch(std::exception& e){
  cout << "\nexception thrown!" << endl;
  cout << e.what() << endl;
  return 1;
} }

// http://dlib.net/dnn_mmod_face_detection_ex.cpp.html
// http://dlib.net/files/data/dlib_face_detection_dataset-2016-09-30.tar.gz
// faces_2016_09_30.xml
template <long num_filters, typename SUBNET> using con5d = con<num_filters,5,5,2,2,SUBNET>;
template <long num_filters, typename SUBNET> using con5  = con<num_filters,5,5,1,1,SUBNET>;
template <typename SUBNET> using downsampler  = relu<affine<con5d<32, relu<affine<con5d<32, relu<affine<con5d<16,SUBNET>>>>>>>>>;
template <typename SUBNET> using rcon5  = relu<affine<con5<45,SUBNET>>>;
using net_type = loss_mmod<con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;

int cnn_face_detector(){ try{
  net_type net;
  deserialize("./mmod_human_face_detector.dat") >> net;
  cout << "deserialize" << endl;

  matrix<rgb_pixel> img;
  load_image(img, "./2008_002470.jpg");
  cout << "img" << endl;

  while(img.size() < 1800*1800){ pyramid_up(img); }
  cout << "pyramid_up" << endl;

  const std::vector<mmod_rect> dets = net(img);
  cout << "net" << endl;

  for (auto&& d : dets){
    auto rect = d.rect;
    cout
      << "(" << rect.left() << "," << rect.top() << ")"
      << ","
      << "(" << rect.right() << "," << rect.bottom() << ")"
      << endl;
  }

  return 0;
}catch(std::exception& e){
  cout << "\nexception thrown!" << endl;
  cout << e.what() << endl;
  return 1;
} }


EMSCRIPTEN_BINDINGS(dlib) {
  emscripten::function("hog_face_detector", &hog_face_detector);
  emscripten::function("cnn_face_detector", &cnn_face_detector);
}