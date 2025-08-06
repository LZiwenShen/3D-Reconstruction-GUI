#ifndef _homework1_hpp_
#define _homework1_hpp_

#include <QImage>
#include <QString>

namespace hw1
{
  QString getStudentName(void);
  
  QImage  filterImage
  (QImage const& inputImage, QVector<int> filter);

  QImage  detectLaser
  (QImage const& inputImage,
   int const x0=0, int const x1=0, int const y0=0, int const y1=0);
};

#endif //_homework1_hpp_
