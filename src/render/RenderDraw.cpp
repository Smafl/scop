#include "Render.hpp"
#include "../scene/Transformation.hpp"
#include "../scene/Material.hpp"

void Render::renderFrame(double deltaTime, Transformation &transformation, Material &material) {

	// Crears the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(_shaderProgram.getShaderProgram());


	// Upload uniforms
	glUniform1f(_uniformLocations.mixValue, material.materialBlend.mixValue);
	glUniform1i(_uniformLocations.useFaceColors, material.materialBlend.faceColorMode);

	if (transformation.animationState.isRotate) {
		if (deltaTime >= 1 / 60) {
			transformation.animationState.rotation += 0.25f * deltaTime * 60.0f;
		}
	}

	if (material.materialBlend.mixValue < material.materialBlend.targetMixValue) {
		material.materialBlend.mixValue += material.materialBlend.transitionSpeed * deltaTime;
		if (material.materialBlend.mixValue > material.materialBlend.targetMixValue)
			material.materialBlend.mixValue = material.materialBlend.targetMixValue;
	} else if (material.materialBlend.mixValue > material.materialBlend.targetMixValue) {
		material.materialBlend.mixValue -= material.materialBlend.transitionSpeed * deltaTime;
		if (material.materialBlend.mixValue < material.materialBlend.targetMixValue)
			material.materialBlend.mixValue = material.materialBlend.targetMixValue;
	}
}
