#include "iglo.h"
#include "iglo_batch_renderer.h"
#include "ShaderCommon.h"
#include "ColorGradient.h"
#include "ParticleRenderer.h"
#include <array>

void ParticleRenderer::Unload()
{
	isLoaded = false;
	context = nullptr;
	colorGradient = nullptr;
	cmd.Unload();

	transitionHeatmapLayout = false;
	graphics = GraphicsParams();
	computeHeatmap.Unload();
	renderHeatmapGrayscale.Unload();
	renderHeatmapColor.Unload();

	for (uint32_t a = 0; a < 2; a++)
	{
		for (uint32_t b = 0; b < 2; b++)
		{
			for (uint32_t c = 0; c < 2; c++)
			{
				for (uint32_t d = 0; d < 2; d++)
				{
					renderPrims[a][b][c][d].Unload();
				}
			}
		}
	}

	heatmap = nullptr;
	renderTarget = nullptr;
}

bool ParticleRenderer::Load(ig::IGLOContext& context)
{
	Unload();

	cmd.Load(context, ig::CommandListType::Graphics);

	renderTargetDesc = context.GetBackBufferRenderTargetDesc();
	renderTargetDesc.msaa = context.GetMaxMultiSampleCount(renderTargetDesc.colorFormats.at(0));

	ig::ReadFileResult f_VS_Rects_ColorBuffer = ig::ReadFile(ShaderFolder + "VS_Rects_ColorBuffer.cso");
	ig::ReadFileResult f_VS_Rects_UniColor = ig::ReadFile(ShaderFolder + "VS_Rects_UniColor.cso");
	ig::ReadFileResult f_VS_Points_ColorBuffer = ig::ReadFile(ShaderFolder + "VS_Points_ColorBuffer.cso");
	ig::ReadFileResult f_VS_Points_UniColor = ig::ReadFile(ShaderFolder + "VS_Points_UniColor.cso");
	ig::ReadFileResult f_PS_Basic = ig::ReadFile(ShaderFolder + "PS_Basic.cso");
	ig::ReadFileResult f_VS_Heatmap = ig::ReadFile(ShaderFolder + "VS_Heatmap.cso");
	ig::ReadFileResult f_PS_Heatmap_Grayscale = ig::ReadFile(ShaderFolder + "PS_Heatmap_Grayscale.cso");
	ig::ReadFileResult f_PS_Heatmap_Color = ig::ReadFile(ShaderFolder + "PS_Heatmap_Color.cso");
	ig::ReadFileResult f_CS_Heatmap = ig::ReadFile(ShaderFolder + "CS_Heatmap.cso");

	if (!f_VS_Rects_ColorBuffer.success ||
		!f_VS_Rects_UniColor.success ||
		!f_VS_Points_ColorBuffer.success ||
		!f_VS_Points_UniColor.success ||
		!f_PS_Basic.success ||
		!f_VS_Heatmap.success ||
		!f_PS_Heatmap_Grayscale.success ||
		!f_PS_Heatmap_Color.success ||
		!f_CS_Heatmap.success)
	{
		ig::PopupMessage("Failed to load one or more shaders from '" + ShaderFolder + "'", "Error", &context);
		return false;
	}

	computeHeatmap.LoadAsCompute(context, ig::Shader(f_CS_Heatmap.fileContent));

	ig::PipelineDesc heatmapDesc;
	heatmapDesc.VS = ig::Shader(f_VS_Heatmap.fileContent);
	heatmapDesc.PS = ig::Shader(f_PS_Heatmap_Grayscale.fileContent);
	heatmapDesc.blendStates = { ig::BlendDesc::BlendDisabled };
	heatmapDesc.depthState = ig::DepthDesc::DepthDisabled;
	heatmapDesc.rasterizerState = ig::RasterizerDesc::NoCull;
	heatmapDesc.renderTargetDesc = context.GetBackBufferRenderTargetDesc();
	heatmapDesc.primitiveTopology = ig::PrimitiveTopology::TriangleStrip;
	heatmapDesc.vertexLayout = {};
	renderHeatmapGrayscale.Load(context, heatmapDesc);

	heatmapDesc.PS = ig::Shader(f_PS_Heatmap_Color.fileContent);
	renderHeatmapColor.Load(context, heatmapDesc);

	ig::BlendDesc additiveBlend;
	additiveBlend.enabled = true;
	additiveBlend.srcBlend = ig::BlendData::One;
	additiveBlend.destBlend = ig::BlendData::One;
	additiveBlend.blendOp = ig::BlendOperation::Add;
	additiveBlend.srcBlendAlpha = ig::BlendData::One;
	additiveBlend.destBlendAlpha = ig::BlendData::Zero;
	additiveBlend.blendOpAlpha = ig::BlendOperation::Add;
	additiveBlend.colorWriteMask = (byte)ig::ColorWriteMask::All;

	for (uint32_t a = 0; a < 2; a++) // Primitive
	{
		for (uint32_t b = 0; b < 2; b++) // Blend state
		{
			for (uint32_t c = 0; c < 2; c++) // MSAA
			{
				for (uint32_t d = 0; d < 2; d++) // Color
				{
					ig::BlendDesc blend = (b == (uint32_t)PermBlendStates::BlendDisabled) ?
						ig::BlendDesc::BlendDisabled : additiveBlend;

					ig::RenderTargetDesc rtd = (c == (uint32_t)PermMSAA::MSAA_Enabled) ?
						renderTargetDesc : context.GetBackBufferRenderTargetDesc();

					ig::BatchParams params = (a == (uint32_t)PermPrimitives::Points) ?
						ig::GetStandardBatchParams(ig::StandardBatchType::Points_XYC, rtd) :
						ig::GetStandardBatchParams(ig::StandardBatchType::Rects, rtd);

					params.pipelineDesc.blendStates = { blend };
					params.pipelineDesc.vertexLayout = {};
					params.pipelineDesc.PS = ig::Shader(f_PS_Basic.fileContent);
					if (a == (uint32_t)PermPrimitives::Points)
					{
						params.pipelineDesc.VS = (d == (uint32_t)PermColor::Uniform) ?
							ig::Shader(f_VS_Points_UniColor.fileContent) :
							ig::Shader(f_VS_Points_ColorBuffer.fileContent);
					}
					else
					{
						params.pipelineDesc.VS = (d == (uint32_t)PermColor::Uniform) ?
							ig::Shader(f_VS_Rects_UniColor.fileContent) :
							ig::Shader(f_VS_Rects_ColorBuffer.fileContent);
						params.pipelineDesc.primitiveTopology = ig::PrimitiveTopology::TriangleList;
					}
					renderPrims[a][b][c][d].Load(context, params.pipelineDesc);
				}
			}
		}
	}

	this->context = &context;
	this->isLoaded = true;
	ResizeTextures(context.GetBackBufferExtent());
	return true;
}

