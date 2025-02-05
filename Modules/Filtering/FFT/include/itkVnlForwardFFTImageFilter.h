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
#ifndef __itkVnlForwardFFTImageFilter_h
#define __itkVnlForwardFFTImageFilter_h

#include "itkForwardFFTImageFilter.h"
#include "vnl/algo/vnl_fft_base.h"

namespace itk
{
/** \class VnlForwardFFTImageFilter
 *
 * \brief VNL based forward Fast Fourier Transform.
 *
 * The input image size must be a multiple of combinations of 2s, 3s,
 * and/or 5s in all dimensions.
 *
 * \ingroup FourierTransform
 *
 * \sa ForwardFFTImageFilter
 * \ingroup ITKFFT
 *
 * \wiki
 * \wikiexample{SpectralAnalysis/VnlForwardFFTImageFilter,Compute the FFT of an image}
 * \wikiexample{SpectralAnalysis/CrossCorrelationInFourierDomain,Compute the cross-correlation of two images in the Fourier domain}
 * \endwiki
 */
template< class TInputImage, class TOutputImage=Image< std::complex<typename TInputImage::PixelType>, TInputImage::ImageDimension> >
class VnlForwardFFTImageFilter:
  public ForwardFFTImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef TInputImage                            InputImageType;
  typedef typename InputImageType::PixelType     InputPixelType;
  typedef typename InputImageType::SizeType      InputSizeType;
  typedef typename InputImageType::SizeValueType InputSizeValueType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::PixelType    OutputPixelType;

  typedef VnlForwardFFTImageFilter                           Self;
  typedef ForwardFFTImageFilter<  TInputImage, TOutputImage> Superclass;
  typedef SmartPointer< Self >                               Pointer;
  typedef SmartPointer< const Self >                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VnlForwardFFTImageFilter,
               ForwardFFTImageFilter);

  /** Extract the dimensionality of the images. They are assumed to be
   * the same. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** These should be defined in every FFT filter class. */
  virtual void GenerateData();

  virtual bool FullMatrix();

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro( ImageDimensionsMatchCheck,
                   ( Concept::SameDimension< InputImageDimension, OutputImageDimension > ) );
  /** End concept checking */
#endif

protected:
  VnlForwardFFTImageFilter() {}
  ~VnlForwardFFTImageFilter() {}

  /** Method to check if an array dimension is legal for prime factor
   * FFT algorithm. */
  bool IsDimensionSizeLegal(InputSizeValueType n);

private:
  VnlForwardFFTImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                          //purposely not implemented

  typedef vnl_vector< vcl_complex< InputPixelType > > SignalVectorType;

  struct vnl_fft_transform:
    public vnl_fft_base< ImageDimension, InputPixelType >
  {
    typedef vnl_fft_base< ImageDimension, InputPixelType > Base;

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
#include "itkVnlForwardFFTImageFilter.hxx"
#endif

#endif
