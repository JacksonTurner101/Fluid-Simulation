#include "Shape.h"

unsigned int Shape::GetIndexBufferId() {

	return m_IbId;
}

unsigned int Shape::GetVertexArrayId()
{
	return m_VaoId;
}

unsigned int Shape::GetShaderId()
{
	return m_ShaderId;
}
