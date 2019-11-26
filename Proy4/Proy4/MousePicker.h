#pragma once
#include "GL/freeglut.h"
#include "vecmath/vecmath.h"
#include "vecmath/mat.h"
#include "vecmath/vec.h"
class MousePicker {
public:
	MousePicker(double projectionMatrix[], double viewMatrix[]) {
		this->projectionMatrix.getGLMatrix(projectionMatrix);
		this->viewMatrix.getGLMatrix(viewMatrix);
	}
	/*Vector3f getMouseRay() {
		return mouseRay;
	}*/
	Vec3d calculateRay(float mouseX, float mouseY) {
		Vec2d normalizedCoord = getNormalizedCoord(mouseX, mouseY);
		//Converted to homogeneous Clip Coordinates
		Vec4d rayClip = Vec4d(normalizedCoord[0], normalizedCoord[1], -1, 1);
		//Converted to eye coordinates
		Vec4d rayEye = eyeCoord(rayClip);
		//Converted to world coordinates
		Vec3d rayWorld = worldCoord(rayEye);
		return rayWorld;
	}
	
private:
	//Vector3f mouseRay = Vector3f(0,0,0);
	Mat4d viewMatrix = Mat4d();
	Mat4d projectionMatrix = Mat4d();
	//Normalised device coordinates
	Vec2d getNormalizedCoord(float mouseX, float mouseY) {
		float x = (mouseX * 2.f) / 640 - 1;
		float y = (mouseY * 2.f) / 480 - 1;
		return Vec2d(x, y);
	}
	//4d Eye (Camera) Coordinates
	Vec4d eyeCoord(Vec4d rayClip) {
		Mat4d invertedMatrix = projectionMatrix.inverse();
		Vec4d rayEye = MatrixCVec(invertedMatrix, rayClip);
		return Vec4d(rayEye[0], rayEye[1], -1.f, 0.f);
	}
	//4d World Coordinates
	Vec3d worldCoord(Vec4d rayEye) {
		Vec4d rayWorld4D = MatrixCVec(viewMatrix.inverse(), rayEye);
		Vec3d rayWorld = Vec3d(rayWorld4D[0], rayWorld4D[1], rayWorld4D[2]);
		rayWorld.normalize();
		return rayWorld;
	}
	Vec4d MatrixCVec(Mat4d mat, Vec4d vec) {
		float arr[] = {0,0,0,0};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				arr[i] += mat[i][j]*vec[j];
			}
		}
		return Vec4d(arr[0],arr[1],arr[2],arr[3]);
	}
};