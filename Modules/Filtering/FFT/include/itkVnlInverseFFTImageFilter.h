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
#ifndef __itkVnlInverseFFTImageFilter_h
#define __itkVnlInverseFFTImageFilter_h

#include "itkInverseFFTImageFilter.h"

#include "itkImage.h"
#include "vnl/algo/vnl_fft_base.h"

namespace itk
{
/** \class VnlInverseFFTImageFilter
 *
 * \brief VNL based reverse Fast Fourier Transform.
 *
 * The input image size must be a multiple of combinations of 2s, 3s,
 * and/or 5s in all dimensions.
 *
 * \ingroup FourierTransform
 *
 * \sa InverseFFTImageFilter
 * \ingroup ITKFFT
 *
 * \wiki
 * \wikiexample{SpectralAnalysis/CrossCorrelationInFourierDomain,Compute the cross-correlation of two images in the Fourier domain}
 * \endwiki
 */
template< class TInputImage, class TOutputImage=Image< typename TInputImage::PixelType::value_type, TInputImage::ImageDimension> >
class VnlInverseFFTImageFilter:
  public InverseFFTImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef TInputImage                            InputImageType;
  typedef typename InputImageType::PixelType     InputPixelType;
  typedef typename InputImageType::SizeType      InputSizeType;
  typedef typename InputImageType::SizeValueType InputSizeValueType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::PixelType    OutputPixelType;
  typedef typename OutputImageType::SizeType     OutputSizeType;

  typedef VnlInverseFFTImageFilter                           Self;
  typedef InverseFFTImageFilter< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer< Self >                               Pointer;
  typedef SmartPointer< const Self >                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VnlInverseFFTImageFilter,
               InverseFFTImageFilter);

  /** Extract the dimensionality of the images. They must be the
   * same. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** These should be defined in every FFT filter class. */
  virtual void GenerateData();  // generates output from input

  virtual bool FullMatrix();

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( PixelUnsignedIntDivisionOperatorsCheck,
                   ( Concept::DivisionOperators< OutputPixelType, unsigned int > ) );
  itkConceptMacro( ImageDimensionsMatchCheck,
                   ( Concept::SameDimension< InputImageDimension, OutputImageDimension > ) );
  /** End concept checking */
#endif

protected:
  VnlInverseFFTImageFilter()  {}
  virtual ~VnlInverseFFTImageFilter(){}

  /** Method to check if an array dimension is legal for the prime
   * factor FFT algorithm. */
  bool IsDimensionSizeLegal(InputSizeValueType n);

private:
  VnlInverseFFTImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                          //purposely not implemented

  typedef vnl_vector< InputPixelType  > SignalVectorType;

  struct vnl_fft_transform:
    public vnl_fft_base< ImageDimension, OutputPixelType >
  {
    typedef vnl_fft_base< ImageDimension, OutputPixelType > Base;

    //: constructor takes size of signal.
    vnl_fft_transform( const InputSizeType & s )
    {
      for( int i=0; i<ImageDimension; i++ )
      {
        Base::factors_[ImageDimension - i - 1].resize(s[i]);
      }
    }
  };

};
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVnlInverseFFTImageFilter.hxx"
#endif

#endif
