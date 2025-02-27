#ifdef IGLO_VULKAN

#if 0
; SPIR-V
; Version: 1.0
; Generator: Google spiregg; 0
; Bound: 63
; Schema: 0
               OpCapability Shader
               OpCapability RuntimeDescriptorArray
               OpExtension "SPV_EXT_descriptor_indexing"
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %CSMain "CSMain" %gl_GlobalInvocationID
               OpExecutionMode %CSMain LocalSize 8 8 1
               OpSource HLSL 660
               OpName %type_PushConstants "type.PushConstants"
               OpMemberName %type_PushConstants 0 "srcTextureIndex"
               OpMemberName %type_PushConstants 1 "destTextureIndex"
               OpMemberName %type_PushConstants 2 "bilinearClampSamplerIndex"
               OpMemberName %type_PushConstants 3 "padding"
               OpMemberName %type_PushConstants 4 "inverseDestTextureSize"
               OpName %PushConstants "PushConstants"
               OpName %type_2d_image "type.2d.image"
               OpName %ResourceDescriptorHeap "ResourceDescriptorHeap"
               OpName %type_2d_image_0 "type.2d.image"
               OpName %ResourceDescriptorHeap_0 "ResourceDescriptorHeap"
               OpName %type_sampler "type.sampler"
               OpName %SamplerDescriptorHeap "SamplerDescriptorHeap"
               OpName %CSMain "CSMain"
               OpName %type_sampled_image "type.sampled.image"
               OpDecorate %gl_GlobalInvocationID BuiltIn GlobalInvocationId
               OpDecorate %PushConstants DescriptorSet 0
               OpDecorate %PushConstants Binding 0
               OpDecorate %ResourceDescriptorHeap DescriptorSet 0
               OpDecorate %ResourceDescriptorHeap Binding 1
               OpDecorate %ResourceDescriptorHeap_0 DescriptorSet 0
               OpDecorate %ResourceDescriptorHeap_0 Binding 1
               OpDecorate %SamplerDescriptorHeap DescriptorSet 0
               OpDecorate %SamplerDescriptorHeap Binding 2
               OpMemberDecorate %type_PushConstants 0 Offset 0
               OpMemberDecorate %type_PushConstants 1 Offset 4
               OpMemberDecorate %type_PushConstants 2 Offset 8
               OpMemberDecorate %type_PushConstants 3 Offset 12
               OpMemberDecorate %type_PushConstants 4 Offset 16
               OpDecorate %type_PushConstants Block
        %int = OpTypeInt 32 1
      %int_0 = OpConstant %int 0
      %int_1 = OpConstant %int 1
      %int_2 = OpConstant %int 2
      %float = OpTypeFloat 32
  %float_0_5 = OpConstant %float 0.5
    %v2float = OpTypeVector %float 2
         %19 = OpConstantComposite %v2float %float_0_5 %float_0_5
      %int_4 = OpConstant %int 4
    %float_0 = OpConstant %float 0
       %uint = OpTypeInt 32 0
%type_PushConstants = OpTypeStruct %uint %uint %uint %uint %v2float
%_ptr_Uniform_type_PushConstants = OpTypePointer Uniform %type_PushConstants
     %v3uint = OpTypeVector %uint 3
%_ptr_Input_v3uint = OpTypePointer Input %v3uint
%type_2d_image = OpTypeImage %float 2D 2 0 0 1 Unknown
%_runtimearr_type_2d_image = OpTypeRuntimeArray %type_2d_image
%_ptr_UniformConstant__runtimearr_type_2d_image = OpTypePointer UniformConstant %_runtimearr_type_2d_image
%type_2d_image_0 = OpTypeImage %float 2D 2 0 0 2 Rgba32f
%_runtimearr_type_2d_image_0 = OpTypeRuntimeArray %type_2d_image_0
%_ptr_UniformConstant__runtimearr_type_2d_image_0 = OpTypePointer UniformConstant %_runtimearr_type_2d_image_0
%type_sampler = OpTypeSampler
%_runtimearr_type_sampler = OpTypeRuntimeArray %type_sampler
%_ptr_UniformConstant__runtimearr_type_sampler = OpTypePointer UniformConstant %_runtimearr_type_sampler
       %void = OpTypeVoid
         %33 = OpTypeFunction %void
    %v4float = OpTypeVector %float 4
%_ptr_Uniform_uint = OpTypePointer Uniform %uint
%_ptr_UniformConstant_type_2d_image = OpTypePointer UniformConstant %type_2d_image
%_ptr_UniformConstant_type_2d_image_0 = OpTypePointer UniformConstant %type_2d_image_0
%_ptr_UniformConstant_type_sampler = OpTypePointer UniformConstant %type_sampler
     %v2uint = OpTypeVector %uint 2
