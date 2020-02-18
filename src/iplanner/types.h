#ifndef IPLANNER_TYPES_H_
#define IPLANNER_TYPES_H_

#include <Eigen/Dense>

namespace iplanner
{
// Vector/matrices
using Vector2i = Eigen::Vector2i;
using Vector2f = Eigen::Vector2f;
using Vector2d = Eigen::Vector2d;

using Vector3i = Eigen::Vector3i;
using Vector3f = Eigen::Vector3f;
using Vector3d = Eigen::Vector3d;

using Vector4i = Eigen::Vector4i;
using Vector4f = Eigen::Vector4f;
using Vector4d = Eigen::Vector4d;

using Matrix2f = Eigen::Matrix2f;
using Matrix2d = Eigen::Matrix2d;

using Matrix3f = Eigen::Matrix3f;
using Matrix3d = Eigen::Matrix3d;

using Matrix4f = Eigen::Matrix4f;
using Matrix4d = Eigen::Matrix4d;

using Affine3f = Eigen::Affine3f;
using Affine3d = Eigen::Affine3d;

using AngleAxisf = Eigen::AngleAxisf;
using AngleAxisd = Eigen::AngleAxisd;

using VectorXf = Eigen::VectorXf;
using VectorXd = Eigen::VectorXd;

template<typename T>
using VectorX = Eigen::Matrix<T, -1, 1>;

template<typename T, int rows>
using Vector = Eigen::Matrix<T, rows, 1>;

template<typename T, int rows, int cols>
using Matrix = Eigen::Matrix<T, rows, cols>;
}

#endif // IPLANNER_TYPES_H_
