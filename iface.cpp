#include <dlib/image_processing/object_detector.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <iostream>

using std::cout;
using std::endl;
using namespace dlib;

int main(){ try{
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
} }