itk_module_test()
set(ITKIOTransformBaseTests
itkTransformFactoryBaseTest.cxx
itkTransformIOTest.cxx
)


CreateTestDriver(ITKIOTransformBase  "${ITKIOTransformBase-Test_LIBRARIES}" "${ITKIOTransformBaseTests}")
