#include "homework1.hpp"
#include <numeric>

namespace hw1 {
  //structure representing a pixel RGBA
  struct RGBAPixel {
    union {
      uchar value[4];
      struct {
        uchar r, g, b, a;
      };
    };
    RGBAPixel(uchar r, uchar g, uchar b, uchar a) : r(r), g(g), b(b), a(a) {}
  };
}

QString hw1::getStudentName(void)
{
  //TODO: uncomment the following line and add your name, it'll be shown in About
  return "Ziwen Shen";
  return QString();
}

QImage hw1::filterImage(QImage const& inputImage, QVector<int> filter)
{
  //Description:
  //  hw1::filterImage() convolves 'inputImage' with the filter coefficients
  //  in the 'filter', and returns the filtered image as result. The original
  //  'inputImage' remains unchanged. The given filter coefficients correspond
  //  to a separable filter and must be applied twice: first to convolve 
  //  columns; and second to convolve rows. The filter is commonly given 
  //  unnormalized and must be normalized.

  // Example:
  //  Qimage inputImage;
  //  QVector<int> filter(QVector<int>() << 1 << 4 << 6 << 4 << 1);
  //  QImage outImage = hw1::filterImage(inputImage, filter)
  // Results in: outImage = ((inputImage * [1 4 6 4 1]) * [1 4 6 4 1]^T) / 256
  //  where * denotes convolution, ^T denotes transpose, and 1/256 is the 
  //  normalization of this particular filter: 256 = (1+4+6++4+1)^2.


  if (inputImage.format() != QImage::Format_RGB32)
  { //unsupported format: do nothing
      return QImage();
  }

  //half filter length
  int N = filter.length() / 2;

  //TODO: calculate normalization constant
  int normalization = std::accumulate(filter.begin(), filter.end(), 0);

  //filter columns
  QImage tempImage(inputImage.size(), QImage::Format_RGB32);
  for (int w = 0; w < inputImage.width(); ++w)
  {
      for (int h = 0; h < inputImage.height(); ++h)
      {
          //accumulate values values
          int r = 0, g = 0, b = 0;
          for (int i = -N; i <= N; ++i)
          {
              //TODO:
              //  use pixel = tempImage.getPixel(...) to get a pixel element
              //  use qRed(pixel), qGreen(pixel), and qBlue(pixel) to
              //    get individual color components
              //

              if (w + i >= 0 && w + i < inputImage.width())
              {
                  QRgb pixel = inputImage.pixel(w + i, h);
                  r += filter[i + N] * qRed(pixel);
                  g += filter[i + N] * qGreen(pixel);
                  b += filter[i + N] * qBlue(pixel);
              }
          }

          //normalize
          r /= normalization;
          g /= normalization;
          b /= normalization;

          tempImage.setPixel(w, h, qRgb(r, g, b));
      }
  }

  //filter rows
  QImage outputImage(inputImage.size(), QImage::Format_RGB32);
  for (int h = 0; h < inputImage.height(); ++h)
  {
      for (int w = 0; w < inputImage.width(); ++w)
      {
          //accumulate values values
          int r = 0, g = 0, b = 0;
          for (int i = -N; i <= N; ++i)
          {
              if (h + i >= 0 && h + i < tempImage.height())
              {
                  QRgb pixel = tempImage.pixel(w, h + i);
                  r += filter[i + N] * qRed(pixel);
                  g += filter[i + N] * qGreen(pixel);
                  b += filter[i + N] * qBlue(pixel);
              }
          }

          //normalize
          r /= normalization;
          g /= normalization;
          b /= normalization;


          outputImage.setPixel(w, h, qRgb(r, g, b));
      }
  }

  return outputImage;
}

QImage hw1::detectLaser
(QImage const& inputImage,
 int const x0, int const x1, int const y0, int const y1) {

  //Description:
  //  hw1::detectLaser() takes an RGB image as input and
  //  creates a new image with all pixels set to 0, except 
  //  the ones corresponding to the projected laser line,
  //  which must be set to 255. 
  //  It is assumed that the laser approximately a vertical line on the image.
  //  this function will scan the image row-by-row and set all the pixels to zero
  //  except one corresponding to the center of the detected laser line, 
  //  if the laser line is detected.

    if (inputImage.format() != QImage::Format_RGB32)
    { //unsupported format
        return QImage();
    }

    //inputImage format is QImage::Format_RGB32


    //TODO: implement this function as you wish to create the best possible output.
    //  In general, the detection may be implemented in several steps:
    //  - smooth the input image to remove noise
    //  - compare the red channel with the others:
    //      e.g. value = r - (g+b)/2
    //  - search the average maximum value of rows:
    //      e.g. avgMax = (max(row[0]) + max(row[1]) + ... + max(row(N)))/N
    //  - apply a threshold to all values much lower than the average maximum:
    //      e.g. if (pixel<0.8*avgMax) { pixel = 0; }
    //  - search the maximum of each row and set that pixel equal to 255


    QImage outputImage(inputImage.size(), QImage::Format_RGB32);
    outputImage.fill(qRgb(0, 0, 0));

    //smooth image
    //blur: f = [1 4 6 4 1]
    //TODO:
    QVector<int> filter(QVector<int>() << 1 << 4 << 6 << 4 << 1);
    QImage filteredImage = hw1::filterImage(inputImage, filter);

    //isolate red color
    for (int h = 0; h < filteredImage.height(); ++h)
    {
        for (int w = 0; w < filteredImage.width(); ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);

            int value = r - (g + b) / 2;
            if (value < 0)
                value = 0;

            filteredImage.setPixel(w, h, qRgb(value, value, value));
        }
    }

    //search average maximum of each row
    int sum = 0;
    for (int h = 0; h < outputImage.height(); ++h)
    {
        int max = 0;
        for (int w = 0; w < outputImage.width(); ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) > max)
                max = qRed(pixel);
        }
        sum += max;
    }
    double avg = sum / outputImage.height();

    //apply threshold
    for (int h = 0; h < outputImage.height(); ++h)
    {
        for (int w = 0; w < outputImage.width(); ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) < 0.8 * avg)
                filteredImage.setPixel(w, h, qRgb(0, 0, 0));
        }
    }

    //search the maximum of each row
    for (int h = 0; h < outputImage.height(); ++h)
    {
        int max = 0;
        int w0 = -1;
        for (int w = 0; w < outputImage.width(); ++w)
        {
            QRgb pixel = filteredImage.pixel(w, h);
            if (qRed(pixel) > max)
            {
                w0 = w;
                max = qRed(pixel);
            }
        }
        if (w0 > -1)
            outputImage.setPixel(w0, h, qRgb(255, 255, 255));
    }

    //return outputImage;
    return outputImage;
}
