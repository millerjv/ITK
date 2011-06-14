IF(ITK_USE_GPU)

create_test_sourcelist(Tests ITKGPUCommon-tests.cxx
itkGPUImageTest.cxx
itkGPUImageFilterTest.cxx
itkGPUMeanImageFilterTest.cxx
itkGPUBinaryThresholdImageFilterTest.cxx
itkGPUGradientAnisotropicDiffusionImageFilterTest.cxx
itkGPUDemonsRegistrationFilterTest.cxx
)

add_executable(ITKGPUCommon-tests  ${Tests} )
itk_module_target_label(ITKGPUCommon-tests)
target_link_libraries(ITKGPUCommon-tests  ${ITKGPUCommon_LIBRARIES} )

itk_add_test(NAME itkGPUImageFilterTest
        COMMAND ITKGPUCommon-tests itkGPUImageFilterTest
        ${ITK_EXAMPLE_DATA_ROOT}/BrainProtonDensitySlice.png
        ${TEMP}/gpuImageFilterTest.png)

#add_test(NAME itkGPUImageTest COMMAND ITK-GPUCommonTestDriver itkGPUImageTest)
#
#temporary test
#add_test(NAME itkGPUImageFilterTestTemp
#        COMMAND ITK-GPUCommonTestDriver itkGPUImageFilterTestTemp
#        ${ITK_SOURCE_DIR}/Examples/Data/BrainProtonDensitySlice.png
#        ${TEMP}/gpuImageFilterTest.png)

add_test(NAME itkGPUMeanImageFilterTest
        COMMAND ITK-GPUCommonTestDriver itkGPUMeanImageFilterTest
        ${ITK_SOURCE_DIR}/Examples/Data/BrainProtonDensitySlice.png
        ${TEMP}/gpuMeanImageFilterTest.png)

add_test(NAME itkGPUBinaryThresholdImageFilterTest
        COMMAND ITK-GPUCommonTestDriver itkGPUBinaryThresholdImageFilterTest
        ${ITK_SOURCE_DIR}/Examples/Data/BrainProtonDensitySlice.png
        ${TEMP}/gpuBinaryThresholdImageFilterTest.png)

add_test(NAME itkGPUGradientAnisotropicDiffusionImageFilterTest
        COMMAND ITK-GPUCommonTestDriver itkGPUGradientAnisotropicDiffusionImageFilterTest
        ${ITK_SOURCE_DIR}/Examples/Data/BrainProtonDensitySlice.png
        ${TEMP}/gpuGradientAnisotropicDiffusionImageFilterTest.png)

add_test(NAME itkGPUDemonsRegistrationFilterTest
        COMMAND ITK-GPUCommonTestDriver itkGPUDemonsRegistrationFilterTest
        ${ITK_SOURCE_DIR}/Examples/Data/BrainProtonDensitySlice.png
        ${TEMP}/gpuImageFilterTest.png
        ${TEMP}/output)


ENDIF(ITK_USE_GPU)