void ParticleRenderer::SetColorGradient(const ColorGradient* colorGradient)
{
	this->colorGradient = colorGradient;
}

void ParticleRenderer::ResizeTextures(ig::Extent2D textureSize)
{
	if (!isLoaded) throw std::exception();
	if (!context) throw std::exception();

	this->textureSize = textureSize;

	// MSAA render target
	if (renderTarget) context->DelayedTextureUnload(renderTarget);
	renderTarget = std::make_shared<ig::Texture>();
	renderTarget->Load(*context, textureSize.width, textureSize.height, renderTargetDesc.colorFormats.at(0),
		ig::TextureUsage::RenderTexture, renderTargetDesc.msaa);
	ig::Print(ig::ToString("Render target texture size ", renderTarget->GetWidth(), "x", renderTarget->GetHeight(), "\n"));

	// Heatmap texture
	ig::Extent2D heatmapSize = GetHeatmapSize(textureSize, graphics.heatmapPixelSize);
	if (heatmap) context->DelayedTextureUnload(heatmap);
	heatmap = std::make_shared<ig::Texture>();
	heatmap->Load(*context, heatmapSize.width, heatmapSize.height, ig::Format::UINT32_NotNormalized, ig::TextureUsage::UnorderedAccess);
	ig::Print(ig::ToString("Heatmap texture size ", heatmap->GetWidth(), "x", heatmap->GetHeight(), "\n"));

	transitionHeatmapLayout = true;
}

