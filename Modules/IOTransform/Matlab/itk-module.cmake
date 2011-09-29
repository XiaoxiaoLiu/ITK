set(DOCUMENTATION "This module contains classes that handle
IO of itkTransform in Matlab format.")

itk_module(ITKIOTransformMatlab
  DEPENDS
    ITKIOTransformBase
  TEST_DEPENDS
    ITKTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
)
