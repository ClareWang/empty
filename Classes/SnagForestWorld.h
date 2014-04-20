#ifndef __SNAGFOREST_WORLD_H__
#define __SNAGFOREST_WORLD_H__

class SnagForestWorld: public Box2dWorld
{
public :
	SnagForestWorld(float sizeX,float sizeY)
	{
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			// Floor
			shape.Set(b2Vec2(0.0/PT_RATIO, 0.0/PT_RATIO), b2Vec2(sizeX/PT_RATIO, 0.0/PT_RATIO));
			ground->CreateFixture(&shape, 0.0f);

			// Left wall
			shape.Set(b2Vec2(0.0/PT_RATIO, 0.0/PT_RATIO), b2Vec2(0.0/PT_RATIO, sizeY/PT_RATIO));
			ground->CreateFixture(&shape, 0.0f);

			// Right wall
			shape.Set(b2Vec2(sizeX/PT_RATIO, 0.0/PT_RATIO), b2Vec2(sizeX/PT_RATIO, sizeY/PT_RATIO));
			ground->CreateFixture(&shape, 0.0f);

			// Top
			shape.Set(b2Vec2(0.0/PT_RATIO, sizeY/PT_RATIO), b2Vec2(sizeX/PT_RATIO, sizeY/PT_RATIO));
			ground->CreateFixture(&shape, 0.0f);


			// create slots
			for (int32 i = 1; i < 12; ++i)
			{
				shape.Set(b2Vec2(sizeX/5/2/PT_RATIO * i, 0.0/PT_RATIO), b2Vec2(sizeX/5/2/PT_RATIO * i, 58/PT_RATIO));
				ground->CreateFixture(&shape, 0.0f);
			}

		}

		{
			float radius = 5.0f;
			b2CircleShape shape;
			shape.m_radius = radius/PT_RATIO;
			float winX = sizeX -radius*2;
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 0.01f;
			float32 restitution = 0.0f;
			for (int32 i = 0; i < 7; ++i)
			{
				for(int32 j = 0; j < 13; ++j)
				{
					b2BodyDef bd;
					bd.type = b2_staticBody;//b2_dynamicBody;
					restitution = 0.4*CCRANDOM_0_1();
					fd.friction = 0.05*CCRANDOM_0_1();;
					if (j%2 == 1)
					{
						bd.position.Set((winX/6 * i+radius)/PT_RATIO, (420-(winX/6/2)*j)/PT_RATIO);
					}
					else
					{
						bd.position.Set((winX/6/2 + winX/6 * i+radius)/PT_RATIO, (420-(winX/6/2)*j)/PT_RATIO);
					}
					bd.userData = NULL;
					b2Body* body = m_world->CreateBody(&bd);

					fd.restitution = restitution;
					body->CreateFixture(&fd);
				}

			}
		}

	}

	static Box2dWorld* Create(float sizeX,float sizeY)
	{
		return new SnagForestWorld(sizeX, sizeY);
	}

};
#endif // __SNAGFOREST_WORLD_H__