void ParticleRenderer::SetGraphicsParams(const GraphicsParams& params)
{
	if (!isLoaded) throw std::exception();
	if (!context) throw std::exception();
	if (params.heatmapPixelSize == 0) throw std::invalid_argument("Heatmap pixel size can't be 0.");

	this->graphics = params;

	ig::Extent2D oldHeatmapSize = ig::Extent2D(heatmap->GetWidth(), heatmap->GetHeight());
	ig::Extent2D newHeatmapSize = GetHeatmapSize(context->GetBackBufferExtent(), params.heatmapPixelSize);
	if (oldHeatmapSize != newHeatmapSize)
	{
		ResizeTextures(context->GetBackBufferExtent());
	}
}

void ParticleRenderer::ClearHeatmap()
{
	if (!isLoaded) throw std::exception();
	if (!heatmap) throw std::exception();
	if (!heatmap->IsLoaded()) throw std::exception();

	context->WaitForIdleDevice();

	cmd.Begin();
	{
		cmd.AddTextureBarrier(*heatmap, ig::SimpleBarrier::Discard, ig::SimpleBarrier::ClearUnorderedAccess);
		cmd.FlushBarriers();

		std::array<uint32_t, 4> values = { 0, 0, 0, 0 };
		cmd.ClearUnorderedAccessTextureUInt32(*heatmap, values.data());

		cmd.AddTextureBarrier(*heatmap, ig::SimpleBarrier::ClearUnorderedAccess, ig::SimpleBarrier::ComputeShaderUnorderedAccess);
		cmd.FlushBarriers();
	}
	cmd.End();

	context->Submit(cmd);
	context->WaitForIdleDevice();

	transitionHeatmapLayout = false;
}

void ParticleRenderer::Render(const ig::Buffer& bufferMP, const ig::Buffer& bufferVP, uint32_t numMP, uint32_t numVP)
{
	if (graphics.drawMode == DrawMode::Points ||
		graphics.drawMode == DrawMode::Rects)
	{
		RenderPrimitives(bufferMP, bufferVP, numMP, numVP);
	}
	else
	{
		RenderHeatmap(bufferMP, bufferVP, numMP, numVP);
	}
}

