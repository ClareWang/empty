#ifndef __POLY_SPRITE_H__
#define __POLY_SPRITE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class PolySprite: public cocos2d::CCSprite
{
public:
	PolySprite() : vertexs_(NULL), uvs_(NULL), indices_(NULL), verCnt_(0) {}
	virtual ~PolySprite();

	static PolySprite* create(const char *pFile,
		const cocos2d::CCPoint *uvs,
		int verCnt,
		const int *indices);

	//重载父类draw
	void draw();

private:
	//初始化顶点信息
	bool initWithUV(const cocos2d::CCPoint *uvs,
		const int *indices,
		int verCnt);

	//计算中点
	cocos2d::CCPoint getCenter();
	void translate(const cocos2d::CCPoint&);
	void drawPoly();
	void releasePoly();

private:
	//多边形顶点
	cocos2d::ccVertex2F *vertexs_;
	//定点纹理坐标
	cocos2d::ccVertex2F *uvs_;
	//三角形索引
	unsigned short *indices_;
	//顶点颜色
	unsigned char *colors_;
	//顶点数目
	int verCnt_;
};

#endif // __POLY_SPRITE_H__