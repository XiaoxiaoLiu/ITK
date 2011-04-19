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
#ifndef __itkTemporalProcessObject_cxx
#define __itkTemporalProcessObject_cxx

#include "itkTemporalProcessObject.h"
#include "itkTemporalRegion.h"
#include "itkTemporalDataObject.h"

#include <math.h>

namespace itk
{

//-CONSTRUCTOR PRINT-----------------------------------------------------------

//
// Constructor
//
TemporalProcessObject::TemporalProcessObject()
  : m_UnitInputNumberOfFrames(1),
    m_UnitOutputNumberOfFrames(1),
    m_FrameSkipPerOutput(1),
    m_InputStencilCurrentFrameIndex(0)
{}

//
// PrintSelf
//
void
TemporalProcessObject::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "TemporalProcessObject" << std::endl;
}

//-PROPAGATE REQUESTED REGION CALLBACKS----------------------------------------

//
// EnlargeOutputRequestedRegion
//
void
TemporalProcessObject::EnlargeOutputRequestedRegion(DataObject* output)
{
  // Check that output is a TemporalDataObject
  TemporalDataObject* tOutput = dynamic_cast<TemporalDataObject*>(output);

  if (tOutput)
    {
    this->EnlargeOutputRequestedTemporalRegion(tOutput);
    }
  else
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::EnlargeOutputRequestedRegion() "
                      << "cannot cast " << typeid(output).name() << " to "
                      << typeid(TemporalDataObject*).name() );
    }
}

//
// EnlargeOutputRequestedTemporalRegion
// TODO: Hanle RealTime
//
void
TemporalProcessObject::EnlargeOutputRequestedTemporalRegion(TemporalDataObject* output)
{
  // Make sure the requested output temporal region duration is a multiple of
  // the unit number of output frames
  TemporalRegion outReqTempRegion = output->GetRequestedTemporalRegion();
  unsigned long outFrameDuration = outReqTempRegion.GetFrameDuration();

  //DEBUG
  std::cout << "initial out frame duration = " << outFrameDuration << std::endl;

  unsigned int remainder = outFrameDuration % m_UnitOutputNumberOfFrames;
  if (remainder > 0)
    {
    outFrameDuration += (m_UnitOutputNumberOfFrames - remainder);
    }
  outReqTempRegion.SetFrameDuration(outFrameDuration);
  output->SetRequestedTemporalRegion(outReqTempRegion);
}

//
// GenerateOutputRequestedRegion
//
void
TemporalProcessObject::GenerateOutputRequestedRegion(DataObject* output)
{
  // Check that output is a TemporalDataObject
  TemporalDataObject* tOutput = dynamic_cast<TemporalDataObject*>(output);

  if (tOutput)
    {
    this->GenerateOutputRequestedTemporalRegion(tOutput);
    }
  else
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::GenerateOutputRequestedRegion() "
                      << "cannot cast " << typeid(output).name() << " to "
                      << typeid(TemporalDataObject*).name() );
    }
}

//
// GenerateInputRequestedRegion
//
void
TemporalProcessObject::GenerateInputRequestedRegion()
{
  // Check that output and input are a TemporalDataObjects
  TemporalDataObject* tOutput = dynamic_cast<TemporalDataObject*>(this->GetOutput(0));
  TemporalDataObject* tInput = dynamic_cast<TemporalDataObject*>(this->GetOutput(0));

  if (!tOutput)
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::GenerateInputRequestedRegion() "
                      << "cannot cast " << typeid(this->GetOutput(0)).name() << " to "
                      << typeid(TemporalDataObject*).name() );
    }
  else if (!tInput)
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::GenerateInputRequestedRegion() "
                      << "cannot cast " << typeid(this->GetInput(0)).name() << " to "
                      << typeid(TemporalDataObject*).name() );
    }
  else
    {
    this->GenerateInputRequestedTemporalRegion();
    }
}

