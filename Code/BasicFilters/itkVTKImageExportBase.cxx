/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVTKImageExportBase.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkVTKImageExportBase.h"
#include "itkCommand.h"

namespace itk
{

/**
 * Constructor sets up information for the image-type indepenedent
 * callbacks implemented in this superclass.
 */
VTKImageExportBase::VTKImageExportBase()
{
  m_LastPipelineMTime = 0;
}

void VTKImageExportBase::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "Last Pipeline MTime: "
     << m_LastPipelineMTime<< std::endl;  
}

//----------------------------------------------------------------------------
void* VTKImageExportBase::GetCallbackUserData()
{
  return this;
}

VTKImageExportBase::UpdateInformationCallbackType
VTKImageExportBase::GetUpdateInformationCallback() const
{
  return &VTKImageExportBase::UpdateInformationCallbackFunction;
}

VTKImageExportBase::PipelineModifiedCallbackType
VTKImageExportBase::GetPipelineModifiedCallback() const
{
  return &VTKImageExportBase::PipelineModifiedCallbackFunction;
}

VTKImageExportBase::WholeExtentCallbackType
VTKImageExportBase::GetWholeExtentCallback() const
{
  return &VTKImageExportBase::WholeExtentCallbackFunction;
}

VTKImageExportBase::SpacingCallbackType
VTKImageExportBase::GetSpacingCallback() const
{
  return &VTKImageExportBase::SpacingCallbackFunction;
}

VTKImageExportBase::OriginCallbackType
VTKImageExportBase::GetOriginCallback() const
{
  return &VTKImageExportBase::OriginCallbackFunction;
}

VTKImageExportBase::ScalarTypeCallbackType
VTKImageExportBase::GetScalarTypeCallback() const
{
  return &VTKImageExportBase::ScalarTypeCallbackFunction;
}

VTKImageExportBase::NumberOfComponentsCallbackType
VTKImageExportBase::GetNumberOfComponentsCallback() const
{
  return &VTKImageExportBase::NumberOfComponentsCallbackFunction;
}

VTKImageExportBase::PropagateUpdateExtentCallbackType
VTKImageExportBase::GetPropagateUpdateExtentCallback() const
{
  return &VTKImageExportBase::PropagateUpdateExtentCallbackFunction;
}

VTKImageExportBase::UpdateDataCallbackType
VTKImageExportBase::GetUpdateDataCallback() const
{
  return &VTKImageExportBase::UpdateDataCallbackFunction;
}

VTKImageExportBase::DataExtentCallbackType
VTKImageExportBase::GetDataExtentCallback() const
{
  return &VTKImageExportBase::DataExtentCallbackFunction;
}

VTKImageExportBase::BufferPointerCallbackType
VTKImageExportBase::GetBufferPointerCallback() const
{
  return &VTKImageExportBase::BufferPointerCallbackFunction;
}


//----------------------------------------------------------------------------

/**
 * Implements the UpdateInformationCallback.  This sends
 * UpdateOutputInformation() through the ITK pipeline, which is the
 * equivalent to VTK's UpdateInformation().
 */
void VTKImageExportBase::UpdateInformationCallback()
{
  this->UpdateOutputInformation();
}


/**
 * Implements the PipelineModifiedCallback.  This returns 1 if the
 * ITK pipeline has been modified since the last time this callback was
 * invoked.  If the pipeline has not been modified, this returns 0.
 */
int VTKImageExportBase::PipelineModifiedCallback()
{
  DataObjectPointer input = this->GetInput(0);
  unsigned long pipelineMTime = input->GetPipelineMTime();
  if(pipelineMTime > m_LastPipelineMTime)
    {
    m_LastPipelineMTime = pipelineMTime;
    return 1;
    }
  else
    {
    return 0;
    }
}


/**
 * Implements the UpdateDataCallback.  This sends and Update() call
 * through the ITK pipeline.
 */
void VTKImageExportBase::UpdateDataCallback()
{
  // Get the input.
  DataObjectPointer input = this->GetInput(0);
  
  // Notify start event observers
  this->InvokeEvent( StartEvent() );

  // Make sure input is up to date.
  input->Update();
  
  // Notify end event observers
  this->InvokeEvent( EndEvent() );
}

//----------------------------------------------------------------------------
void VTKImageExportBase::UpdateInformationCallbackFunction(void* userData)
{
  static_cast<VTKImageExportBase*>(userData)->
    UpdateInformationCallback();
}

int VTKImageExportBase::PipelineModifiedCallbackFunction(void* userData)
{
  return static_cast<VTKImageExportBase*>(userData)->
    PipelineModifiedCallback();
}

int* VTKImageExportBase::WholeExtentCallbackFunction(void* userData)
{
  return static_cast<VTKImageExportBase*>(userData)->
    WholeExtentCallback();
}

VTKSpacingType * VTKImageExportBase::SpacingCallbackFunction(void* userData)
{
  return static_cast<VTKImageExportBase*>(userData)->
    SpacingCallback();
}

VTKOriginType * VTKImageExportBase::OriginCallbackFunction(void* userData)
{
  return static_cast<VTKImageExportBase*>(userData)->
    OriginCallback();
}

const char* VTKImageExportBase::ScalarTypeCallbackFunction(void* userData)
{
  return static_cast<VTKImageExportBase*>(userData)->
    ScalarTypeCallback();
}
 
int VTKImageExportBase::NumberOfComponentsCallbackFunction(void* userData)
{
  return static_cast<VTKImageExportBase*>(userData)->
    NumberOfComponentsCallback();
}

void VTKImageExportBase::PropagateUpdateExtentCallbackFunction(void* userData,
                                                               int* extent)
{
  static_cast<VTKImageExportBase*>(userData)->
    PropagateUpdateExtentCallback(extent);
}

void VTKImageExportBase::UpdateDataCallbackFunction(void* userData)
{
  static_cast<VTKImageExportBase*>(userData)->
    UpdateDataCallback();
}

int* VTKImageExportBase::DataExtentCallbackFunction(void* userData)
{
  return static_cast<VTKImageExportBase*>(userData)->
    DataExtentCallback();
}

void* VTKImageExportBase::BufferPointerCallbackFunction(void* userData)
{
  return static_cast<VTKImageExportBase*>(userData)->
    BufferPointerCallback();
}


} // end namespace itk
