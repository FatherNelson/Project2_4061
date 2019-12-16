//
// Created by Grant Udstrand on 2019-12-05.
//

#ifndef EE5371_ROTATIONS_H
#define EE5371_ROTATIONS_H

/**
 * This function returns a rotation matrix for the given matrix rotated about X
 * @param theta
 * @return matrix
 */
Matrix3x3 aboutX(double theta){
	Matrix3x3 m = Matrix3x3(1,0,0,0,cos(theta),-sin(theta),0,sin(theta),cos(theta));
	return m;
}
/**
 * This function returns a rotation matrix for the given theta about Y
 * @param theta
 * @return
 */
Matrix3x3 aboutY(double theta){
	Matrix3x3 m = Matrix3x3(cos(theta),0,sin(theta),0,1,0,-sin(theta),0,cos(theta));
	return m;
}
/**
 * This function returns a rotation matrix for the given theta about Z
 * @param theta
 * @return
 */
Matrix3x3 aboutZ(double theta){
	Matrix3x3 m = Matrix3x3(cos(theta),-sin(theta),0,sin(theta),cos(theta),0,0,0,1);
	return m;
}
/**
 *
 * @param alpha
 * @param beta
 * @param gamma
 * @return
 */
Matrix3x3 byEuler(double alpha, double beta, double gamma){
	Vector3 v = Vector3(alpha, beta, gamma);
	Matrix3x3 m = Matrix3x3 :: FromEulerZXY(v);
	return m;
}
/**
 * Return the euler angles in terms of degrees
 * @param vec
 * @return
 */
Vector3 getInDegrees(Vector3 vec){
	Vector3 v;
	return v;
}
/**
 *
 * @param vec
 * @return
 */
Vector3 getInRadians(Vector3 vec){
	Vector3 v;
	return v;
}
#endif //EE5371_ROTATIONS_H
