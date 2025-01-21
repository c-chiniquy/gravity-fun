#include "iglo.h"
#include "ShaderCommon.h"
#include "ColorGradient.h"
#include "Universe.h"
#include "GenerateParticles.h"
#include "ParticleRenderer.h"
#include "Scenarios.h"

ScenarioDesc GenerateScenarioDesc(Scenario scenario, ig::Extent2D backBufferSize)
{
	ScenarioDesc out;

	switch (scenario)
	{
	case Scenario::NBodyGalaxy20k:
		out.physics.maxVelocity = 100;
		out.physics.attractionSoftening = 1;
		out.physics.spaceScale = 1;
		out.physics.timeScale = 1;
		out.physics.G = 1;

		out.graphics.drawMode = DrawMode::Rects;
		out.graphics.permBlend = PermBlendStates::AdditiveBlend;
		out.graphics.permColor = PermColor::Uniform;
		out.graphics.permMSAA = PermMSAA::MSAA_Enabled;
		out.graphics.uniformColor = ig::Color32(16, 32, 64);
		out.graphics.heatmapPixelSize = 1;

		out.input.galaxy[0].numRings = 200;
		out.input.galaxy[0].numBands = 100;
		out.input.galaxy[0].blackHoleMass = 50;
		out.input.galaxy[0].ringRadius = 100;
		out.input.galaxy[0].centerEmptySpaceRadius = 20;

		out.input.galaxy[1] = out.input.galaxy[0];

		out.input.ringParam.enableRingParticleMass = true;
		out.input.ringParam.ringParticleMass = 0.001f;
		out.input.ringParam.extraRingVel = 20;

		out.state = GenerateCenteredGalaxy(out.input.galaxy[0], out.input.ringParam, out.physics, backBufferSize);
		break;

	case Scenario::Clusters:
	case Scenario::ClustersFormingBlackhole:
		out.physics.maxVelocity = 100;
		out.physics.attractionSoftening = 1;
		out.physics.spaceScale = 1;
		out.physics.timeScale = 1;
		out.physics.G = 1;

		out.graphics.drawMode = DrawMode::Rects;
		out.graphics.permBlend = PermBlendStates::AdditiveBlend;
		out.graphics.permColor = PermColor::Uniform;
		out.graphics.permMSAA = PermMSAA::MSAA_Enabled;
		out.graphics.uniformColor = ig::Color32(16, 32, 64);
		out.graphics.heatmapPixelSize = 1;

		out.input.galaxy[0].numRings = 100;
		out.input.galaxy[0].numBands = 150;
		out.input.galaxy[0].blackHoleMass = 0.01f;
		out.input.galaxy[0].ringRadius = 100;
		out.input.galaxy[0].centerEmptySpaceRadius = 40;

		out.input.galaxy[1] = out.input.galaxy[0];

		out.input.ringParam.enableRingParticleMass = true;
		out.input.ringParam.ringParticleMass = 0.01f;
		out.input.ringParam.extraRingVel = 20;

		out.input.colParam.velocity = 0.4f;
		out.input.colParam.distFromCenterX = 200;
		out.input.colParam.distFromCenterY = 100;

		if (scenario == Scenario::ClustersFormingBlackhole)
		{
			out.physics.maxVelocity = 4;
			out.physics.attractionSoftening = 2;
			out.physics.timeScale = 0.5f;
		}
		out.state = GenerateCollidingGalaxies(out.input.galaxy, 2, out.input.colParam, out.input.ringParam, out.physics, backBufferSize);
		break;


	case Scenario::TwoCollidingGalaxies320k:
	case Scenario::TwoCollidingGalaxies50M:
	case Scenario::SpiralAndRingGalaxy:
	case Scenario::ColorfulGalaxies:
	case Scenario::RingGalaxiesCollide:
		out.physics.maxVelocity = 2;
		out.physics.attractionSoftening = 0;
		out.physics.spaceScale = 1;
		out.physics.timeScale = 1;
		out.physics.G = 1;

		out.graphics.drawMode = DrawMode::Rects;
		out.graphics.permBlend = PermBlendStates::AdditiveBlend;
		out.graphics.permColor = PermColor::Uniform;
		out.graphics.permMSAA = PermMSAA::MSAA_Enabled;
		out.graphics.uniformColor = ig::Color32(128, 4, 1);
		out.graphics.heatmapPixelSize = 1;

		out.input.galaxy[0].numRings = 400;
		out.input.galaxy[0].numBands = 400;
		out.input.galaxy[0].blackHoleMass = 50;
		out.input.galaxy[0].ringRadius = 40;
		out.input.galaxy[0].centerEmptySpaceRadius = 20;

		out.input.galaxy[1] = out.input.galaxy[0];

		out.input.ringParam.enableRingParticleMass = false;
		out.input.ringParam.extraRingVel = 0;

		out.input.colParam.velocity = 0.4f;
		out.input.colParam.distFromCenterX = 280;
		out.input.colParam.distFromCenterY = 100;

		if (scenario == Scenario::TwoCollidingGalaxies50M)
		{
			out.input.galaxy[0].numRings = 5000;
			out.input.galaxy[0].numBands = 5000;
			out.input.galaxy[0].ringRadius = 100;
			out.input.galaxy[0].blackHoleMass = 20.0f;

			out.input.galaxy[1] = out.input.galaxy[0];

			out.input.colParam.distFromCenterX = 150;
			out.input.colParam.distFromCenterY = 120;

			out.graphics.drawMode = DrawMode::Heatmap_Color;
			out.graphics.clearHeatmap = true;
			out.graphics.heatmapPixelSize = 1;
		}
		if (scenario == Scenario::SpiralAndRingGalaxy)
		{
			out.input.galaxy[0].numRings = 2;
			out.input.galaxy[0].numBands = 100000;
			out.input.galaxy[1].numRings = 100000;
			out.input.galaxy[1].numBands = 2;
		}
		if (scenario == Scenario::ColorfulGalaxies)
		{
			out.physics.maxVelocity = 1000;
			out.physics.attractionSoftening = 0;

			out.graphics.permBlend = PermBlendStates::BlendDisabled;
			out.graphics.permColor = PermColor::Gradient;

			out.input.galaxy[0].numRings = 200;
			out.input.galaxy[0].numBands = 200;
			out.input.galaxy[1].numRings = 200;
			out.input.galaxy[1].numBands = 200;

			out.input.colParam.velocity = 0.3f;
			out.input.colParam.distFromCenterX = 120;
			out.input.colParam.distFromCenterY = 45;
		}
		if (scenario == Scenario::RingGalaxiesCollide)
		{
			out.physics.maxVelocity = 1000;
			out.physics.attractionSoftening = 0;

			out.input.galaxy[0].numRings = 1;
			out.input.galaxy[0].numBands = 1000000;
			out.input.galaxy[0].centerEmptySpaceRadius = 50;
			out.input.galaxy[1].numRings = 1;
			out.input.galaxy[1].numBands = 1000000;
			out.input.galaxy[1].centerEmptySpaceRadius = 50;

			out.input.colParam.velocity = 0.3f;
			out.input.colParam.distFromCenterX = 120;
			out.input.colParam.distFromCenterY = 45;
		}

		out.state = GenerateCollidingGalaxies(out.input.galaxy, 2, out.input.colParam, out.input.ringParam, out.physics, backBufferSize);
		break;

	case Scenario::NBody40k:
	case Scenario::BlackHoleFormation:
	case Scenario::BlackHoleFormationHeatmap:
		out.physics.maxVelocity = 20;
		out.physics.attractionSoftening = 10;
		out.physics.spaceScale = 4;
		out.physics.timeScale = 1;
		out.physics.G = 1;

		out.graphics.drawMode = DrawMode::Rects;
		out.graphics.permBlend = PermBlendStates::AdditiveBlend;
		out.graphics.permColor = PermColor::Uniform;
		out.graphics.permMSAA = PermMSAA::MSAA_Enabled;
		out.graphics.uniformColor = ig::Color32(100, 50, 25);
		out.graphics.heatmapPixelSize = 1;

		out.input.randParam.numParticles = 40000;
		out.input.randParam.minMass = 1;
		out.input.randParam.maxMass = 1;

		if (scenario == Scenario::BlackHoleFormation ||
			scenario == Scenario::BlackHoleFormationHeatmap)
		{
			out.physics.maxVelocity = 20;
			out.physics.attractionSoftening = 10;
			out.physics.spaceScale = 1;
			out.physics.timeScale = 0.2f;
			out.physics.G = 0.5f;

			out.graphics.drawMode = DrawMode::Points;

			if (scenario == Scenario::BlackHoleFormationHeatmap)
			{
				out.graphics.drawMode = DrawMode::Heatmap_Grayscale;
				out.graphics.clearHeatmap = false;
			}
		}

		out.state = GenerateRandomMassParticles(out.input.randParam, backBufferSize);
		break;

	case Scenario::LinedNBody:
		out.physics.maxVelocity = 50;
		out.physics.attractionSoftening = 10;
		out.physics.spaceScale = 1;
		out.physics.timeScale = 1;
		out.physics.G = 0.1f;

		out.graphics.drawMode = DrawMode::Rects;
		out.graphics.permBlend = PermBlendStates::AdditiveBlend;
		out.graphics.permColor = PermColor::Gradient;
		out.graphics.permMSAA = PermMSAA::MSAA_Enabled;
		out.graphics.heatmapPixelSize = 1;

		out.input.linedParam.spacing = 24;
		out.input.linedParam.minMass = 1;
		out.input.linedParam.maxMass = 1;

		out.state = GenerateLinedParticles(out.input.linedParam, false, backBufferSize);
		break;

	case Scenario::CursorInteraction:
		out.physics.maxVelocity = 5;
		out.physics.attractionSoftening = 700;
		out.physics.spaceScale = 1;
		out.physics.timeScale = 1;
		out.physics.G = 1;

		out.physics.cursorInteractionEnabled = true;
		out.physics.cursorMass = 1000;

		out.graphics.drawMode = DrawMode::Points;
		out.graphics.permBlend = PermBlendStates::AdditiveBlend;
		out.graphics.permColor = PermColor::Uniform;
		out.graphics.permMSAA = PermMSAA::MSAA_Enabled;
		out.graphics.uniformColor = ig::Color32(64, 2, 1);
		out.graphics.heatmapPixelSize = 1;

		out.input.linedParam.spacing = 0;
		out.state = GenerateLinedParticles(out.input.linedParam, true, backBufferSize);
		break;

	}

	return out;
}