//
// Created by Grant Udstrand on 2019-12-06.
//

#ifndef EE5371_UNIT_TEST_H
#define EE5371_UNIT_TEST_H
/**
 * This test performs a basic POC of the different conversions within the program, showing that you can in fact
 * convert between all forms freely
 */
void unit_test(){
	Vector3 vec = Vector3(0, 0, 0);
	Matrix3x3 base_matrix = Matrix3x3::FromEulerYXZ(vec);
	Matrix3x3 matrix_from_euler = Matrix3x3::FromEulerYXZ(vec);
	Vector3 thing_tran = Matrix3x3::ToEulerYXZ(base_matrix);
	Quaternion quaternion_from_euler = Quaternion :: FromEuler(vec);
	Quaternion quaternion_from_matrix = Matrix3x3::ToQuaternion(base_matrix);
	Matrix3x3 matrix_from_quaternion = Matrix3x3:: FromQuaternion(quaternion_from_euler);
	Vector3 thing_euler = Quaternion::ToEuler(quaternion_from_matrix);

	printf("Matrix Form: \n");
	matrix_print(base_matrix);
	printf("\n\n");

	printf("Matrix (from euler): \n");
	matrix_print(matrix_from_euler);
	printf("\n\n");

	printf("Matrix (from quaternion):\n");
	matrix_print(matrix_from_quaternion);
	printf("\n\n");

	printf("Quaternion (from matrix): \n");
	printf("%f + %fi + %fj + %fk \n", quaternion_from_matrix.W, quaternion_from_matrix.X, quaternion_from_matrix.Y, quaternion_from_matrix.Z);
	printf("\n");

	printf("Quaternion (from euler)\n");
	printf("%f + %fi + %fj + %fk\n", quaternion_from_euler.W, quaternion_from_euler.X, quaternion_from_euler.Y, quaternion_from_euler.Z);
	printf("\n");
//
////	printf("Quaternion (true) \n");
//
//
	printf("Euler Angle (from quaternion) Form: \n");
	printf("%f %f %f\n", thing_euler.X, thing_euler.Y, thing_euler.Z);
	printf("\n");

	printf("Euler Angle (from matrix) Form: \n");
	printf("%f %f %f\n", thing_tran.X, thing_tran.Y, thing_tran.Z);
	printf("\n");

	printf("Euler Angle (true): \n");
	printf("%f %f %f\n", vec.X, vec.Y, vec.Z);
	printf("\n");
}
#endif //EE5371_UNIT_TEST_H
