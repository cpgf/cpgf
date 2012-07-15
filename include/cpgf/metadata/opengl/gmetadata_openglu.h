#ifndef __GMETADATA_OPENGLU_H
#define __GMETADATA_OPENGLU_H


#include "cpgf/metadata/gmetadataconfig.h"


#include "cpgf/metadata/private/gmetadata_header.h"

// this fake class declaration here is to cheat IsConvertible to avoid incomplete type error
// if the future glu.h declare them (instead of forward them), we should remove below classes.
class GLUnurbs{};
class GLUquadric{};
class GLUtesselator{};

namespace cpgf {


namespace metadata_internal {


template <typename MetaDefine>
void doBuildMetaData_open_glu_constants(const GMetaDataConfigFlags & /*config*/, MetaDefine define)
{
	define.CPGF_MD_TEMPLATE _enum<int>("glu_enum")

		/* Version */
		GENUM_VALUE(GLU_VERSION_1_1)
		GENUM_VALUE(GLU_VERSION_1_2)

		/* Errors: (return value 0 = no error) */
		GENUM_VALUE(GLU_INVALID_ENUM)
		GENUM_VALUE(GLU_INVALID_VALUE)
		GENUM_VALUE(GLU_OUT_OF_MEMORY)
		GENUM_VALUE(GLU_INCOMPATIBLE_GL_VERSION)

		/* StringName */
		GENUM_VALUE(GLU_VERSION)
		GENUM_VALUE(GLU_EXTENSIONS)

		/* Boolean */
		GENUM_VALUE(GLU_TRUE)
		GENUM_VALUE(GLU_FALSE)


		/****           Quadric constants               ****/

		/* QuadricNormal */
		GENUM_VALUE(GLU_SMOOTH)
		GENUM_VALUE(GLU_FLAT)
		GENUM_VALUE(GLU_NONE)

		/* QuadricDrawStyle */
		GENUM_VALUE(GLU_POINT)
		GENUM_VALUE(GLU_LINE)
		GENUM_VALUE(GLU_FILL)
		GENUM_VALUE(GLU_SILHOUETTE)

		/* QuadricOrientation */
		GENUM_VALUE(GLU_OUTSIDE)
		GENUM_VALUE(GLU_INSIDE)

		/* Callback types: */
		/*      GLU_ERROR               100103 */


		/****           Tesselation constants           ****/

		GENUM_VALUE(GLU_TESS_MAX_COORD)

		/* TessProperty */
		GENUM_VALUE(GLU_TESS_WINDING_RULE)
		GENUM_VALUE(GLU_TESS_BOUNDARY_ONLY)
		GENUM_VALUE(GLU_TESS_TOLERANCE)

		/* TessWinding */
		GENUM_VALUE(GLU_TESS_WINDING_ODD)
		GENUM_VALUE(GLU_TESS_WINDING_NONZERO)
		GENUM_VALUE(GLU_TESS_WINDING_POSITIVE)
		GENUM_VALUE(GLU_TESS_WINDING_NEGATIVE)
		GENUM_VALUE(GLU_TESS_WINDING_ABS_GEQ_TWO)

		/* TessCallback */
		GENUM_VALUE(GLU_TESS_BEGIN)
		GENUM_VALUE(GLU_TESS_VERTEX)
		GENUM_VALUE(GLU_TESS_END)
		GENUM_VALUE(GLU_TESS_ERROR)
		GENUM_VALUE(GLU_TESS_EDGE_FLAG)
		GENUM_VALUE(GLU_TESS_COMBINE)
		GENUM_VALUE(GLU_TESS_BEGIN_DATA)
		GENUM_VALUE(GLU_TESS_VERTEX_DATA)
		GENUM_VALUE(GLU_TESS_END_DATA)
		GENUM_VALUE(GLU_TESS_ERROR_DATA)
		GENUM_VALUE(GLU_TESS_EDGE_FLAG_DATA)
		GENUM_VALUE(GLU_TESS_COMBINE_DATA)

		/* TessError */
		GENUM_VALUE(GLU_TESS_ERROR1)
		GENUM_VALUE(GLU_TESS_ERROR2)
		GENUM_VALUE(GLU_TESS_ERROR3)
		GENUM_VALUE(GLU_TESS_ERROR4)
		GENUM_VALUE(GLU_TESS_ERROR5)
		GENUM_VALUE(GLU_TESS_ERROR6)
		GENUM_VALUE(GLU_TESS_ERROR7)
		GENUM_VALUE(GLU_TESS_ERROR8)

		GENUM_VALUE(GLU_TESS_MISSING_BEGIN_POLYGON)
		GENUM_VALUE(GLU_TESS_MISSING_BEGIN_CONTOUR)
		GENUM_VALUE(GLU_TESS_MISSING_END_POLYGON)
		GENUM_VALUE(GLU_TESS_MISSING_END_CONTOUR)
		GENUM_VALUE(GLU_TESS_COORD_TOO_LARGE)
		GENUM_VALUE(GLU_TESS_NEED_COMBINE_CALLBACK)

		/****           NURBS constants                 ****/

		/* NurbsProperty */
		GENUM_VALUE(GLU_AUTO_LOAD_MATRIX)
		GENUM_VALUE(GLU_CULLING)
		GENUM_VALUE(GLU_SAMPLING_TOLERANCE)
		GENUM_VALUE(GLU_DISPLAY_MODE)
		GENUM_VALUE(GLU_PARAMETRIC_TOLERANCE)
		GENUM_VALUE(GLU_SAMPLING_METHOD)
		GENUM_VALUE(GLU_U_STEP)
		GENUM_VALUE(GLU_V_STEP)

		/* NurbsSampling */
		GENUM_VALUE(GLU_PATH_LENGTH)
		GENUM_VALUE(GLU_PARAMETRIC_ERROR)
		GENUM_VALUE(GLU_DOMAIN_DISTANCE)


		/* NurbsTrim */
		GENUM_VALUE(GLU_MAP1_TRIM_2)
		GENUM_VALUE(GLU_MAP1_TRIM_3)

		/* NurbsDisplay */
		/*      GLU_FILL                100012 */
		GENUM_VALUE(GLU_OUTLINE_POLYGON)
		GENUM_VALUE(GLU_OUTLINE_PATCH)

		/* NurbsCallback */
		/*      GLU_ERROR               100103 */

		/* NurbsErrors */
		GENUM_VALUE(GLU_NURBS_ERROR1)
		GENUM_VALUE(GLU_NURBS_ERROR2)
		GENUM_VALUE(GLU_NURBS_ERROR3)
		GENUM_VALUE(GLU_NURBS_ERROR4)
		GENUM_VALUE(GLU_NURBS_ERROR5)
		GENUM_VALUE(GLU_NURBS_ERROR6)
		GENUM_VALUE(GLU_NURBS_ERROR7)
		GENUM_VALUE(GLU_NURBS_ERROR8)
		GENUM_VALUE(GLU_NURBS_ERROR9)
		GENUM_VALUE(GLU_NURBS_ERROR10)
		GENUM_VALUE(GLU_NURBS_ERROR11)
		GENUM_VALUE(GLU_NURBS_ERROR12)
		GENUM_VALUE(GLU_NURBS_ERROR13)
		GENUM_VALUE(GLU_NURBS_ERROR14)
		GENUM_VALUE(GLU_NURBS_ERROR15)
		GENUM_VALUE(GLU_NURBS_ERROR16)
		GENUM_VALUE(GLU_NURBS_ERROR17)
		GENUM_VALUE(GLU_NURBS_ERROR18)
		GENUM_VALUE(GLU_NURBS_ERROR19)
		GENUM_VALUE(GLU_NURBS_ERROR20)
		GENUM_VALUE(GLU_NURBS_ERROR21)
		GENUM_VALUE(GLU_NURBS_ERROR22)
		GENUM_VALUE(GLU_NURBS_ERROR23)
		GENUM_VALUE(GLU_NURBS_ERROR24)
		GENUM_VALUE(GLU_NURBS_ERROR25)
		GENUM_VALUE(GLU_NURBS_ERROR26)
		GENUM_VALUE(GLU_NURBS_ERROR27)
		GENUM_VALUE(GLU_NURBS_ERROR28)
		GENUM_VALUE(GLU_NURBS_ERROR29)
		GENUM_VALUE(GLU_NURBS_ERROR30)
		GENUM_VALUE(GLU_NURBS_ERROR31)
		GENUM_VALUE(GLU_NURBS_ERROR32)
		GENUM_VALUE(GLU_NURBS_ERROR33)
		GENUM_VALUE(GLU_NURBS_ERROR34)
		GENUM_VALUE(GLU_NURBS_ERROR35)
		GENUM_VALUE(GLU_NURBS_ERROR36)
		GENUM_VALUE(GLU_NURBS_ERROR37)
	;
}

template <typename MetaDefine>
void doBuildMetaData_open_glu_functions(const GMetaDataConfigFlags & /*config*/, MetaDefine define)
{
	GM(gluErrorString)
//	GM(gluErrorUnicodeStringEXT)
	GM(gluGetString)
	GM(gluOrtho2D)
	GM(gluPerspective)
	GM(gluPickMatrix)
	GM(gluLookAt)
	GM(gluProject)
	GM(gluUnProject)
	GM(gluScaleImage)
	GM(gluBuild1DMipmaps)
	GM(gluBuild2DMipmaps)
	GM(gluNewQuadric)
	GM(gluDeleteQuadric)
	GM(gluQuadricNormals)
	GM(gluQuadricTexture)
	GM(gluQuadricOrientation)
	GM(gluQuadricDrawStyle)
	GM(gluCylinder)
	GM(gluDisk)
	GM(gluPartialDisk)
	GM(gluSphere)
	GM(gluQuadricCallback)
	GM(gluNewTess)
	GM(gluDeleteTess)
	GM(gluTessBeginPolygon)
	GM(gluTessBeginContour)
	GM(gluTessVertex)
	GM(gluTessEndContour)
	GM(gluTessEndPolygon)
	GM(gluTessProperty)
	GM(gluTessNormal)
	GM(gluTessCallback)
	GM(gluGetTessProperty)
	GM(gluNewNurbsRenderer)
	GM(gluDeleteNurbsRenderer)
	GM(gluBeginSurface)
	GM(gluBeginCurve)
	GM(gluEndCurve)
	GM(gluEndSurface)
	GM(gluBeginTrim)
	GM(gluEndTrim)
	GM(gluPwlCurve)
	GM(gluNurbsCurve)
	GM(gluNurbsSurface)
	GM(gluLoadSamplingMatrices)
	GM(gluNurbsProperty)
	GM(gluGetNurbsProperty)
	GM(gluNurbsCallback)
}

template <typename MetaDefine>
void doBuildMetaData_open_glu(const GMetaDataConfigFlags & config, MetaDefine define)
{
	doBuildMetaData_open_glu_constants(config, define);
	doBuildMetaData_open_glu_functions(config, define);
}


} // namespace metadata_internal


template <typename MetaDefine>
void buildMetaData_open_glu(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_open_glu(config, define);
}

template <typename MetaDefine>
void buildMetaData_open_glu(MetaDefine define)
{
	buildMetaData_open_glu(mdcScriptable | mdcAutoProperty, define);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif

