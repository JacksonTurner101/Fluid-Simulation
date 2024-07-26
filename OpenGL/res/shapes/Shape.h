#pragma once

class Shape {
public:
	virtual void Draw() = 0;
	unsigned int GetIndexBufferId();
	unsigned int GetVertexArrayId();
	unsigned int GetShaderId();
private:
	unsigned int m_IbId;
	unsigned int m_VaoId;
	unsigned int m_ShaderId;
};