/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDifferenceImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkDifferenceImageFilter_txx
#define _itkDifferenceImageFilter_txx

#include "itkDifferenceImageFilter.h"

#include "itkConstSmartNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace itk
{

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
DifferenceImageFilter<TInputImage, TOutputImage>
::DifferenceImageFilter()
{
  // We require two inputs to execute.
  this->SetNumberOfRequiredInputs(2);
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void 
DifferenceImageFilter<TInputImage, TOutputImage>
::SetValidInput(const InputImageType* validImage)
{
  // The valid image should be input 0.
  this->SetInput(0, validImage);
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void 
DifferenceImageFilter<TInputImage, TOutputImage>
::SetTestInput(const InputImageType* testImage)
{
  // The test image should be input 1.
  this->SetInput(1, testImage);
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType &threadRegion, int threadId)
{
  typedef ConstSmartNeighborhoodIterator<InputImageType> SmartIterator;
  typedef ImageRegionConstIterator<OutputImageType> InputIterator;
  typedef ImageRegionIterator<OutputImageType> OutputIterator;
  typedef NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> FacesCalculator;
  typedef typename FacesCalculator::RadiusType RadiusType;
  typedef typename FacesCalculator::FaceListType FaceListType;
  typedef typename FaceListType::iterator FaceListIterator;
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  
  // Prepare standard boundary condition.
  ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;
  
  // Get a pointer to each image.
  const InputImageType* validImage = this->GetInput(0);
  const InputImageType* testImage = this->GetInput(1);
  OutputImageType* outputPtr = this->GetOutput();
  
  // Create a radius of 1 pixel.
  RadiusType radius;
  radius.Fill(1);
  
  // Find the data-set boundary faces.
  FacesCalculator boundaryCalculator;
  FaceListType faceList = boundaryCalculator(testImage, threadRegion, radius);
  
  // Support progress methods/callbacks.
  ProgressReporter progress(this, threadId, threadRegion.GetNumberOfPixels());
  
  // Process the internal face and each of the boundary faces.
  for (FaceListIterator face = faceList.begin(); face != faceList.end(); ++face)
    { 
    SmartIterator test(radius, testImage, *face); // Iterate over test image.
    InputIterator valid(validImage, *face);       // Iterate over valid image.
    OutputIterator out(outputPtr, *face);         // Iterate over output image.
    test.OverrideBoundaryCondition(&nbc);
    
    for(valid.GoToBegin(), test.GoToBegin(), out.GoToBegin();
        !valid.IsAtEnd();
        ++valid, ++test, ++out)
      {
      // Get the current valid pixel.
      InputPixelType t = valid.Get();
      
      // Find the closest-valued pixel in the neighborhood of the test
      // image.
      OutputPixelType minimumDifference = NumericTraits<OutputPixelType>::max();
      unsigned int neighborhoodSize = test.Size();
      for (unsigned int i=0; i < neighborhoodSize; ++i)
        {
        OutputPixelType difference = t - test.GetPixel(i);
        if(NumericTraits<OutputPixelType>::IsNegative(difference))
          {
          difference = -difference;
          }
        if(difference < minimumDifference)
          {
          minimumDifference = difference;
          }
        }
      
      // Store the minimum difference value in the output image.
      out.Set(minimumDifference);
      progress.CompletedPixel();
      }
    }
}

} // end namespace itk

#endif