%_ptr_Uniform_v2float = OpTypePointer Uniform %v2float
%type_sampled_image = OpTypeSampledImage %type_2d_image
%PushConstants = OpVariable %_ptr_Uniform_type_PushConstants Uniform
%gl_GlobalInvocationID = OpVariable %_ptr_Input_v3uint Input
%ResourceDescriptorHeap = OpVariable %_ptr_UniformConstant__runtimearr_type_2d_image UniformConstant
%ResourceDescriptorHeap_0 = OpVariable %_ptr_UniformConstant__runtimearr_type_2d_image_0 UniformConstant
%SamplerDescriptorHeap = OpVariable %_ptr_UniformConstant__runtimearr_type_sampler UniformConstant
     %CSMain = OpFunction %void None %33
         %41 = OpLabel
         %42 = OpLoad %v3uint %gl_GlobalInvocationID
         %43 = OpAccessChain %_ptr_Uniform_uint %PushConstants %int_0
         %44 = OpLoad %uint %43
         %45 = OpAccessChain %_ptr_UniformConstant_type_2d_image %ResourceDescriptorHeap %44
         %46 = OpLoad %type_2d_image %45
         %47 = OpAccessChain %_ptr_Uniform_uint %PushConstants %int_1
         %48 = OpLoad %uint %47
         %49 = OpAccessChain %_ptr_UniformConstant_type_2d_image_0 %ResourceDescriptorHeap_0 %48
         %50 = OpLoad %type_2d_image_0 %49
         %51 = OpAccessChain %_ptr_Uniform_uint %PushConstants %int_2
         %52 = OpLoad %uint %51
         %53 = OpAccessChain %_ptr_UniformConstant_type_sampler %SamplerDescriptorHeap %52
         %54 = OpLoad %type_sampler %53
         %55 = OpVectorShuffle %v2uint %42 %42 0 1
         %56 = OpConvertUToF %v2float %55
         %57 = OpFAdd %v2float %56 %19
         %58 = OpAccessChain %_ptr_Uniform_v2float %PushConstants %int_4
         %59 = OpLoad %v2float %58
         %60 = OpFMul %v2float %57 %59
         %61 = OpSampledImage %type_sampled_image %46 %54
         %62 = OpImageSampleExplicitLod %v4float %61 %60 Lod %float_0
               OpImageWrite %50 %55 %62 None
               OpReturn
               OpFunctionEnd

#endif

const unsigned char g_CS_GenerateMipmaps[] = {
  0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00,
  0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0xb6, 0x14, 0x00, 0x00,
  0x0a, 0x00, 0x08, 0x00, 0x53, 0x50, 0x56, 0x5f, 0x45, 0x58, 0x54, 0x5f,
  0x64, 0x65, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x6f, 0x72, 0x5f, 0x69,
  0x6e, 0x64, 0x65, 0x78, 0x69, 0x6e, 0x67, 0x00, 0x0e, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x06, 0x00,
  0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x53, 0x4d, 0x61,
  0x69, 0x6e, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x06, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00,
  0x05, 0x00, 0x00, 0x00, 0x94, 0x02, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x50, 0x75, 0x73,
  0x68, 0x43, 0x6f, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x74, 0x73, 0x00, 0x00,
  0x06, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x73, 0x72, 0x63, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x49, 0x6e,
  0x64, 0x65, 0x78, 0x00, 0x06, 0x00, 0x08, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74, 0x54, 0x65, 0x78, 0x74,
  0x75, 0x72, 0x65, 0x49, 0x6e, 0x64, 0x65, 0x78, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x0a, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x62, 0x69, 0x6c, 0x69, 0x6e, 0x65, 0x61, 0x72, 0x43, 0x6c, 0x61, 0x6d,
  0x70, 0x53, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x49, 0x6e, 0x64, 0x65,
  0x78, 0x00, 0x00, 0x00, 0x06, 0x00, 0x05, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x70, 0x61, 0x64, 0x64, 0x69, 0x6e, 0x67, 0x00,
  0x06, 0x00, 0x09, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x69, 0x6e, 0x76, 0x65, 0x72, 0x73, 0x65, 0x44, 0x65, 0x73, 0x74, 0x54,
  0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x53, 0x69, 0x7a, 0x65, 0x00, 0x00,
  0x05, 0x00, 0x06, 0x00, 0x04, 0x00, 0x00, 0x00, 0x50, 0x75, 0x73, 0x68,
  0x43, 0x6f, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x74, 0x73, 0x00, 0x00, 0x00,
  0x05, 0x00, 0x06, 0x00, 0x05, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65,
  0x2e, 0x32, 0x64, 0x2e, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x00, 0x00, 0x00,
  0x05, 0x00, 0x08, 0x00, 0x06, 0x00, 0x00, 0x00, 0x52, 0x65, 0x73, 0x6f,
  0x75, 0x72, 0x63, 0x65, 0x44, 0x65, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74,
  0x6f, 0x72, 0x48, 0x65, 0x61, 0x70, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00,
  0x07, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x32, 0x64, 0x2e,
  0x69, 0x6d, 0x61, 0x67, 0x65, 0x00, 0x00, 0x00, 0x05, 0x00, 0x08, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x52, 0x65, 0x73, 0x6f, 0x75, 0x72, 0x63, 0x65,
  0x44, 0x65, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x6f, 0x72, 0x48, 0x65,
  0x61, 0x70, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x09, 0x00, 0x00, 0x00,
  0x74, 0x79, 0x70, 0x65, 0x2e, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72,
  0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x08, 0x00, 0x0a, 0x00, 0x00, 0x00,
  0x53, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x44, 0x65, 0x73, 0x63, 0x72,
  0x69, 0x70, 0x74, 0x6f, 0x72, 0x48, 0x65, 0x61, 0x70, 0x00, 0x00, 0x00,
  0x05, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x53, 0x4d, 0x61,
  0x69, 0x6e, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x0b, 0x00, 0x00, 0x00,
  0x74, 0x79, 0x70, 0x65, 0x2e, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x64,
  0x2e, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0x47, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x47, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x47, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00,
  0x21, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x48, 0x00, 0x05, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x23, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x47, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x15, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x2b, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x17, 0x00, 0x04, 0x00,
  0x12, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x2c, 0x00, 0x05, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
  0x11, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x2b, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x16, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x07, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00,
  0x16, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x04, 0x00, 0x17, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x16, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
  0x19, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x19, 0x00, 0x09, 0x00, 0x05, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1d, 0x00, 0x03, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x04, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1a, 0x00, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00, 0x07, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x03, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0x07, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x1d, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x02, 0x00,
  0x09, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x03, 0x00, 0x1e, 0x00, 0x00, 0x00,
  0x09, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x1f, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x21, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x22, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
  0x23, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x04, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x05, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x25, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
  0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
  0x17, 0x00, 0x04, 0x00, 0x27, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x28, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x03, 0x00,
  0x0b, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00,
  0x17, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x3b, 0x00, 0x04, 0x00, 0x19, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x1b, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00,
  0x1d, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x3b, 0x00, 0x04, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x20, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00,
  0xf8, 0x00, 0x02, 0x00, 0x29, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00,
  0x18, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x41, 0x00, 0x05, 0x00, 0x23, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00,
  0x16, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00,
  0x41, 0x00, 0x05, 0x00, 0x24, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00,
  0x05, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x2d, 0x00, 0x00, 0x00,
  0x41, 0x00, 0x05, 0x00, 0x23, 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00,
  0x16, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, 0x00,
  0x41, 0x00, 0x05, 0x00, 0x25, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00,
  0x07, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00,
  0x41, 0x00, 0x05, 0x00, 0x23, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00,
  0x16, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00,
  0x41, 0x00, 0x05, 0x00, 0x26, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00,
  0x0a, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00,
  0x09, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00,
  0x4f, 0x00, 0x07, 0x00, 0x27, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00,
  0x2a, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x70, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00,
  0x38, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x00,
  0x12, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
  0x13, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x28, 0x00, 0x00, 0x00,
  0x3a, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
  0x3d, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00,
  0x3a, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, 0x12, 0x00, 0x00, 0x00,
  0x3c, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00,
  0x56, 0x00, 0x05, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00,
  0x2e, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x58, 0x00, 0x07, 0x00,
  0x22, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x00, 0x00,
  0x3c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00,
  0x63, 0x00, 0x05, 0x00, 0x32, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00,
  0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00,
  0x38, 0x00, 0x01, 0x00
};