void ParticleRenderer::RenderHeatmap(const ig::Buffer& bufferMP, const ig::Buffer& bufferVP, uint32_t numMP, uint32_t numVP)
{
	if (!context) throw std::exception();
	if (graphics.heatmapPixelSize == 0) throw std::runtime_error("heatmap pixel size should never be 0.");

	ig::Extent2D heatmapSize = GetHeatmapSize(context->GetBackBufferExtent(), graphics.heatmapPixelSize);
	uint32_t heatmapWidth = heatmapSize.width;
	uint32_t heatmapHeight = heatmapSize.height;

	if (heatmapWidth != heatmap->GetWidth() ||
		heatmapHeight != heatmap->GetHeight())
	{
		throw std::runtime_error("Heatmap texture has an unexpected size.");
	}

	cmd.Begin();
	{
		cmd.AddTextureBarrier(context->GetBackBuffer(), ig::SimpleBarrier::Discard, ig::SimpleBarrier::RenderTarget);
		cmd.FlushBarriers();

		cmd.SetRenderTarget(&context->GetBackBuffer());
		cmd.SetViewport((float)context->GetWidth(), (float)context->GetHeight());
		cmd.SetScissorRectangle(context->GetWidth(), context->GetHeight());
		cmd.ClearColor(context->GetBackBuffer(), ig::Colors::Black);

		////////////////////////////
		// Update Heatmap texture //
		////////////////////////////
		{
			cmd.AddBufferBarrier(bufferMP,
				ig::BarrierSync::ComputeShading, ig::BarrierSync::ComputeShading,
				ig::BarrierAccess::UnorderedAccess, ig::BarrierAccess::ShaderResource);
			cmd.AddBufferBarrier(bufferVP,
				ig::BarrierSync::ComputeShading, ig::BarrierSync::ComputeShading,
				ig::BarrierAccess::UnorderedAccess, ig::BarrierAccess::ShaderResource);
			if (transitionHeatmapLayout && !graphics.clearHeatmap)
			{
				transitionHeatmapLayout = false;
				cmd.AddTextureBarrier(*heatmap, ig::SimpleBarrier::Discard, ig::SimpleBarrier::ComputeShaderUnorderedAccess);
			}
			cmd.FlushBarriers();

			if (graphics.clearHeatmap)
			{
				transitionHeatmapLayout = false;
				cmd.AddTextureBarrier(*heatmap, ig::SimpleBarrier::Discard, ig::SimpleBarrier::ClearUnorderedAccess);
				cmd.FlushBarriers();

				std::array<uint32_t, 4> clearValues = { 0, 0, 0, 0 };
				cmd.ClearUnorderedAccessTextureUInt32(*heatmap, clearValues.data());

				cmd.AddTextureBarrier(*heatmap, ig::SimpleBarrier::ClearUnorderedAccess, ig::SimpleBarrier::ComputeShaderUnorderedAccess);
				cmd.FlushBarriers();
			}

			struct PushConstants
			{
				uint32_t particleBufferIndex = IGLO_UINT32_MAX;
				uint32_t destTextureIndex = IGLO_UINT32_MAX;

				ig::Vector2 inverseHeatmapPixelSize; // Possible values: 1/1, 1/2, 1/4, 1/8, etc...
				ig::Extent2D destTextureSize;
				uint32_t numParticles = 0;
				uint32_t dispatchX = 0;
			};

			PushConstants pushConstants;
			pushConstants.destTextureIndex = heatmap->GetUnorderedAccessDescriptor()->heapIndex;
			pushConstants.inverseHeatmapPixelSize = ig::Vector2(
				1.0f / (float)graphics.heatmapPixelSize,
				1.0f / (float)graphics.heatmapPixelSize);
			pushConstants.destTextureSize = heatmapSize;

			uint32_t threadsPerGroup = ComputeBlockSizeX * ComputeBlockSizeY;

			// Compute shader will count how many particles are within the bounds of each pixel
			if (numMP > 0)
			{
				uint32_t totalThreadGroups = (numMP + (threadsPerGroup - 1)) / threadsPerGroup;
				uint32_t gridSize = (uint32_t)ceilf(sqrtf((float)totalThreadGroups));

				cmd.SetPipeline(computeHeatmap);
				pushConstants.particleBufferIndex = bufferMP.GetDescriptor()->heapIndex;
				pushConstants.numParticles = numMP;
				pushConstants.dispatchX = gridSize;
				cmd.SetComputePushConstants(&pushConstants, sizeof(pushConstants));
				cmd.DispatchCompute(gridSize, gridSize, 1);
			}
			if (numVP > 0)
			{
				uint32_t totalThreadGroups = (numVP + (threadsPerGroup - 1)) / threadsPerGroup;
				uint32_t gridSize = (uint32_t)ceilf(sqrtf((float)totalThreadGroups));

				cmd.SetPipeline(computeHeatmap);
				pushConstants.particleBufferIndex = bufferVP.GetDescriptor()->heapIndex;
				pushConstants.numParticles = numVP;
				pushConstants.dispatchX = gridSize;
				cmd.SetComputePushConstants(&pushConstants, sizeof(pushConstants));
				cmd.DispatchCompute(gridSize, gridSize, 1);
			}

			cmd.AddBufferBarrier(bufferMP,
				ig::BarrierSync::ComputeShading, ig::BarrierSync::ComputeShading,
				ig::BarrierAccess::ShaderResource, ig::BarrierAccess::UnorderedAccess);
			cmd.AddBufferBarrier(bufferVP,
				ig::BarrierSync::ComputeShading, ig::BarrierSync::ComputeShading,
				ig::BarrierAccess::ShaderResource, ig::BarrierAccess::UnorderedAccess);
			cmd.AddTextureBarrier(*heatmap, ig::SimpleBarrier::ComputeShaderUnorderedAccess, ig::SimpleBarrier::PixelShaderResource);
			cmd.FlushBarriers();
		}

		//////////////////////////
		// Draw Heatmap texture //
		//////////////////////////
		{
			bool grayscale = (graphics.drawMode == DrawMode::Heatmap_Grayscale);
			cmd.SetPipeline(grayscale ? renderHeatmapGrayscale : renderHeatmapColor);

			struct PushConstants
			{
				uint32_t textureIndex = IGLO_UINT32_MAX;

				ig::Vector2 inverseHeatmapPixelSize; // Possible values: 1/1, 1/2, 1/4, 1/8, etc...
			};
			PushConstants pushConstants;
			pushConstants.textureIndex = heatmap->GetDescriptor()->heapIndex;
			pushConstants.inverseHeatmapPixelSize = ig::Vector2(
				1.0f / (float)graphics.heatmapPixelSize,
				1.0f / (float)graphics.heatmapPixelSize);

			cmd.SetPushConstants(&pushConstants, sizeof(pushConstants));
			cmd.Draw(4);

			cmd.AddTextureBarrier(*heatmap, ig::SimpleBarrier::PixelShaderResource, ig::SimpleBarrier::ComputeShaderUnorderedAccess);
			cmd.FlushBarriers();
		}

	}
	cmd.End();

	context->Submit(cmd);
}