//
// GenerateInputRequestedTemporalRegion
// TODO: Hanle RealTime
//
void
TemporalProcessObject::GenerateInputRequestedTemporalRegion()
{
  // This should only get called after verifying that input(0) and output(0)
  // can validly be cast to TemporalDataObjects, so don't check cast here
  TemporalDataObject* input = dynamic_cast<TemporalDataObject*>(this->GetInput(0));
  TemporalDataObject* output = dynamic_cast<TemporalDataObject*>(this->GetOutput(0));

  TemporalRegion outReqTempRegion = output->GetRequestedTemporalRegion();

  // This should always be a whole number because of EnlargeOutputRequestedTemporalRegion
  // but do it safely in case the subclass overrides it
  unsigned long numInputRequests =
    (unsigned long)ceil((double)outReqTempRegion.GetFrameDuration() /
                        (double)m_UnitOutputNumberOfFrames);

  // The number of input requests indicates the number of times the process
  // will have to request a temporal region of size m_UnitInputNumberOfFrames.
  // Each request besides the last will require m_FrameSkipPerOutput new frames
  // to be loaded.
  unsigned long inputDuration = m_FrameSkipPerOutput * (numInputRequests - 1) +
                                  m_UnitInputNumberOfFrames;

  // Compute the start of the input requested temporal region based on
  // m_InputStencilCurrentFrameIndex
  long inputStart = outReqTempRegion.GetFrameStart() - m_InputStencilCurrentFrameIndex;

  // Make sure we're not requesting a negative frame (this may be replaced by
  // boundary conditions at some point)
  if (inputStart < 0)
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::GenerateInputRequestedTemporalRegion() "
                      << "cannot request a region with a starting frame of " << inputStart);
    }

  // Set up the region and assign it to input
  TemporalRegion inReqTempRegion;
  inReqTempRegion.SetFrameStart(inputStart);
  inReqTempRegion.SetFrameDuration(inputDuration);
  input->SetRequestedTemporalRegion(inReqTempRegion);
}

//
// UpdateOutputInformation
// TODO: Hanle RealTime
//
void
TemporalProcessObject::UpdateOutputInformation()
{
  // Update using inherited system
  Superclass::UpdateOutputInformation();

  TemporalDataObject* input = dynamic_cast<TemporalDataObject*>(this->GetInput(0));
  TemporalDataObject* output = dynamic_cast<TemporalDataObject*>(this->GetOutput(0));
  if (!input)
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::GenerateOutputRequestedTemporalRegion() "
                      << "cannot cast " << typeid(input).name() << " to "
                      << typeid(TemporalDataObject*).name() );
    }
  if (!output)
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::GenerateOutputRequestedTemporalRegion() "
                      << "cannot cast " << typeid(output).name() << " to "
                      << typeid(TemporalDataObject*).name() );
    }

  // Compute duration for output largest possible region
  TemporalRegion inputLargestRegion = input->GetLargestPossibleTemporalRegion();
  long scannableDuration = inputLargestRegion.GetFrameDuration() -
                            m_UnitInputNumberOfFrames + 1;
  long outputDuration = m_UnitOutputNumberOfFrames *
    ((double)(scannableDuration - 1) / (double)(m_FrameSkipPerOutput) + 1);

  // Compute the start of the output region
  long outputStart = inputLargestRegion.GetFrameStart() + m_InputStencilCurrentFrameIndex;

  // Set up output largets possible region
  TemporalRegion largestRegion = output->GetLargestPossibleTemporalRegion();
  largestRegion.SetFrameDuration(outputDuration);
  largestRegion.SetFrameStart(outputStart);
  output->SetLargestPossibleTemporalRegion(largestRegion);
}


//-TEMPORAL STREAMING----------------------------------------------------------

