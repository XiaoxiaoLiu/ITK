/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/


#include "itkImage.h"
#include "itkFFTTest.h"

#include "itkVnlInverseFFTImageFilter.h"

// Test FFT using VNL Libraries. The test is performed for 2 3d array one of
// them having the same dimension(4,4,4) and the other having different
// dimensions (3,4,5).  Images are created with different dimensions in the
// test function based on the second template argument    and  the size of
// these dimensions are taken from the array.  The data types used are float
// and double.
int itkVnlFFTTest(int, char *[])
{
  typedef itk::Image< float, 1>               ImageF1;
  typedef itk::Image< std::complex<float>, 1> ImageCF1;
  typedef itk::Image< float, 2>               ImageF2;
  typedef itk::Image< std::complex<float>, 2> ImageCF2;
  typedef itk::Image< float, 3>               ImageF3;
  typedef itk::Image< std::complex<float>, 3> ImageCF3;
  typedef itk::Image< float, 4>               ImageF4;
  typedef itk::Image< std::complex<float>, 4> ImageCF4;

  typedef itk::Image< double, 1>               ImageD1;
  typedef itk::Image< std::complex<double>, 1> ImageCD1;
  typedef itk::Image< double, 2>               ImageD2;
  typedef itk::Image< std::complex<double>, 2> ImageCD2;
  typedef itk::Image< double, 3>               ImageD3;
  typedef itk::Image< std::complex<double>, 3> ImageCD3;
  typedef itk::Image< double, 4>               ImageD4;
  typedef itk::Image< std::complex<double>, 4> ImageCD4;

  unsigned int SizeOfDimensions1[] = { 4,4,4,4 };
  unsigned int SizeOfDimensions2[] = { 3,5,4 };
  unsigned int SizeOfDimensions3[] = { 7,6,4 }; // Should fail
  int rval = 0;
  std::cerr << "Vnl float,1 (4,4,4)" << std::endl;
  if((test_fft<float,1,
      itk::VnlForwardFFTImageFilter<ImageF1> ,
      itk::VnlInverseFFTImageFilter<ImageCF1> >(SizeOfDimensions1)) != 0)
    {
    rval++;
    std::cerr << "--------------------- Failed!" << std::endl;
    }
  std::cerr << "Vnl float,2 (4,4,4)"<< std::endl;
  if((test_fft<float,2,
      itk::VnlForwardFFTImageFilter<ImageF2> ,
      itk::VnlInverseFFTImageFilter<ImageCF2> >(SizeOfDimensions1)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl float,3 (4,4,4)"<< std::endl;
  if((test_fft<float,3,
      itk::VnlForwardFFTImageFilter<ImageF3> ,
      itk::VnlInverseFFTImageFilter<ImageCF3> >(SizeOfDimensions1)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl float,4 (4,4,4)"<< std::endl;
  if((test_fft<float,4,
      itk::VnlForwardFFTImageFilter<ImageF4> ,
      itk::VnlInverseFFTImageFilter<ImageCF4> >(SizeOfDimensions1)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl double,1 (4,4,4)"<< std::endl;
  if((test_fft<double,1,
      itk::VnlForwardFFTImageFilter<ImageD1> ,
      itk::VnlInverseFFTImageFilter<ImageCD1> >(SizeOfDimensions1)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl double,2 (4,4,4)"<< std::endl;
  if((test_fft<double,2,
      itk::VnlForwardFFTImageFilter<ImageD2> ,
      itk::VnlInverseFFTImageFilter<ImageCD2> >(SizeOfDimensions1)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl double,3 (4,4,4)"<< std::endl;
  if((test_fft<double,3,
      itk::VnlForwardFFTImageFilter<ImageD3> ,
      itk::VnlInverseFFTImageFilter<ImageCD3> >(SizeOfDimensions1)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl float,1 (3,5,4)" << std::endl;
  if((test_fft<float,1,
      itk::VnlForwardFFTImageFilter<ImageF1> ,
      itk::VnlInverseFFTImageFilter<ImageCF1> >(SizeOfDimensions2)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl float,2 (3,5,4)"<< std::endl;
  if((test_fft<float,2,
      itk::VnlForwardFFTImageFilter<ImageF2> ,
      itk::VnlInverseFFTImageFilter<ImageCF2> >(SizeOfDimensions2)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl float,3 (3,5,4)"<< std::endl;
  if((test_fft<float,3,
      itk::VnlForwardFFTImageFilter<ImageF3> ,
      itk::VnlInverseFFTImageFilter<ImageCF3> >(SizeOfDimensions2)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl double,1 (3,5,4)"<< std::endl;
  if((test_fft<double,1,
      itk::VnlForwardFFTImageFilter<ImageD1> ,
      itk::VnlInverseFFTImageFilter<ImageCD1> >(SizeOfDimensions2)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl double,2 (3,5,4)"<< std::endl;
  if((test_fft<double,2,
      itk::VnlForwardFFTImageFilter<ImageD2> ,
      itk::VnlInverseFFTImageFilter<ImageCD2> >(SizeOfDimensions2)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  std::cerr << "Vnl double,3 (3,5,4)"<< std::endl;
  if((test_fft<double,3,
      itk::VnlForwardFFTImageFilter<ImageD3> ,
      itk::VnlInverseFFTImageFilter<ImageCD3> >(SizeOfDimensions2)) != 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }

  // These tests should fail.

  std::cerr << "Vnl float,1 (7,6,4)" << std::endl;
  if((test_fft<float,1,
      itk::VnlForwardFFTImageFilter<ImageF1> ,
      itk::VnlInverseFFTImageFilter<ImageCF1> >(SizeOfDimensions3)) == 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  else
    {
    std::cerr << "Caught expected size error." << std::endl;
    }

  std::cerr << "Vnl float,2 (7,6,4)" << std::endl;
  if((test_fft<float,2,
      itk::VnlForwardFFTImageFilter<ImageF2> ,
      itk::VnlInverseFFTImageFilter<ImageCF2> >(SizeOfDimensions3)) == 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  else
    {
    std::cerr << "Caught expected size error." << std::endl;
    }

  std::cerr << "Vnl float,3 (7,6,4)" << std::endl;
  if((test_fft<float,3,
      itk::VnlForwardFFTImageFilter<ImageF3> ,
      itk::VnlInverseFFTImageFilter<ImageCF3> >(SizeOfDimensions3)) == 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  else
    {
    std::cerr << "Caught expected size error." << std::endl;
    }

  std::cerr << "Vnl double,1 (7,6,4)" << std::endl;
  if((test_fft<double,1,
      itk::VnlForwardFFTImageFilter<ImageD1> ,
      itk::VnlInverseFFTImageFilter<ImageCD1> >(SizeOfDimensions3)) == 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  else
    {
    std::cerr << "Caught expected size error." << std::endl;
    }

  std::cerr << "Vnl double,2 (7,6,4)" << std::endl;
  if((test_fft<double,2,
      itk::VnlForwardFFTImageFilter<ImageD2> ,
      itk::VnlInverseFFTImageFilter<ImageCD2> >(SizeOfDimensions3)) == 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  else
    {
    std::cerr << "Caught expected size error." << std::endl;
    }

  std::cerr << "Vnl double,3 (7,6,4)" << std::endl;
  if((test_fft<double,3,
      itk::VnlForwardFFTImageFilter<ImageD3> ,
      itk::VnlInverseFFTImageFilter<ImageCD3> >(SizeOfDimensions3)) == 0)
    {
    std::cerr << "--------------------- Failed!" << std::endl;
    rval++;
    }
  else
    {
    std::cerr << "Caught expected size error." << std::endl;
    }

  return rval == 0 ? 0 : -1;
}