void ParticleRenderer::RenderPrimitives(const ig::Buffer& bufferMP, const ig::Buffer& bufferVP, uint32_t numMP, uint32_t numVP)
{
	if (graphics.permColor == PermColor::Gradient)
	{
		if (!colorGradient) throw std::runtime_error("No color gradient is set.");
	}

	bool useMSAA = (graphics.permMSAA == PermMSAA::MSAA_Enabled);

	if (useMSAA)
	{
		if (renderTarget->GetWidth() != context->GetWidth() ||
			renderTarget->GetHeight() != context->GetHeight())
		{
			throw std::runtime_error("MSAA render texture size doesn't match back buffer size.");
		}
	}

	cmd.Begin();
	{

		// If MSAA is requested, we will render onto an MSAA render target.
		if (useMSAA)
		{
			cmd.AddTextureBarrier(*renderTarget, ig::SimpleBarrier::Discard, ig::SimpleBarrier::RenderTarget);
			cmd.FlushBarriers();

			cmd.SetRenderTarget(renderTarget.get());
			cmd.SetViewport((float)renderTarget->GetWidth(), (float)renderTarget->GetHeight());
			cmd.SetScissorRectangle(renderTarget->GetWidth(), renderTarget->GetHeight());
			cmd.ClearColor(*renderTarget, ig::Colors::Black);
		}
		else
		{
			cmd.AddTextureBarrier(context->GetBackBuffer(), ig::SimpleBarrier::Discard, ig::SimpleBarrier::RenderTarget);
			cmd.FlushBarriers();

			cmd.SetRenderTarget(&context->GetBackBuffer());
			cmd.SetViewport((float)context->GetWidth(), (float)context->GetHeight());
			cmd.SetScissorRectangle(context->GetWidth(), context->GetHeight());
			cmd.ClearColor(context->GetBackBuffer(), ig::Colors::Black);
		}

		/////////////////////
		// Draw primitives //
		/////////////////////
		{
			cmd.AddBufferBarrier(bufferMP,
				ig::BarrierSync::ComputeShading, ig::BarrierSync::VertexShading,
				ig::BarrierAccess::UnorderedAccess, ig::BarrierAccess::ShaderResource);
			cmd.AddBufferBarrier(bufferVP,
				ig::BarrierSync::ComputeShading, ig::BarrierSync::VertexShading,
				ig::BarrierAccess::UnorderedAccess, ig::BarrierAccess::ShaderResource);
			cmd.FlushBarriers();

			PermPrimitives p = (graphics.drawMode == DrawMode::Points) ? PermPrimitives::Points : PermPrimitives::Rects;
			cmd.SetPipeline(renderPrims[(int)p][(int)graphics.permBlend][(int)graphics.permMSAA][(int)graphics.permColor]);

			struct PushConstants
			{
				uint32_t particleBufferIndex = IGLO_UINT32_MAX;
				uint32_t colorGradientBufferIndex = IGLO_UINT32_MAX;

				uint32_t numColorPoints = 0;
				uint32_t numParticles = 0;
				ig::Vector2 screenSize;
				uint32_t uniformColor = 0;
			};

			PushConstants pushConstants;
			pushConstants.particleBufferIndex = bufferMP.GetDescriptor()->heapIndex;
			pushConstants.screenSize = ig::Vector2((float)context->GetWidth(), (float)context->GetHeight());
			pushConstants.numParticles = numMP;
			if (graphics.permColor == PermColor::Gradient)
			{
				pushConstants.colorGradientBufferIndex = colorGradient->GetBuffer().GetDescriptor()->heapIndex;
				pushConstants.numColorPoints = colorGradient->GetNumColorPoints();
			}
			else
			{
				pushConstants.uniformColor = graphics.uniformColor.rgba;
			}

			cmd.SetPushConstants(&pushConstants, sizeof(pushConstants));

			if (graphics.drawMode == DrawMode::Points)
			{
				// Draw mass particles
				cmd.Draw(numMP);

				// Draw visual particless
				pushConstants.particleBufferIndex = bufferVP.GetDescriptor()->heapIndex;
				pushConstants.numParticles = numVP;
				cmd.SetPushConstants(&pushConstants, sizeof(pushConstants));
				cmd.Draw(numVP);
			}
			else if (graphics.drawMode == DrawMode::Rects)
			{
				// Draw mass particles
				const uint32_t verticesPerQuad = 6;
				cmd.Draw(numMP * verticesPerQuad);

				// Draw visual particles
				pushConstants.particleBufferIndex = bufferVP.GetDescriptor()->heapIndex;
				pushConstants.numParticles = numVP;
				cmd.SetPushConstants(&pushConstants, sizeof(pushConstants));
				cmd.Draw(numVP * verticesPerQuad);
			}

			cmd.AddBufferBarrier(bufferMP,
				ig::BarrierSync::VertexShading, ig::BarrierSync::ComputeShading,
				ig::BarrierAccess::ShaderResource, ig::BarrierAccess::UnorderedAccess);
			cmd.AddBufferBarrier(bufferVP,
				ig::BarrierSync::VertexShading, ig::BarrierSync::ComputeShading,
				ig::BarrierAccess::ShaderResource, ig::BarrierAccess::UnorderedAccess);
			cmd.FlushBarriers();
		}

		// Resolve render target to back buffer
		if (useMSAA)
		{
			cmd.AddTextureBarrier(*renderTarget, ig::SimpleBarrier::RenderTarget, ig::SimpleBarrier::ResolveSource);
			cmd.AddTextureBarrier(context->GetBackBuffer(), ig::SimpleBarrier::Discard, ig::SimpleBarrier::ResolveDest);
			cmd.FlushBarriers();

			cmd.ResolveTexture(*renderTarget, context->GetBackBuffer());

			cmd.AddTextureBarrier(context->GetBackBuffer(), ig::SimpleBarrier::ResolveDest, ig::SimpleBarrier::RenderTarget);
			cmd.FlushBarriers();

			cmd.SetRenderTarget(&context->GetBackBuffer());
			cmd.SetViewport((float)context->GetWidth(), (float)context->GetHeight());
			cmd.SetScissorRectangle(context->GetWidth(), context->GetHeight());
		}
	}
	cmd.End();

	context->Submit(cmd);
}

ig::Extent2D ParticleRenderer::GetHeatmapSize(ig::Extent2D backBufferSize, uint32_t heatmapPixelSize)
{
	uint32_t heatmapWidth = backBufferSize.width / heatmapPixelSize;
	uint32_t heatmapHeight = backBufferSize.height / heatmapPixelSize;
	return ig::Extent2D(heatmapWidth, heatmapHeight);
}