#else

#if 0
;
; Note: shader requires additional functionality:
;       Resource descriptor heap indexing
;       Sampler descriptor heap indexing
;
;
; Input signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
;
; Output signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
; shader hash: 6f1489da605e3a5927c9da4efc96b162
;
; Pipeline Runtime Information: 
;
;PSVRuntimeInfo:
; Compute Shader
; NumThreads=(8,8,1)
; MinimumExpectedWaveLaneCount: 0
; MaximumExpectedWaveLaneCount: 4294967295
; UsesViewID: false
; SigInputElements: 0
; SigOutputElements: 0
; SigPatchConstOrPrimElements: 0
; SigInputVectors: 0
; SigOutputVectors[0]: 0
; SigOutputVectors[1]: 0
; SigOutputVectors[2]: 0
; SigOutputVectors[3]: 0
; EntryFunctionName: CSMain
;
;
; Buffer Definitions:
;
; cbuffer 
; {
;
;   [24 x i8] (type annotation not present)
;
; }
;
;
; Resource Bindings:
;
; Name                                 Type  Format         Dim      ID      HLSL Bind  Count
; ------------------------------ ---------- ------- ----------- ------- -------------- ------
;                                   cbuffer      NA          NA     CB0            cb0     1
;
target datalayout = "e-m:e-p:32:32-i1:32-i8:32-i16:32-i32:32-i64:64-f16:32-f32:32-f64:64-n8:16:32:64"
target triple = "dxil-ms-dx"

%dx.types.Handle = type { i8* }
%dx.types.ResBind = type { i32, i32, i32, i8 }
%dx.types.ResourceProperties = type { i32, i32 }
%dx.types.CBufRet.i32 = type { i32, i32, i32, i32 }
%dx.types.CBufRet.f32 = type { float, float, float, float }
%dx.types.ResRet.f32 = type { float, float, float, float, i32 }
%PushConstants = type { i32, i32, i32, i32, <2 x float> }

