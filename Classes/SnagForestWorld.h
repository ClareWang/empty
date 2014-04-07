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



			// create slots
			for (int32 i = 1; i < 12; ++i)
			{
				shape.Set(b2Vec2(sizeX/5/2/PT_RATIO * i, 0.0/PT_RATIO), b2Vec2(sizeX/5/2/PT_RATIO * i, 58/PT_RATIO));
				ground->CreateFixture(&shape, 0.0f);
			}

		}

		{
			b2CircleShape shape;
			shape.m_radius = 0.1/PT_RATIO;

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			fd.friction = 3.0f;

			float32 restitution = 0.1f;

			for (int32 i = 0; i < 7; ++i)
			{
				for(int32 j = 0; j < 13; ++j)
				{
					b2BodyDef bd;
					bd.type = b2_staticBody;//b2_dynamicBody;

					if (j%2 == 1)
					{
						bd.position.Set(sizeX/6/PT_RATIO * i, (sizeX+70-(sizeX/6/2-5)*j)/PT_RATIO);
					}
					else
					{
						bd.position.Set(sizeX/6/2/PT_RATIO + sizeX/6/PT_RATIO * i, (sizeX+70-(sizeX/6/2-5)*j)/PT_RATIO);
					}
					bd.userData = NULL;
					b2Body* body = m_world->CreateBody(&bd);

					fd.restitution = restitution;
					body->CreateFixture(&fd);
				}

			}
		}

		/*{
			b2CircleShape shape1;
			shape1.m_radius = 12.5/PT_RATIO;
			b2FixtureDef fd1;
			fd1.shape = &shape1;
			fd1.density = 1.0f;
			fd1.friction = 0.0f;
			float32 restitution = 0.1f;
			b2BodyDef bd1;
			bd1.type = b2_dynamicBody;
			bd1.position.Set(150/PT_RATIO, sizeY/PT_RATIO);

			b2Body* body = m_world->CreateBody(&bd1);

			fd1.restitution = restitution;
			body->CreateFixture(&fd1);

		}*/

	}

	static Box2dWorld* Create(float sizeX,float sizeY)
	{
		return new SnagForestWorld(sizeX, sizeY);
	}

};
#endif // __SNAGFOREST_WORLD_H__