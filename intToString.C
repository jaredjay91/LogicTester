#include <sstream>

string intToString(int i=123){
  std::string out_string;
  std::stringstream ss;
  ss << i;
  out_string = ss.str();
  //cout << "out_string = " << out_string << endl;
  return out_string;
}
