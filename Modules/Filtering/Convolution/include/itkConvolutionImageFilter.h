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
#ifndef __itkConvolutionImageFilter_h
#define __itkConvolutionImageFilter_h

#include "itkImageToImageFilter.h"

#include "itkProgressAccumulator.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace itk
{
/** \class ConvolutionImageFilter
 * \brief Convolve a given image with an arbitrary image kernel.
 *
 * This filter operates by centering the flipped kernel at each pixel
 * in the image and computing the inner product between pixel values
 * in the image and pixel values in the kernel. The center of the
 * kernel is defined as \f$ \lfloor (2*i+s-1)/2 \rfloor \f$ where
 * \f$i\f$ is the index and \f$s\f$ is the size of the largest
 * possible region of the kernel image. For kernels with odd sizes in
 * all dimensions, this corresponds to the center pixel. If a
 * dimension of the kernel image has an even size, then the center
 * index of the kernel in that dimension will be the largest integral
 * index that is less than the continuous index of the image center.
 *
 * The kernel can optionally be normalized to sum to 1 using
 * NormalizeOn(). Normalization is off by default.
 *
 * \warning This filter ignores the spacing, origin, and orientation
 * of the kernel image and treats them as identical to those in the
 * input image.
 *
 * This code was contributed in the Insight Journal paper:
 *
 * "Image Kernel Convolution"
 * by Tustison N., Gee J.
 * http://hdl.handle.net/1926/1323
 * http://www.insight-journal.org/browse/publication/208
 *
 * \author Nicholas J. Tustison
 * \author James C. Gee
 * \ingroup ITKConvolution
 *
 * \wiki
 * \wikiexample{ImageProcessing/ConvolutionImageFilter,Convolve an image with a kernel}
 * \endwiki
 */
template< class TInputImage, class TKernelImage = TInputImage, class TOutputImage = TInputImage >
class ITK_EXPORT ConvolutionImageFilter :
  public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  typedef ConvolutionImageFilter                          Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information ( and related methods ) */
  itkTypeMacro(ConvolutionImageFilter, ImageToImageFilter);

  /** Dimensionality of input and output data is assumed to be the same. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  typedef TInputImage                          InputImageType;
  typedef TOutputImage                         OutputImageType;
  typedef TKernelImage                         KernelImageType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename OutputImageType::PixelType  OutputPixelType;
  typedef typename KernelImageType::PixelType  KernelPixelType;
  typedef typename InputImageType::IndexType   InputIndexType;
  typedef typename OutputImageType::IndexType  OutputIndexType;
  typedef typename KernelImageType::IndexType  KernelIndexType;
  typedef typename InputImageType::SizeType    InputSizeType;
  typedef typename OutputImageType::SizeType   OutputSizeType;
  typedef typename KernelImageType::SizeType   KernelSizeType;
  typedef typename InputImageType::RegionType  InputRegionType;
  typedef typename OutputImageType::RegionType OutputRegionType;
  typedef typename KernelImageType::RegionType KernelRegionType;

  /** Typedef to describe the boundary condition. */
  typedef ImageBoundaryCondition< TInputImage >           BoundaryConditionType;
  typedef BoundaryConditionType *                         BoundaryConditionPointerType;
  typedef ZeroFluxNeumannBoundaryCondition< TInputImage > DefaultBoundaryConditionType;

  /** Set/get the boundary condition. */
  itkSetMacro(BoundaryCondition, BoundaryConditionPointerType);
  itkGetConstMacro(BoundaryCondition, BoundaryConditionPointerType);

  /** Set/get the image kernel. */
  itkSetInputMacro(ImageKernel, KernelImageType);
  itkGetInputMacro(ImageKernel, KernelImageType);

  /** Normalize the output image by the sum of the kernel
   * components. Defaults to off. */
  itkSetMacro(Normalize, bool);
  itkGetConstMacro(Normalize, bool);
  itkBooleanMacro(Normalize);

  typedef enum
  {
    SAME = 0,
    VALID
  } OutputRegionModeType;

  /** Sets the output region mode. If set to SAME, the output region
   * will be the same as the input region, and regions of the image
   * near the boundaries will contain contributions from outside the
   * input image as determined by the boundary condition set in
   * SetBoundaryCondition(). If set to VALID, the output region
   * consists of pixels computed only from pixels in the input image
   * (no extrapolated contributions from the boundary condition are
   * needed). The output is therefore smaller than the input
   * region. Default output region mode is SAME. */
  itkSetEnumMacro(OutputRegionMode, OutputRegionModeType);
  itkGetEnumMacro(OutputRegionMode, OutputRegionModeType);
  virtual void SetOutputRegionModeToSame();
  virtual void SetOutputRegionModeToValid();

  /** ConvolutionImageFilter needs the entire image kernel, which in
   * general is going to be a different size then the output requested
   * region. As such, this filter needs to provide an implementation
   * for GenerateInputRequestedRegion() in order to inform the
   * pipeline execution model.
   *
   * \sa ProcessObject::GenerateInputRequestedRegion()  */
  virtual void GenerateInputRequestedRegion();

protected:
  ConvolutionImageFilter();
  ~ConvolutionImageFilter() {}

  void PrintSelf(std::ostream & os, Indent indent) const;

  /** The largest possible output region may differ from the largest
   * possible input region. */
  void GenerateOutputInformation();

  /** This filter uses a minipipeline to compute the output. */
  void GenerateData();

  /** The kernel needs padding if any of the sizes of its dimensions is
   * even. This method checks for this condition. */
  bool GetKernelNeedsPadding() const;

  /** Calculates the padding width needed to make each dimension odd. */
  KernelSizeType GetKernelPadSize() const;

  /** Calculates the radius of the kernel. */
  template< class TImage >
  KernelSizeType GetKernelRadius(const TImage *kernelImage) const;

  /** Get the valid region of the convolution. */
  OutputRegionType GetValidRegion() const;

  /** Default superclass implementation ensures that input images
   * occupy same physical space. This is not needed for this filter. */
  virtual void VerifyInputInformation() {};

private:
  ConvolutionImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);         //purposely not implemented

  template< class TImage >
  void ComputeConvolution( const TImage *kernelImage,
                           ProgressAccumulator *progress );

  bool m_Normalize;

  DefaultBoundaryConditionType m_DefaultBoundaryCondition;
  BoundaryConditionPointerType m_BoundaryCondition;

  OutputRegionModeType m_OutputRegionMode;
};
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkConvolutionImageFilter.hxx"
#endif

#endif
