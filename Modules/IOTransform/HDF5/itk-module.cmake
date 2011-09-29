set(DOCUMENTATION "This module contains classes that handle
IO of itkTransform in HDF5 format.")

itk_module(ITKIOTransformHDF5
  DEPENDS
    ITKIOTransformBase
  TEST_DEPENDS
    ITKTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
)
