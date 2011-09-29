set(DOCUMENTATION "This module contains the base classes for handling
various itkTransfrom IO formats.")

itk_module(ITKIOTransformBase
  DEPENDS
    ITKCommon
  TEST_DEPENDS
    ITKTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
)