define void @CSMain() {
  %1 = call %dx.types.Handle @dx.op.createHandleFromBinding(i32 217, %dx.types.ResBind { i32 0, i32 0, i32 0, i8 2 }, i32 0, i1 false)  ; CreateHandleFromBinding(bind,index,nonUniformIndex)
  %2 = call %dx.types.Handle @dx.op.annotateHandle(i32 216, %dx.types.Handle %1, %dx.types.ResourceProperties { i32 13, i32 24 })  ; AnnotateHandle(res,props)  resource: CBuffer
  %3 = call i32 @dx.op.threadId.i32(i32 93, i32 0)  ; ThreadId(component)
  %4 = call i32 @dx.op.threadId.i32(i32 93, i32 1)  ; ThreadId(component)
  %5 = call %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32 59, %dx.types.Handle %2, i32 0)  ; CBufferLoadLegacy(handle,regIndex)
  %6 = extractvalue %dx.types.CBufRet.i32 %5, 0
  %7 = call %dx.types.Handle @dx.op.createHandleFromHeap(i32 218, i32 %6, i1 false, i1 false)  ; CreateHandleFromHeap(index,samplerHeap,nonUniformIndex)
  %8 = call %dx.types.Handle @dx.op.annotateHandle(i32 216, %dx.types.Handle %7, %dx.types.ResourceProperties { i32 2, i32 1033 })  ; AnnotateHandle(res,props)  resource: Texture2D<4xF32>
  %9 = extractvalue %dx.types.CBufRet.i32 %5, 1
  %10 = call %dx.types.Handle @dx.op.createHandleFromHeap(i32 218, i32 %9, i1 false, i1 false)  ; CreateHandleFromHeap(index,samplerHeap,nonUniformIndex)
  %11 = call %dx.types.Handle @dx.op.annotateHandle(i32 216, %dx.types.Handle %10, %dx.types.ResourceProperties { i32 4098, i32 1033 })  ; AnnotateHandle(res,props)  resource: RWTexture2D<4xF32>
  %12 = extractvalue %dx.types.CBufRet.i32 %5, 2
  %13 = call %dx.types.Handle @dx.op.createHandleFromHeap(i32 218, i32 %12, i1 true, i1 false)  ; CreateHandleFromHeap(index,samplerHeap,nonUniformIndex)
  %14 = call %dx.types.Handle @dx.op.annotateHandle(i32 216, %dx.types.Handle %13, %dx.types.ResourceProperties { i32 14, i32 0 })  ; AnnotateHandle(res,props)  resource: SamplerState
  %15 = uitofp i32 %3 to float
  %16 = uitofp i32 %4 to float
  %17 = fadd fast float %15, 5.000000e-01
  %18 = fadd fast float %16, 5.000000e-01
  %19 = call %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32 59, %dx.types.Handle %2, i32 1)  ; CBufferLoadLegacy(handle,regIndex)
  %20 = extractvalue %dx.types.CBufRet.f32 %19, 0
  %21 = extractvalue %dx.types.CBufRet.f32 %19, 1
  %22 = fmul fast float %20, %17
  %23 = fmul fast float %21, %18
  %24 = call %dx.types.ResRet.f32 @dx.op.sampleLevel.f32(i32 62, %dx.types.Handle %8, %dx.types.Handle %14, float %22, float %23, float undef, float undef, i32 0, i32 0, i32 undef, float 0.000000e+00)  ; SampleLevel(srv,sampler,coord0,coord1,coord2,coord3,offset0,offset1,offset2,LOD)
  %25 = extractvalue %dx.types.ResRet.f32 %24, 0
  %26 = extractvalue %dx.types.ResRet.f32 %24, 1
  %27 = extractvalue %dx.types.ResRet.f32 %24, 2
  %28 = extractvalue %dx.types.ResRet.f32 %24, 3
  call void @dx.op.textureStore.f32(i32 67, %dx.types.Handle %11, i32 %3, i32 %4, i32 undef, float %25, float %26, float %27, float %28, i8 15)  ; TextureStore(srv,coord0,coord1,coord2,value0,value1,value2,value3,mask)
  ret void
}

; Function Attrs: nounwind readnone
declare i32 @dx.op.threadId.i32(i32, i32) #0

; Function Attrs: nounwind readnone
declare %dx.types.Handle @dx.op.createHandleFromHeap(i32, i32, i1, i1) #0

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.f32 @dx.op.sampleLevel.f32(i32, %dx.types.Handle, %dx.types.Handle, float, float, float, float, i32, i32, i32, float) #1

; Function Attrs: nounwind
declare void @dx.op.textureStore.f32(i32, %dx.types.Handle, i32, i32, i32, float, float, float, float, i8) #2

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.f32 @dx.op.cbufferLoadLegacy.f32(i32, %dx.types.Handle, i32) #1

; Function Attrs: nounwind readonly
declare %dx.types.CBufRet.i32 @dx.op.cbufferLoadLegacy.i32(i32, %dx.types.Handle, i32) #1

; Function Attrs: nounwind readnone
declare %dx.types.Handle @dx.op.annotateHandle(i32, %dx.types.Handle, %dx.types.ResourceProperties) #0

; Function Attrs: nounwind readnone
declare %dx.types.Handle @dx.op.createHandleFromBinding(i32, %dx.types.ResBind, i32, i1) #0

attributes #0 = { nounwind readnone }
attributes #1 = { nounwind readonly }
attributes #2 = { nounwind }

!llvm.ident = !{!0}
!dx.version = !{!1}
!dx.valver = !{!2}
!dx.shaderModel = !{!3}
!dx.resources = !{!4}
!dx.entryPoints = !{!7}

!0 = !{!"dxc(private) 1.8.0.4739 (d9a5e97d0)"}
!1 = !{i32 1, i32 6}
!2 = !{i32 1, i32 8}
!3 = !{!"cs", i32 6, i32 6}
!4 = !{null, null, !5, null}
!5 = !{!6}
!6 = !{i32 0, %PushConstants* undef, !"", i32 0, i32 0, i32 1, i32 24, null}
!7 = !{void ()* @CSMain, !"CSMain", null, !4, !8}
!8 = !{i32 0, i64 3221225472, i32 4, !9}
!9 = !{i32 8, i32 8, i32 1}

#endif

