#pragma once
//#include "stdafx.h"
#include "MFC_OSG2.h"
#include <algorithm>
#define NUM_SOLUTIONS	2	/* Number of possible solutions  */
#define NUM_DOFS		6	/* Number of joints to be solved */
//#define PI 3.14159265358979323846

#define bool2BOOL(trueOrfalse)	\
	( trueOrfalse == true ) ? TRUE : FALSE

#define BOOL2bool(TRUE_Or_FALSE)	\
	(( TRUE_Or_FALSE == TRUE ) ? true : false)

class COptDeviceAttribute;

#define pi_de 57.29577951
#define de_pi 0.017453292

#define JOINT_MOTION	1
#define LINEAR_MOTION	2
#define SHORTESTANGLE	1
#define TURNNUMBER		2

#define TYPE_WORKCELL	0x01
#define TYPE_DEVICE		0x02
#define TYPE_PART		0x03
#define TYPE_PATH		0x04
#define TYPE_TAG		0x05
#define TYPE_FRAME		0x06

class OPoint3D
{
public:
	~OPoint3D();

	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.
	float		z;	//!< The z-coordinate of a 3-dimensional point.

	/*! The default constructor. */
	OPoint3D() : x(0), y(0), z(0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	OPoint3D(float X, float Y, float Z = 0.0f) : x(X), y(Y), z(Z) {};

	/*! This constructor accepts a pointer to an OPoint3D object. */
	OPoint3D(OPoint3D const * p) : x(p->x), y(p->y), z(p->z) {};

	/*! This constructor accepts an OPoint3D reference. */
	OPoint3D(OPoint3D const & p) : x(p.x), y(p.y), z(p.z) {};

	/*!  This method packs an OPoint3D object with coordinate values.  */
	void Set(float X, float Y, float Z = 0.0f) { x = X; y = Y; z = Z; };

	/*!  This method packs an OPoint3D object with coordinate values from a given OPoint3D.  */
	void Set(OPoint3D *p) { x = p->x; y = p->y; z = p->z; };

	/*!  This method packs an OPoint3D object with coordinate values from a given OPoint3D.  */
	void Set(OPoint3D const *p) { x = p->x; y = p->y; z = p->z; };

	/*!  This method packs an OPoint3D object with coordinate values from a given OPoint3D that is passed by reference.  */
	void Set(OPoint3D &p) { x = p.x; y = p.y; z = p.z; };

	/*!  This method packs an OPoint3D object with coordinate values from a given OPoint3D that is passed by reference.  */
	void Set(const OPoint3D &p) { x = p.x; y = p.y; z = p.z; };

	/*!  This method increases/decreases the coordinate values of an existing OPoint3D object.
	\param X The amount in which you want to increment or decrement the x coordinate.
	\param Y The amount in which you want to increment or decrement the y coordinate.
	\param Z The amount in which you want to increment or decrement the z coordinate.
	*/
	void Add(float X, float Y, float Z = 0.0){ x += X; y += Y; z += Z; };

	OPoint3D const operator -(const OPoint3D &p2) const
	{
		return OPoint3D(x - p2.x, y - p2.y, z - p2.z);
	}

	OPoint3D const operator +(const OPoint3D &p2) const
	{
		return OPoint3D(x + p2.x, y + p2.y, z + p2.z);
	}

	OPoint3D const operator *(float const rhs) const
	{
		return OPoint3D(x*rhs, y*rhs, z*rhs);
	}

	OPoint3D const operator /(float const rhs) const
	{
		return OPoint3D(x / rhs, y / rhs, z / rhs);
	}

	OPoint3D const & operator += (OPoint3D const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	OPoint3D const & operator -= (OPoint3D const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	OPoint3D const & operator *= (float const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	OPoint3D const & operator /= (float const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator ==(OPoint3D const & rhs) const
	{
		return Equal(&rhs);
	}

	bool operator !=(OPoint3D const & rhs) const
	{
		return !Equal(&rhs);
	}


	/*!\def OPoint3D_EPSILON */
#define OPoint3D_EPSILON (1e-5f)  //!<  OPoint3D_EPSILON


	/*! This method checks if the xyz components of the point are equal.  */
	bool Equal(OPoint3D const *p, float epsi = OPoint3D_EPSILON) const {
		return (bool)(
			(x - p->x < epsi) && (x - p->x > -epsi) &&
			(y - p->y < epsi) && (y - p->y > -epsi) &&
			(z - p->z < epsi) && (z - p->z > -epsi));
	};

	/*!  This method checks if the xyz components of the point are equal.  */
	bool Equal(OPoint3D const &p, float epsi = OPoint3D_EPSILON) const {
		return Equal(&p, epsi);
	};
};

class OPoint2D
{
public:
	~OPoint2D();

	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.

	/*! The default constructor. */
	OPoint2D() : x(0), y(0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	OPoint2D(float X, float Y = 0.0f) : x(X), y(Y) {};

	/*! This constructor accepts a pointer to an OPoint2D object. */
	OPoint2D(OPoint2D const * p) : x(p->x), y(p->y) {};

	/*! This constructor accepts an OPoint2D reference. */
	OPoint2D(OPoint2D const & p) : x(p.x), y(p.y) {};

	/*!  This method packs an OPoint2D object with coordinate values.  */
	void Set(float X, float Y = 0.0f) { x = X; y = Y; };

	/*!  This method packs an OPoint2D object with coordinate values from a given OPoint2D.  */
	void Set(OPoint2D *p) { x = p->x; y = p->y; };

	/*!  This method packs an OPoint2D object with coordinate values from a given OPoint2D.  */
	void Set(OPoint2D const *p) { x = p->x; y = p->y; };

	/*!  This method packs an OPoint2D object with coordinate values from a given OPoint2D that is passed by reference.  */
	void Set(OPoint2D &p) { x = p.x; y = p.y; };

	/*!  This method packs an OPoint2D object with coordinate values from a given OPoint2D that is passed by reference.  */
	void Set(const OPoint2D &p) { x = p.x; y = p.y; };

	/*!  This method increases/decreases the coordinate values of an existing OPoint2D object.
	\param X The amount in which you want to increment or decrement the x coordinate.
	\param Y The amount in which you want to increment or decrement the y coordinate.
	*/
	void Add(float X, float Y = 0.0){ x += X; y += Y; };

	OPoint2D const operator -(const OPoint2D &p2) const
	{
		return OPoint2D(x - p2.x, y - p2.y);
	}

	OPoint2D const operator +(const OPoint2D &p2) const
	{
		return OPoint2D(x + p2.x, y + p2.y);
	}

	OPoint2D const operator *(float const rhs) const
	{
		return OPoint2D(x*rhs, y*rhs);
	}

	OPoint2D const operator /(float const rhs) const
	{
		return OPoint2D(x / rhs, y / rhs);
	}

	OPoint2D const & operator += (OPoint2D const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	OPoint2D const & operator -= (OPoint2D const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	OPoint2D const & operator *= (float const rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	OPoint2D const & operator /= (float const rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	bool operator ==(OPoint2D const & rhs) const
	{
		return Equal(&rhs);
	}

	bool operator !=(OPoint2D const & rhs) const
	{
		return !Equal(&rhs);
	}


	/*! This method checks if the xyz components of the point are equal.  */
	bool Equal(OPoint2D const *p, float epsi = OPoint3D_EPSILON) const {
		return (bool)(
			(x - p->x < epsi) && (x - p->x > -epsi) &&
			(y - p->y < epsi) && (y - p->y > -epsi));
	};

	/*!  This method checks if the xyz components of the point are equal.  */
	bool Equal(OPoint2D const &p, float epsi = OPoint3D_EPSILON) const {
		return Equal(&p, epsi);
	};
};

class MATRIX44 : public CObject
{
public:
	DECLARE_SERIAL(MATRIX44)
	MATRIX44(void);
	~MATRIX44(void);
	MATRIX44(const MATRIX44& s)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				this->m[i][j] = s.m[i][j];
			}
		}
		this->m_bIdentity = s.m_bIdentity;
	};
	float m[4][4];
	bool m_bIdentity;
	void Identity();
	void GetTr(float(*mat)[4]);
	void SetTr(float(*mat)[4]);
	void GetTr(double(*mat)[4]);
	void SetTr(double(*mat)[4]);
	void AppendMatrix(float(*mat)[4]);
	void AppendMatrix(double(*mat)[4]);
	void GetTr(MATRIX44& mat);
	void SetTr(MATRIX44 mat);
	void AppendMatrix(MATRIX44 mat);
	void Tranfrom_Point(OPoint3D& in_point, OPoint3D& out_point);
	void Tranfrom_Point(OPoint3D* in_point, OPoint3D& out_point);
	void Tranfrom_Point(float x, float y, float z, OPoint3D& out_point);
	void GetInvTr(MATRIX44& mat);
	void GetInvTr(float(*mat)[4]);
	void GetInvTr(double(*mat)[4]);
	OPoint3D Tranfrom_Point(OPoint3D& in_point);
	void Serialize(CArchive& ar);
};

class MATRIX43
{
public:
	MATRIX43();
	~MATRIX43();

	float m[4][3];
};

typedef struct {
	int	row;
	int	col;
}	MATHEAD;

typedef struct {
	MATHEAD	head;
	/*
	* only the starting address of the following will be
	* returned to the C programmer, like malloc() concept
	*/
	double	*matrix;
}	MATBODY;

typedef	double	**MATRIX;
typedef	float 	**MATRIXf;

#define	Mathead(a)	((MATHEAD *)((MATHEAD *)(a) - 1))
#define MatRow(a)	(Mathead(a)->row)
#define	MatCol(a)	(Mathead(a)->col)

/*
*----------------------------------------------------------------------------
*	mat_errors definitions
*----------------------------------------------------------------------------
*/
#define	MAT_MALLOC	1
#define MAT_FNOTOPEN	2
#define	MAT_FNOTGETMAT	3

/*
*----------------------------------------------------------------------------
*	matrice types
*----------------------------------------------------------------------------
*/
#define UNDEFINED	-1
#define ZERO_MATRIX	0
#define	UNIT_MATRIX	1

#define MAXTCPNUM		10
#define MAXBASEAUXNUM	6
#define MAXMOUNTAUXNUM	6

class COptUtil
{
public:
	COptUtil(void);

	~COptUtil(void);
	static float _IGRIP_COLOR_INDEX[35][3];
	static float _IGRIP_TO_ROBCAD_COLOR_INDEX[35];
	static float _ROBCAD_COLOR_INDEX[18][3];
	static char* _ROBCAD_COLOR_NAME[18];

	static int GetRobcadColorIndex(CString color_name);
	static int GetIgripColorIndex(COLORREF color_ref);
	static int SplitString(CString& str_input, vector< char >& split_chrs_input, vector< CString >& str_output);
	static int SplitString(char* str_input, vector< char >& split_chrs_input, vector< CString >& str_output);
	static int SplitString(string& str_input, vector< char >& split_chrs_input, vector< string >& str_output);

	static void DrawAxis(CString axis_name);
	static void Make_Mat_YPR(double tx, double ty, double tz, double yaw, double pitch, double roll, double(*mat)[4]);
	static void Make_Mat_RXYZ(double tx, double ty, double tz, double rx, double ry, double rz, double(*mat)[4]);
	static int Make_Pos_YPR(double(*mat)[4], double *tx, double *ty, double *tz, double *yaw, double *pitch, double *roll);
	static int Make_Pos_RXYZ(double(*mat)[4], double *tx, double *ty, double *tz, double *rx, double *ry, double *rz);
	static void Make_Mat_YPR(float tx, float ty, float tz, float yaw, float pitch, float roll, float(*mat)[4]);
	static void Make_Mat_RXYZ(float tx, float ty, float tz, float rx, float ry, float rz, float(*mat)[4]);
	static int Make_Pos_YPR(float(*mat)[4], float *tx, float *ty, float *tz, float *yaw, float *pitch, float *roll);
	static int Make_Pos_RXYZ(float(*mat)[4], float *tx, float *ty, float *tz, float *rx, float *ry, float *rz);
	static void Compute_Matrix_Product(double(*mat1)[4], double(*mat2)[4], double(*out_mat)[4]);
	static void Compute_Matrix_Product(float(*mat1)[4], float(*mat2)[4], float(*out_mat)[4]);
	static void Compute_Matrix_Inverse(double(*in_mat)[4], double(*out_mat)[4]);
	static void Compute_Matrix_Inverse(float(*in_mat)[4], float(*out_mat)[4]);
	static void Copy_Matrix(double(*in_mat)[4], double(*out_mat)[4]);
	static void Copy_Matrix(float(*in_mat)[4], float(*out_mat)[4]);
	static void Copy_Matrix(double(*in_mat)[4], float(*out_mat)[4]);
	static void Copy_Matrix(float(*in_mat)[4], double(*out_mat)[4]);
	static void Identity_Matrix(float(*m)[4]);
	static void Identity_Matrix(double(*m)[4]);

	static double IGRIP_SSSA(double a, double b, double c);
	static double IGRIP_SASS(double a, double b, double c);
	static double IGRIP_SASA(double a, double b, double c);
	static CString IGRIP_SSSA(CString s1, CString s2, CString s3);
	static CString IGRIP_SASS(CString s1, CString a1, CString s2);
	static CString IGRIP_SASA(CString s1, CString a1, CString s2);

	static MATRIX mat_error(int errno);
	static MATRIX _mat_creat(int row, int col);
	static MATRIX mat_creat(int row, int col, int type);
	static MATRIX mat_fill(MATRIX A, int type);
	static int mat_free(MATRIX A);
	static MATRIX mat_copy(MATRIX A);
	static MATRIX mat_colcopy1(MATRIX A, MATRIX B, int cola, int colb);
	static int fgetmat(MATRIX A, FILE *fp);
	//static MATRIX mat_dumpf	();
	//static MATRIX mat_dump		();
	//static MATRIX mat_fdump	();
	//static MATRIX mat_fdumpf       ();

	static MATRIX mat_add(MATRIX A, MATRIX B);
	static MATRIX mat_sub(MATRIX A, MATRIX B);
	static MATRIX mat_mul(MATRIX A, MATRIX B);
	static double mat_diagmul(MATRIX A);
	static MATRIX mat_tran(MATRIX A);
	static MATRIX mat_inv(MATRIX a);
	static MATRIX mat_SymToeplz(MATRIX R);

	static int mat_lu(MATRIX A, MATRIX P);
	static MATRIX mat_backsubs1(MATRIX A, MATRIX B, MATRIX X, MATRIX P, int xcol);
	static MATRIX mat_lsolve(MATRIX a, MATRIX b);

	static MATRIX mat_submat(MATRIX A, int i, int j);
	static double mat_cofact(MATRIX A, int i, int j);
	static double mat_det(MATRIX a);
	static double mat_minor(MATRIX A, int i, int j);

	static MATRIX mat_durbin(MATRIX R);
	static MATRIX mat_lsolve_durbin(MATRIX A, MATRIX B);
	//static int Type_S_Inverse_Kin(	
	//	double mat[4][4],		//TCP 값
	//	int config[3],			//config 값
	//	double aa[6],			//
	//	double dd[6],			//
	//	int sign[6],			//joint 회전 방향 값
	//	double jointOffset[6],	//joint 회전 offset 값
	//	double alpha,			//5축 offset alpha 값
	//	double deltaX[6],		//Calibration delta vlaue
	//	double deltaY[6], 
	//	double deltaZ[6], 
	//	double deltaW[6], 
	//	double deltaP[6], 
	//	double deltaR[6], 
	//	double theta[6]);		//theta 값

	static int XYZWPRtoMAT(double(*result)[4], double pos[]);
	static int MultiplyMAT(double(*result)[4], double(*mat1)[4], double(*mat2)[4]);
	static int MultiplyMAT2(double(*result)[4], double(*mat)[4][4], int matsize);
	static int InverseMAT(double(*result)[4], double(*mat)[4]);
	static int BaseAuxFK(double(*CurRobPosMatFromBaseAux)[4], double(*BaseAuxParaMat)[4][4], double CurBaseAuxTheta[], int BaseAuxNum, int BaseAuxType[], double(*BaseAuxJointDV)[4][4]);
	static int RobotFK(double(*CurrentTcpPos)[4],
		double CurRobJointTheta[], double aa[], double dd[], double alpha,
		int RobJointSign[], double RobJointOffset[],
		double(*RobJointDV)[4][4], double(*MountMat)[4], double(*TcpMat)[4], double linkT[][4][4]);
	static int RobotIK(double theta[],		//theta[6]
		double(*TargetPos0T6)[4],		//0T6 값
		double aa[], double dd[],		//Link 값
		double alpha,					//로봇 5축 offset
		double(*RobJointDV)[4][4],		//로봇 Delta Vector Calibration 값
		int TargetConfig[],				//Config 값
		int RobJointSign[],				//조인트 회전 방향 값
		double RobJointOffset[]);		//조인트 Offset 값
	static int FindFinalTheta(double TargetRobJointTheta[], double BeginRobJointTheta[], int MotionType, int JointInterPolMode, int TurnNumberValue[], double JointLimit[][6], int kin_type);
	//static int ThetaGenerator(double CurRobJointTheta[], double CurBaseAuxTheta[], double CurMountAuxTheta[],										// output
	//	double CurrentTime,																													// input from user
	//	double ConstantVelTime, double AccelerateTime,																						// input from motion planner
	//	double(*BaseAuxParaMat)[4][4], double(*BaseAuxRobPosMat)[4],																// input from 데이터 정리
	//	double(*MountMat)[4], double(*TcpMat)[4], int TargetConfig[], double(*RobJointDV)[4][4],									// input from 데이터 정리
	//	double(*BeginPosTcpFromBaseAux)[4], double(*TargetPosTcpFromBaseAux)[4], double(*DU1)[3], double TcpRotationValue, 			// input from 데이터 정리
	//	double TargetRobJointTheta[],																			// input from inverse kinematics
	//	COptDeviceAttribute* pAttribute,		// input from Device attribute
	//	double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],										// input from Device state
	//	double TargetBaseAuxTheta[], double TargetMountAuxTheta[], int MotionType, int TurnNumberValue[], double JointLimit[][6],
	//	double BaseAuxJointDV[][4][4], int kin_type);							// input from Tag attribute
	//static int ThetaGenerator(double CurRobJointTheta[], double CurBaseAuxTheta[], double CurMountAuxTheta[],										// output
	//	double CurrentTime,																													// input from user
	//	double ConstantVelTime, double AccelerateTime,																						// input from motion planner
	//	double BaseAuxParaMat[][4][4], double BaseAuxRobPosMat[][4],																// input from 데이터 정리
	//	double MountMat[][4], double TcpMat[][4], int TargetConfig[], double RobJointDV[][4][4],									// input from 데이터 정리
	//	double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue, 			// input from 데이터 정리
	//	double TargetRobJointTheta[],																			// input from inverse kinematics
	//	double aa[], double dd[], double alpha, int JointInterPolMode,																// input from Device Attribute
	//	int BaseAuxNum, int MountAuxNum, int BaseAuxType[], int MountAuxType[], int RobJointSign[], double RobJointOffset[],		// input from Device attribute
	//	double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],										// input from Device state
	//	double TargetBaseAuxTheta[], double TargetMountAuxTheta[], int MotionType, int TurnNumberValue[], double JointLimit[][6],
	//	double BaseAuxJointDV[][4][4], int kin_type);							// input from Tag attribute
	static int FindUtFunction(double* Ut, double CurrentTime, double ConstantVelTime, double AccelerateTime);
	static int FindJointMotionTheta(double CurRobJointTheta[], double CurBaseAuxTheta[], double CurMountAuxTheta[],	// output
		double Ut,																								// input from Utfunction
		double TargetRobJointTheta[],																			// input from inverse kinematics
		int BaseAuxNum, int MountAuxNum, 																		// input from Device attribute
		double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],					// input from Device state
		double TargetBaseAuxTheta[], double TargetMountAuxTheta[], int kin_type);												// input from Tag attribute
	static int FindLinearMotionTheta(double CurRobJointTheta[], double CurBaseAuxTheta[], double CurMountAuxTheta[],						// output
		double Ut,																													// input from Utfunction
		double BaseAuxParaMat[][4][4], double BaseAuxRobPosMat[][4],																// input from 데이터 정리
		double MountMat[][4], double TcpMat[][4], int TargetConfig[], double RobJointDV[][4][4],									// input from 데이터 정리
		double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue, 			// input from 데이터 정리
		double aa[], double dd[], double alpha, int JointInterPolMode,																// input from Device Attribute
		int BaseAuxNum, int MountAuxNum, int BaseAuxType[], int MountAuxType[], int RobJointSign[], double RobJointOffset[],		// input from Device attribute
		double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],										// input from Device state
		double TargetBaseAuxTheta[], double TargetMountAuxTheta[], int MotionType, int TurnNumberValue[], double JointLimit[][6], double(*BaseAuxJointDV)[4][4], int kin_type);							// input from Tag attribute
	static int MotionPlanner(double* ConstantVelTime, double* AccelerateTime,																	//output
		double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue,				//input from 데이터 정리
		double TargetRobJointTheta[],																									//input from inverse kinematics	
		int BaseAuxNum, int MountAuxNum, double MaxRobJointVel[], double MaxRobJointAcc[],												//input from device attribute
		double MaxBaseAuxVel[], double MaxBaseAuxAcc[], double MaxMountAuxVel[], double MaxMountAuxAcc[],								//input from device attribute
		double MaxTcpLinVel, double MaxTcpLinAcc, double MaxTcpAngVel, double MaxTcpAngAcc,												//input from device attribute
		double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],											//input from device state
		int MotionType, double TargetBaseAuxTheta[], double TargetMountAuxTheta[], double TargetVelPercent, double TargetAccPercent);	//input from Tag Attribute
	static int FindJointMotionTime(double* ConstantVelTime, double* AccelerateTime,											//output
		double TargetRobJointTheta[],																					//input from inverse kinematics
		int BaseAuxNum, int MountAuxNum, double MaxRobJointVel[], double MaxRobJointAcc[],								//input from device attribute
		double MaxBaseAuxVel[], double MaxBaseAuxAcc[], double MaxMountAuxVel[], double MaxMountAuxAcc[],				//input from device attribute
		double BeginRobJointTheta[], double BeginBaseAuxTheta[], double BeginMountAuxTheta[],							//input from device state
		double TargetBaseAuxTheta[], double TargetMountAuxTheta[], double TargetVelPercent, double TargetAccPercent);	//input from Tag Attribute
	static int FindLinearMotionTime(double* ConstantVelTime, double* AccelerateTime,													//output
		double BeginPosTcpFromBaseAux[][4], double TargetPosTcpFromBaseAux[][4], double DU1[][3], double TcpRotationValue, 		//input from 데이터 정리
		int BaseAuxNum, int MountAuxNum,																						//input from device attribute
		double MaxBaseAuxVel[], double MaxBaseAuxAcc[], double MaxMountAuxVel[], double MaxMountAuxAcc[],						//input from device attribute
		double MaxTcpLinVel, double MaxTcpLinAcc, double MaxTcpAngVel, double MaxTcpAngAcc,										//input from device attribute
		double BeginBaseAuxTheta[], double BeginMountAuxTheta[],																//input from device state
		double TargetBaseAuxTheta[], double TargetMountAuxTheta[], double TargetVelPercent, double TargetAccPercent);			//input from Tag Attribute
	static int InverseKinematics(double TargetRobJointTheta[],																			// output
		double TargetTagPosMat[][4], double DevicePosMat[][4], double BaseAuxParaMat[][4][4], double BaseAuxRobPosMat[][4],		// input from 데이터 정리
		double MountMat[][4], double TcpMat[][4], int TargetConfig[], double RobJointDV[][4][4],								// input from 데이터 정리
		double aa[], double dd[], double alpha, int JointInterPolMode,															// input from Device Attribute
		int RobJointSign[], double RobJointOffset[], int BaseAuxNum, int BaseAuxType[], 										// input from Device Attribute
		double BeginRobJointTheta[], 																							// input from Device State
		double TargetBaseAuxTheta[], int MotionType, int TurnNumberValue[], double JointLimit[][6], double(*BaseAuxJointDV)[4][4], int kin_type);														// input from Tag Attribute
};
