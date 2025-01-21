#pragma once

enum class Scenario
{
	NBodyGalaxy20k = 0,
	Clusters,
	ClustersFormingBlackhole,
	TwoCollidingGalaxies320k,
	TwoCollidingGalaxies50M,
	NBody40k,
	BlackHoleFormation,
	BlackHoleFormationHeatmap,
	LinedNBody,
	CursorInteraction,
	SpiralAndRingGalaxy,
	ColorfulGalaxies,
	RingGalaxiesCollide,

	ScenarioCount,
};

// For imgui
static const char* ScenarioItems[] = 
{
	"20k NBody Galaxy",
	"Merging clusters",
	"Merging clusters form a black hole",
	"Two colliding galaxies (320k)",
	"Two colliding galaxies (50M)",
	"40k NBody random particles",
	"Black hole formation",
	"Black hole formation with heatmap",
	"Lined particles",
	"Cursor Interaction",
	"Spiral Galaxy VS Ring Galaxy",
	"Colorful galaxy collision",
	"Ring galaxies collide",
};
static const char* InteractWithParticlesHint = "Hold left/right mouse button to interact with particles.";

struct ImGuiInputParams
{
	Galaxy galaxy[2] = {};
	GalaxyCollisionParams colParam;
	GalaxyRingParams ringParam;
	LinedParticlesParams linedParam;
	RandomMassParticlesParams randParam;
};

struct ScenarioDesc
{
	ParticleState state;
	PhysicsParams physics;
	GraphicsParams graphics;
	ImGuiInputParams input;
};

ScenarioDesc GenerateScenarioDesc(Scenario scenario, ig::Extent2D backBufferSize);
