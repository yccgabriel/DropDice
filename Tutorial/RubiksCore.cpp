#include "RubiksCore.h"
#include "RayTracer.h"
#include "Floor.h"

#ifdef _DEBUG
std::ostream& operator<<(std::ostream& out, const glm::mat4& m)
{
	out << m[0][0] << "\t" << m[0][1] << "\t" << m[0][2] << "\t" << m[0][3] << "\n";
	out << m[1][0] << "\t" << m[1][1] << "\t" << m[1][2] << "\t" << m[1][3] << "\n";
	out << m[2][0] << "\t" << m[2][1] << "\t" << m[2][2] << "\t" << m[2][3] << "\n";
	out << m[3][0] << "\t" << m[3][1] << "\t" << m[3][2] << "\t" << m[3][3] << "\n";
	return out;
}
std::ostream& operator<<(std::ostream& out, const glm::vec3& v)
{
	out << v.x << ",\t" << v.y << ",\t" << v.z;
	return out;
}
#endif


RubiksCore::RubiksCore()
{
	mXRod = rod.CreateInstance();
	mYRod = rod.CreateInstance();
	mZRod = rod.CreateInstance();
	
	rod.RotateInstanceInLocalSpace(mXRod, glm::mat4());
	rod.RotateInstanceInLocalSpace(mYRod, glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));	// rotate 90 degrees about Z-axis
	rod.RotateInstanceInLocalSpace(mZRod, glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f)));	// rotate 90 degrees about negative Y-axis
}

RubiksCore::~RubiksCore()
{
}

void RubiksCore::Draw()
{
	mXRod->transform = transform*mXRod->localtrans;
	mYRod->transform = transform*mYRod->localtrans;
	mZRod->transform = transform*mZRod->localtrans;
	rod.DrawInstance(mXRod, glm::vec3(1.0f, 0.0f, 0.0f));	// red
	rod.DrawInstance(mYRod, glm::vec3(0.0f, 1.0f, 0.0f));	// green
	rod.DrawInstance(mZRod, glm::vec3(0.0f, 0.0f, 1.0f));	// blue
}

void RubiksCore::ClickOnRod(Instance* instance, RayTracer::Line cursor)
{
	mTranslateCore = true;
	mActiveInstance = instance;
	mActiveInstanceType = RubiksCore::ROD;

	glm::vec3 boxMin = glm::vec3(instance->transform*glm::vec4(instance->mBoxMinXYZ, 1));
	glm::vec3 boxMax = glm::vec3(instance->transform*glm::vec4(instance->mBoxMaxXYZ, 1));
	RayTracer::Line diagonal(boxMin, boxMax - boxMin);
	glm::vec3 intersection = RayTracer::SkewLinesNearestPoint(diagonal, cursor);
	float lambda = instance->lambda = glm::length(intersection - boxMin) / glm::length(boxMax - boxMin);

	// Set reference point
	mReferenceLambdaPoint = boxMin + lambda*(boxMax - boxMin);
	mReferenceAttachedInstancePosition = glm::vec3(mAttachedInstance->transform*glm::vec4());

	// calculate Rod axis
	RT::Line unitX(RT::Line(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)));
	mActiveRodAxis = instance->transform * unitX;
}

void RubiksCore::ClickOnCube(Instance* instance, RayTracer::Line cursor)
{
	mRotateCore = true;
	mActiveInstance = instance;
	mActiveInstanceType = RubiksCore::CUBE;
}

void RubiksCore::Release()
{
	mTranslateCore = false;
	mRotateCore = false;
	mActiveInstance = nullptr;
	mActiveInstanceType = RubiksCore::NTH;
	//mReferenceLambdaPoint = glm::vec3();
//	std::cout << "XRod:" << std::endl;
//	std::cout << mXRod->transform << std::endl;
//	std::cout << "YRod:" << std::endl;
//	std::cout << mYRod->transform << std::endl;
}

void RubiksCore::TranslateCore(const glm::vec3& cam_pos, const glm::vec3& ray_dir)
{
	RayTracer::Line cameraRay(cam_pos, ray_dir);	// ray in model space
	Instance* ins = mActiveInstance;
	glm::vec3 lambdaPoint = glm::vec3(ins->transform * glm::vec4(ins->mBoxMinXYZ + ins->lambda*(ins->mBoxMaxXYZ - ins->mBoxMinXYZ), 1));
	glm::vec3 nearestPoint = RT::SkewLinesNearestPoint(RT::Line(lambdaPoint, mActiveRodAxis.direction), cameraRay);

	glm::vec3 translationVector = nearestPoint - mReferenceLambdaPoint;
	
	Floor::TranslateInstance(mAttachedInstance, mReferenceAttachedInstancePosition, translationVector);
	this->transform = mAttachedInstance->transform;
}