//
// GenerateData
//
void
TemporalProcessObject::GenerateData()
{
  // Call Pre-processing method
  this->BeforeTemporalStreamingGenerateData();

  // Split up the requested output temporal region
  std::vector<TemporalRegion> inputTemporalRegionRequests = this->SplitRequestedTemporalRegion();

  //DEBUG
  std::cout << "inputTemporalRegionRequests split up into " << inputTemporalRegionRequests.size()
            << " requests" << std::endl;

  // Get the first output frame location
  TemporalDataObject* output = dynamic_cast<TemporalDataObject*>(this->GetOutput(0));
  if (!output)
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::GenerateData() "
                      << "cannot cast " << typeid(output).name() << " to "
                      << typeid(TemporalDataObject*).name() );
    }
  unsigned long outputStartFrame = output->GetUnbufferedRequestedTemporalRegion().GetFrameStart();

  // Process each of the temporal sub-regions in sequence
  for (unsigned int i = 0; i < inputTemporalRegionRequests.size(); ++i)
    {
    // Set Input's requested region to the new reqest at i
    TemporalDataObject* input = dynamic_cast<TemporalDataObject*>(this->GetInput(0));
    if (!input)
      {
      itkExceptionMacro(<< "itk::TemporalProcessObject::GenerateData() "
                        << "cannot cast " << typeid(input).name() << " to "
                        << typeid(TemporalDataObject*).name() );
      }
    input->SetRequestedTemporalRegion(inputTemporalRegionRequests[i]);

    // Call Input's UpdateOutputData()
    input->UpdateOutputData();

    // Call TemporalStreamingGenerateData to process the newly acquired data
    this->TemporalStreamingGenerateData(outputStartFrame);

    // Increment outputStartFrame
    outputStartFrame += this->m_UnitOutputNumberOfFrames;
    }

  // Call post-processing method
  this->AfterTemporalStreamingGenerateData();
}


//
// TemporalStreamingGenerateData
//
void
TemporalProcessObject::TemporalStreamingGenerateData(unsigned long outputFrameStart)
{
  itkExceptionMacro(<< "itk::Error: " << this->GetNameOfClass()
                    << "(" << this << "): Subclass should override this method!!!" );
}


//
// SplitRequestedTemporalRegion
// TODO: Hanle RealTime
//
std::vector<TemporalRegion>
TemporalProcessObject::SplitRequestedTemporalRegion()
{

  // Get the current output TemporalDataObject
  TemporalDataObject* outputObject = dynamic_cast<TemporalDataObject*>(this->GetOutput(0));
  if (!outputObject)
    {
    itkExceptionMacro(<< "itk::TemporalProcessObject::SplitRequestedTemporalRegion() "
                      << "cannot cast " << typeid(outputObject).name() << " to "
                      << typeid(TemporalDataObject*).name() );
    }

  // Get the TemporalRegion representing the difference between the output's
  // requested temporal region and its buffered temporal region. This
  // difference is defined as any time that is covered by the requested region
  // but not by the buffered region
  TemporalRegion unbufferedRegion = outputObject->GetUnbufferedRequestedTemporalRegion();

  // Calculate the number of input requests that will be needed
  unsigned long numRequests = (unsigned long)(ceil(
                                              (double)(unbufferedRegion.GetFrameDuration() /
                                              (double)(m_UnitOutputNumberOfFrames)) ));

  //DEBUG
  std::cout << "unbuffered region size = " << unbufferedRegion.GetFrameDuration() << std::endl;
  std::cout << "unit output = " << m_UnitOutputNumberOfFrames << std::endl;
  std::cout << "num requests = " << numRequests << std::endl;

  // Calculate left extra frames that will be requested (might be unnecessary)
  //unsigned long extraFrames = (numRequests * m_UnitOutputNumberOfFrames) -
  //                              unbufferedRegion.GetFrameDuration();

  // Set up the requested input temporal region set (TODO: NOT PROPERLY HANDLING REAL TIME!!!!!!!!)
  std::vector<TemporalRegion> inputTemporalRegionRequests;

  unsigned long regionStartFrame = 1;
  if (this->m_FrameSkipPerOutput > 0)
    {
    regionStartFrame = unbufferedRegion.GetFrameStart();
    }
  else if (this->m_FrameSkipPerOutput < 0)
    {
    regionStartFrame = unbufferedRegion.GetFrameStart() + unbufferedRegion.GetFrameDuration() -
                        this->m_UnitOutputNumberOfFrames;
    }
  for (unsigned int i = 0; i < numRequests; ++i)
    {
    // Create the requested region
    TemporalRegion r;
    r.SetFrameStart(regionStartFrame);
    r.SetFrameDuration(this->m_UnitInputNumberOfFrames);
    inputTemporalRegionRequests.push_back(r);

    // Move the start position for the next requested region
    regionStartFrame += this->m_FrameSkipPerOutput;
    }

  // Return the set of regions
  return inputTemporalRegionRequests;

}

} // end namespace itk

#endif
