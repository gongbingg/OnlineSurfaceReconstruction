#pragma once

#include <nanogui/window.h>
#include <nanogui/label.h>

#include "Data.h"
#include "GLBuffer.h"
#include "GLVertexArray.h"
#include "common.h"
#include "Camera.h"
#include "AbstractViewer.h"
#include "Tool.h"
#include "Selection.h"

//Represents a modification tool that fills a hole in a point cloud by plane fitting and RBF-based reconstruction
class FillHoleTool : public Tool
{
public:
	FillHoleTool(AbstractViewer* viewer, DataGL& data, float& selectionRadius);

	void enterTool();
	void exitTool();

	void draw(const Matrix4f& mv, const Matrix4f& proj);

	bool mouseButtonEvent(const Eigen::Vector2i & p, int button, bool down, int modifiers);
	bool mouseMotionEvent(const Eigen::Vector2i & p, const Eigen::Vector2i & rel, int button, int modifiers);
	bool scrollEvent(const Eigen::Vector2i & p, const Eigen::Vector2f & rel);

private:

	const double fillRatePerSecond = 0.6; //the percentage of the selection circle that is filled within a second

	enum State
	{
		DefineSupport,
		AddPoints,
		AddingPoints,
	};

	State state;

	//returns the value of the underlying RBF
	float rbf(float);
	//returns the derivative of the underlying RBF
	float rbfDeriv(float);

	//calculates the supporting plane and RBF weights for reconstruction
	void calculateBasis();

	//generates n points in the given sphere
	void addPoints(const Vector3f& center, float radius, int n);

	void resetSupport();

	AbstractViewer* viewer;
	nanogui::Window* window;
	nanogui::Label* lblStatus;

	Selection support;

	GLBuffer planePositionsBuffer;
	GLVertexArray planeVAO;

	float& selectionRadius;

	DataGL& data;

	//Control points for the RBFs (subset of selected points)
	std::vector<THierarchy::VertexIndex> rbfCenters;
	Eigen::Matrix<double, Eigen::Dynamic, 4> weights;
	Vector3f centroid;
	Matrix3f eigenVectors;

	int pixelsMoved;

	std::chrono::high_resolution_clock::time_point lastPointsAdded;

	std::mt19937 rnd;
	Scan* scan;

	static int scanNr;
};