const unsigned char g_CS_GenerateMipmaps[] = {
  0x44, 0x58, 0x42, 0x43, 0x93, 0xff, 0xef, 0x75, 0x86, 0x06, 0x3e, 0xe0,
  0x11, 0x3f, 0x89, 0x56, 0x14, 0xf7, 0x22, 0x91, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
  0x48, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00,
  0xd8, 0x00, 0x00, 0x00, 0xf4, 0x00, 0x00, 0x00, 0x53, 0x46, 0x49, 0x30,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
  0x49, 0x53, 0x47, 0x31, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x4f, 0x53, 0x47, 0x31, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x50, 0x53, 0x56, 0x30,
  0x68, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x43, 0x53, 0x4d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x48, 0x41, 0x53, 0x48, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x6f, 0x14, 0x89, 0xda, 0x60, 0x5e, 0x3a, 0x59, 0x27, 0xc9, 0xda, 0x4e,
  0xfc, 0x96, 0xb1, 0x62, 0x44, 0x58, 0x49, 0x4c, 0x04, 0x07, 0x00, 0x00,
  0x66, 0x00, 0x05, 0x00, 0xc1, 0x01, 0x00, 0x00, 0x44, 0x58, 0x49, 0x4c,
  0x06, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xec, 0x06, 0x00, 0x00,
  0x42, 0x43, 0xc0, 0xde, 0x21, 0x0c, 0x00, 0x00, 0xb8, 0x01, 0x00, 0x00,
  0x0b, 0x82, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
  0x07, 0x81, 0x23, 0x91, 0x41, 0xc8, 0x04, 0x49, 0x06, 0x10, 0x32, 0x39,
  0x92, 0x01, 0x84, 0x0c, 0x25, 0x05, 0x08, 0x19, 0x1e, 0x04, 0x8b, 0x62,
  0x80, 0x18, 0x45, 0x02, 0x42, 0x92, 0x0b, 0x42, 0xc4, 0x10, 0x32, 0x14,
  0x38, 0x08, 0x18, 0x4b, 0x0a, 0x32, 0x62, 0x88, 0x48, 0x90, 0x14, 0x20,
  0x43, 0x46, 0x88, 0xa5, 0x00, 0x19, 0x32, 0x42, 0xe4, 0x48, 0x0e, 0x90,
  0x11, 0x23, 0xc4, 0x50, 0x41, 0x51, 0x81, 0x8c, 0xe1, 0x83, 0xe5, 0x8a,
  0x04, 0x31, 0x46, 0x06, 0x51, 0x18, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x1b, 0x8c, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x40, 0x02, 0xa8, 0x0d,
  0x86, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x20, 0x01, 0xd5, 0x06, 0x62,
  0xf8, 0xff, 0xff, 0xff, 0xff, 0x01, 0x90, 0x00, 0x49, 0x18, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x13, 0x82, 0x60, 0x42, 0x20, 0x4c, 0x08, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x89, 0x20, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,
  0x32, 0x22, 0x88, 0x09, 0x20, 0x64, 0x85, 0x04, 0x13, 0x23, 0xa4, 0x84,
  0x04, 0x13, 0x23, 0xe3, 0x84, 0xa1, 0x90, 0x14, 0x12, 0x4c, 0x8c, 0x8c,
  0x0b, 0x84, 0xc4, 0x4c, 0x10, 0x8c, 0xc1, 0x08, 0x40, 0x09, 0x00, 0x0a,
  0xe6, 0x08, 0xc0, 0xa0, 0x0c, 0xc3, 0x30, 0x10, 0x31, 0x47, 0x80, 0x90,
  0x71, 0xcf, 0x70, 0xf9, 0x13, 0xf6, 0x10, 0x92, 0x1f, 0x02, 0xcd, 0xb0,
  0x10, 0x28, 0x38, 0xe6, 0x08, 0x82, 0x52, 0x20, 0xc3, 0x90, 0x24, 0xa4,
  0xcc, 0x00, 0xdc, 0x34, 0x5c, 0xfe, 0x84, 0x3d, 0x84, 0xe4, 0xaf, 0x84,
  0xb4, 0x12, 0x93, 0x5f, 0xdc, 0x36, 0x2a, 0x18, 0x86, 0x61, 0x86, 0xc2,
  0x34, 0x03, 0x82, 0x30, 0x0c, 0xc3, 0x0c, 0xc3, 0xc0, 0x90, 0x53, 0x16,
  0x60, 0x40, 0x86, 0x61, 0x60, 0x18, 0x86, 0x31, 0x08, 0x3a, 0x6a, 0xb8,
  0xfc, 0x09, 0x7b, 0x08, 0xc9, 0xe7, 0x36, 0xaa, 0x58, 0x89, 0xc9, 0x2f,
  0x6e, 0x1b, 0x11, 0x0c, 0xc3, 0x30, 0x85, 0x90, 0x06, 0x64, 0xa0, 0xe9,
  0xa8, 0xe1, 0xf2, 0x27, 0xec, 0x21, 0x24, 0x9f, 0xdb, 0xa8, 0x62, 0x25,
  0x26, 0x1f, 0xb9, 0x6d, 0x44, 0x0c, 0xc3, 0x30, 0x14, 0xa2, 0x1a, 0x90,
  0x81, 0xac, 0xdb, 0x86, 0xcb, 0x9f, 0xb0, 0x87, 0x90, 0xfc, 0x95, 0x90,
  0x1c, 0x2a, 0x12, 0x88, 0x34, 0x72, 0x1e, 0x22, 0x9a, 0x10, 0x42, 0x42,
  0xc2, 0x30, 0x14, 0x02, 0x19, 0x10, 0x8c, 0xb2, 0x83, 0x86, 0xcb, 0x9f,
  0xb0, 0x87, 0x90, 0xfc, 0x95, 0x90, 0x36, 0xa4, 0x19, 0x10, 0x31, 0x0c,
  0x83, 0x51, 0x0a, 0x64, 0xd8, 0x86, 0x84, 0xb8, 0x81, 0x80, 0x61, 0x04,
  0x81, 0xb9, 0x26, 0xa5, 0xa4, 0x83, 0x73, 0x1a, 0x69, 0x02, 0x9a, 0x49,
  0x42, 0xc5, 0x30, 0x0c, 0xc3, 0x47, 0xe0, 0x1c, 0x01, 0x28, 0x00, 0x00,
  0x13, 0x14, 0x72, 0xc0, 0x87, 0x74, 0x60, 0x87, 0x36, 0x68, 0x87, 0x79,
  0x68, 0x03, 0x72, 0xc0, 0x87, 0x0d, 0xaf, 0x50, 0x0e, 0x6d, 0xd0, 0x0e,
  0x7a, 0x50, 0x0e, 0x6d, 0x00, 0x0f, 0x7a, 0x30, 0x07, 0x72, 0xa0, 0x07,
  0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x71, 0xa0, 0x07, 0x73, 0x20, 0x07,
  0x6d, 0x90, 0x0e, 0x78, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e,
  0x71, 0x60, 0x07, 0x7a, 0x30, 0x07, 0x72, 0xd0, 0x06, 0xe9, 0x30, 0x07,
  0x72, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x76, 0x40, 0x07,
  0x7a, 0x60, 0x07, 0x74, 0xd0, 0x06, 0xe6, 0x10, 0x07, 0x76, 0xa0, 0x07,
  0x73, 0x20, 0x07, 0x6d, 0x60, 0x0e, 0x73, 0x20, 0x07, 0x7a, 0x30, 0x07,
  0x72, 0xd0, 0x06, 0xe6, 0x60, 0x07, 0x74, 0xa0, 0x07, 0x76, 0x40, 0x07,
  0x6d, 0xe0, 0x0e, 0x78, 0xa0, 0x07, 0x71, 0x60, 0x07, 0x7a, 0x30, 0x07,
  0x72, 0xa0, 0x07, 0x76, 0x40, 0x07, 0x43, 0x9e, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x3c, 0x04, 0x10, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x79, 0x14, 0x20,
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xf2, 0x38,
  0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xe4,
  0x81, 0x80, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
  0xc8, 0x33, 0x01, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0x90, 0xc7, 0x02, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x21, 0x4f, 0x06, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x43, 0x1e, 0x0e, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xb2, 0x40, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
  0x32, 0x1e, 0x98, 0x14, 0x19, 0x11, 0x4c, 0x90, 0x8c, 0x09, 0x26, 0x47,
  0xc6, 0x04, 0x43, 0x1a, 0x4a, 0xa0, 0x18, 0x0a, 0x62, 0x04, 0xa0, 0x60,
  0x0a, 0x81, 0xc4, 0x02, 0x04, 0x04, 0x04, 0xc4, 0xa0, 0x70, 0x06, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00,
  0x1a, 0x03, 0x4c, 0x90, 0x46, 0x02, 0x13, 0xc4, 0x31, 0x20, 0xc3, 0x1b,
  0x43, 0x81, 0x93, 0x4b, 0xb3, 0x0b, 0xa3, 0x2b, 0x4b, 0x01, 0x89, 0x71,
  0xc1, 0x71, 0x81, 0x71, 0xa1, 0xb9, 0x99, 0xc9, 0x01, 0x41, 0x21, 0xcb,
  0x09, 0xab, 0x29, 0xcb, 0xb9, 0x21, 0x83, 0x49, 0xd9, 0x10, 0x04, 0x13,
  0x84, 0x21, 0x99, 0x20, 0x0c, 0xca, 0x06, 0x61, 0x20, 0x26, 0x08, 0xc3,
  0xb2, 0x41, 0x18, 0x0c, 0x0a, 0x63, 0x73, 0x1b, 0x06, 0x84, 0x20, 0x26,
  0x08, 0x03, 0x33, 0x41, 0x08, 0x03, 0x88, 0xc0, 0x04, 0x61, 0x68, 0x36,
  0x20, 0xca, 0xc2, 0x28, 0xca, 0xd0, 0x00, 0x1b, 0x02, 0x67, 0x03, 0x01,
  0x00, 0x0f, 0x30, 0x41, 0x10, 0x00, 0x1a, 0x43, 0x53, 0x4d, 0x61, 0x69,
  0x6e, 0x13, 0x04, 0x31, 0x78, 0x26, 0x08, 0x83, 0xb3, 0x61, 0x30, 0x8c,
  0x61, 0x03, 0xa1, 0x4c, 0x54, 0xb5, 0xa1, 0x88, 0x24, 0x00, 0xb2, 0xaa,
  0xb0, 0xb1, 0xd9, 0xb5, 0xb9, 0xa4, 0x91, 0x95, 0xb9, 0xd1, 0x4d, 0x09,
  0x82, 0x2a, 0x64, 0x78, 0x2e, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e,
  0x53, 0x02, 0xa2, 0x09, 0x19, 0x9e, 0x8b, 0x5d, 0x18, 0x9b, 0x5d, 0x99,
  0xdc, 0x94, 0xc0, 0xa8, 0x43, 0x86, 0xe7, 0x32, 0x87, 0x16, 0x46, 0x56,
  0x26, 0xd7, 0xf4, 0x46, 0x56, 0xc6, 0x36, 0x25, 0x40, 0xca, 0x90, 0xe1,
  0xb9, 0xc8, 0x95, 0xcd, 0xbd, 0xd5, 0xc9, 0x8d, 0x95, 0xcd, 0x4d, 0x09,
  0x9e, 0x3a, 0x64, 0x78, 0x2e, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f,
  0x69, 0x6e, 0x74, 0x73, 0x53, 0x02, 0x0b, 0x00, 0x79, 0x18, 0x00, 0x00,
  0x4c, 0x00, 0x00, 0x00, 0x33, 0x08, 0x80, 0x1c, 0xc4, 0xe1, 0x1c, 0x66,
  0x14, 0x01, 0x3d, 0x88, 0x43, 0x38, 0x84, 0xc3, 0x8c, 0x42, 0x80, 0x07,
  0x79, 0x78, 0x07, 0x73, 0x98, 0x71, 0x0c, 0xe6, 0x00, 0x0f, 0xed, 0x10,
  0x0e, 0xf4, 0x80, 0x0e, 0x33, 0x0c, 0x42, 0x1e, 0xc2, 0xc1, 0x1d, 0xce,
  0xa1, 0x1c, 0x66, 0x30, 0x05, 0x3d, 0x88, 0x43, 0x38, 0x84, 0x83, 0x1b,
  0xcc, 0x03, 0x3d, 0xc8, 0x43, 0x3d, 0x8c, 0x03, 0x3d, 0xcc, 0x78, 0x8c,
  0x74, 0x70, 0x07, 0x7b, 0x08, 0x07, 0x79, 0x48, 0x87, 0x70, 0x70, 0x07,
  0x7a, 0x70, 0x03, 0x76, 0x78, 0x87, 0x70, 0x20, 0x87, 0x19, 0xcc, 0x11,
  0x0e, 0xec, 0x90, 0x0e, 0xe1, 0x30, 0x0f, 0x6e, 0x30, 0x0f, 0xe3, 0xf0,
  0x0e, 0xf0, 0x50, 0x0e, 0x33, 0x10, 0xc4, 0x1d, 0xde, 0x21, 0x1c, 0xd8,
  0x21, 0x1d, 0xc2, 0x61, 0x1e, 0x66, 0x30, 0x89, 0x3b, 0xbc, 0x83, 0x3b,
  0xd0, 0x43, 0x39, 0xb4, 0x03, 0x3c, 0xbc, 0x83, 0x3c, 0x84, 0x03, 0x3b,
  0xcc, 0xf0, 0x14, 0x76, 0x60, 0x07, 0x7b, 0x68, 0x07, 0x37, 0x68, 0x87,
  0x72, 0x68, 0x07, 0x37, 0x80, 0x87, 0x70, 0x90, 0x87, 0x70, 0x60, 0x07,
  0x76, 0x28, 0x07, 0x76, 0xf8, 0x05, 0x76, 0x78, 0x87, 0x77, 0x80, 0x87,
  0x5f, 0x08, 0x87, 0x71, 0x18, 0x87, 0x72, 0x98, 0x87, 0x79, 0x98, 0x81,
  0x2c, 0xee, 0xf0, 0x0e, 0xee, 0xe0, 0x0e, 0xf5, 0xc0, 0x0e, 0xec, 0x30,
  0x03, 0x62, 0xc8, 0xa1, 0x1c, 0xe4, 0xa1, 0x1c, 0xcc, 0xa1, 0x1c, 0xe4,
  0xa1, 0x1c, 0xdc, 0x61, 0x1c, 0xca, 0x21, 0x1c, 0xc4, 0x81, 0x1d, 0xca,
  0x61, 0x06, 0xd6, 0x90, 0x43, 0x39, 0xc8, 0x43, 0x39, 0x98, 0x43, 0x39,
  0xc8, 0x43, 0x39, 0xb8, 0xc3, 0x38, 0x94, 0x43, 0x38, 0x88, 0x03, 0x3b,
  0x94, 0xc3, 0x2f, 0xbc, 0x83, 0x3c, 0xfc, 0x82, 0x3b, 0xd4, 0x03, 0x3b,
  0xb0, 0xc3, 0x0c, 0xc4, 0x21, 0x07, 0x7c, 0x70, 0x03, 0x7a, 0x28, 0x87,
  0x76, 0x80, 0x87, 0x19, 0xd1, 0x43, 0x0e, 0xf8, 0xe0, 0x06, 0xe4, 0x20,
  0x0e, 0xe7, 0xe0, 0x06, 0xf6, 0x10, 0x0e, 0xf2, 0xc0, 0x0e, 0xe1, 0x90,
  0x0f, 0xef, 0x50, 0x0f, 0xf4, 0x00, 0x00, 0x00, 0x71, 0x20, 0x00, 0x00,
  0x2a, 0x00, 0x00, 0x00, 0x06, 0x60, 0x70, 0xac, 0x09, 0x20, 0x8d, 0x1d,
  0x50, 0xc3, 0xe5, 0x3b, 0x8f, 0x0f, 0x34, 0x8d, 0x33, 0x01, 0x13, 0x11,
  0x02, 0xcd, 0xb0, 0x10, 0x56, 0xb0, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10,
  0x50, 0x45, 0x41, 0x44, 0xa5, 0x03, 0x0c, 0x25, 0x61, 0x00, 0x02, 0xe6,
  0x17, 0xb7, 0x6d, 0x06, 0xdb, 0x70, 0xf9, 0xce, 0xe3, 0x0b, 0x01, 0x55,
  0x14, 0x44, 0x54, 0x3a, 0xc0, 0x50, 0x12, 0x06, 0x20, 0x60, 0x3e, 0x72,
  0xdb, 0x86, 0xd0, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10, 0x11, 0xc0, 0x44,
  0x84, 0x40, 0x33, 0x2c, 0xc4, 0x17, 0x39, 0xcc, 0x86, 0x34, 0x03, 0xd2,
  0x18, 0x26, 0xa0, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10, 0x11, 0xc0, 0x44,
  0x84, 0x40, 0x33, 0x2c, 0xc4, 0x17, 0x39, 0x4c, 0x48, 0x00, 0x8f, 0x0d,
  0x54, 0xc3, 0xe5, 0x3b, 0x8f, 0x2f, 0x01, 0xcc, 0xb3, 0x10, 0x25, 0x51,
  0x11, 0x8b, 0x5f, 0xdc, 0xb6, 0x11, 0x58, 0xc3, 0xe5, 0x3b, 0x8f, 0x3f,
  0x11, 0xd7, 0x44, 0x45, 0x04, 0x3b, 0x39, 0x11, 0xe1, 0x17, 0xb7, 0x6d,
  0x01, 0xd2, 0x70, 0xf9, 0xce, 0xe3, 0x4f, 0x47, 0x44, 0x00, 0x83, 0x38,
  0xf8, 0xc8, 0x6d, 0x03, 0x61, 0x20, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00,
  0x13, 0x04, 0x41, 0x2c, 0x10, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00,
  0x34, 0x14, 0xec, 0x40, 0xd1, 0x0e, 0x94, 0x6e, 0x40, 0xd9, 0x95, 0x24,
  0xc4, 0x0c, 0x40, 0xc9, 0x0e, 0x94, 0x46, 0x11, 0x14, 0x21, 0x50, 0x71,
  0x14, 0x5f, 0x19, 0x06, 0x90, 0x51, 0x04, 0xe5, 0x41, 0xc9, 0x08, 0x40,
  0x0d, 0x10, 0x33, 0x46, 0x00, 0x82, 0x20, 0x08, 0x7f, 0x33, 0x00, 0x23,
  0x00, 0x84, 0xcd, 0x21, 0x60, 0xcd, 0x1c, 0x42, 0x56, 0xcd, 0x21, 0x68,
  0xd5, 0x1c, 0xc2, 0xc6, 0xd0, 0x36, 0x07, 0xc1, 0x30, 0x8c, 0x07, 0x00,
  0x23, 0x06, 0x08, 0x00, 0x82, 0x60, 0xc0, 0x89, 0xc1, 0x14, 0x78, 0xca,
  0x88, 0xc1, 0x01, 0x80, 0x20, 0x18, 0x64, 0x64, 0xa0, 0x05, 0xc6, 0x88,
  0x81, 0x01, 0x80, 0x20, 0x18, 0x10, 0x6b, 0x90, 0x81, 0xc1, 0x88, 0x81,
  0x01, 0x80, 0x20, 0x18, 0x10, 0x6c, 0xa0, 0x91, 0xc1, 0x88, 0xc1, 0x01,
  0x80, 0x20, 0x18, 0x58, 0x68, 0xa0, 0x0d, 0x62, 0x30, 0x9a, 0x10, 0x00,
  0x23, 0x06, 0x08, 0x00, 0x82, 0x60, 0xa0, 0xb8, 0x81, 0x17, 0x40, 0xd0,
  0x88, 0xc1, 0x01, 0x80, 0x20, 0x18, 0x64, 0x6a, 0x00, 0x06, 0xc1, 0x32,
  0x9a, 0x40, 0x04, 0x23, 0x06, 0x08, 0x00, 0x82, 0x60, 0xa0, 0xc4, 0x41,
  0x18, 0x04, 0xd3, 0x34, 0x62, 0x70, 0x00, 0x20, 0x08, 0x06, 0x59, 0x1b,
  0x8c, 0x41, 0xd0, 0x8c, 0x26, 0x1c, 0xc2, 0x88, 0x01, 0x02, 0x80, 0x20,
  0x18, 0x28, 0x74, 0x40, 0x06, 0x41, 0x65, 0x8d, 0x18, 0x1c, 0x00, 0x08,
  0x82, 0x41, 0x06, 0x07, 0x66, 0x10, 0x3c, 0xc7, 0x30, 0x75, 0x0c, 0x53,
  0x26, 0x60, 0xf0, 0x31, 0x21, 0x83, 0xcf, 0x88, 0xc1, 0x01, 0x80, 0x20,
  0x18, 0x4c, 0x77, 0x80, 0x06, 0xd1, 0x1c, 0x8c, 0x26, 0x04, 0xc0, 0x68,
  0x82, 0x10, 0x98, 0x50, 0xc8, 0xc7, 0x84, 0x42, 0x3e, 0x23, 0x06, 0x0f,
  0x00, 0x82, 0x60, 0xe0, 0xf8, 0x41, 0x19, 0x40, 0x8a, 0x10, 0x78, 0x5e,
  0x1d, 0xd4, 0xc1, 0x1a, 0x74, 0xa3, 0x09, 0x01, 0x30, 0x9a, 0x20, 0x04,
  0xa3, 0x09, 0x83, 0x30, 0x9a, 0x40, 0x0c, 0x23, 0x06, 0x0e, 0x00, 0x82,
  0x60, 0x00, 0x89, 0x42, 0x1a, 0x48, 0x5a, 0x06, 0x07, 0xc4, 0x20, 0x04,
  0x67, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#endif
