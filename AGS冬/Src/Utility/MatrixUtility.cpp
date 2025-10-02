#include "MatrixUtility.h"

MATRIX MatrixUtility::GetMatrixRotateXYZ(const VECTOR& euler)
{
	MATRIX ret = MGetIdent();
	ret = MMult(ret, MGetRotX(euler.x));
	ret = MMult(ret, MGetRotY(euler.y));
	ret = MMult(ret, MGetRotZ(euler.z));
	return ret;
}

MATRIX MatrixUtility::Multiplication(const MATRIX& child, const MATRIX& parent)
{
	// 親→子の順で掛ける
	return MMult(child, parent);
}
MATRIX MatrixUtility::Multiplication(const VECTOR& childEuler, const VECTOR& parentEuler)
{
	MATRIX parentMat = GetMatrixRotateXYZ(parentEuler);
	MATRIX childMat = GetMatrixRotateXYZ(childEuler);

	// 親→子の順で掛ける
	return MMult(childMat, parentMat);
}
