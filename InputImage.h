// Manage a text file with a single column of integers
// George F. Riley, Georgia Tech, Fall 2009

#include <fstream>

class InputImage {
public:
  // Load the specified file
  InputImage(const char* fileName);
  // Return the width of the file
  int       GetWidth() const;
  // Get a pointer to the file data.  Note that the return is a 1-D
  // array.
  int*  GetImageData() const;
  // Save a new file from the specified data array d
  // The size of d should be w.
static void SaveImageData(const char* newFileName, int* d, int w);
private:
  int width;         // width of image
  int* data;     // The data
};
