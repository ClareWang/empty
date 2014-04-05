#include "Box2dWorld.h"

#include <cstring>
using namespace std;

#include "SnagForestWorld.h"


GameEntry g_gameEntries[] =
{
    {"SnagForestWorld", SnagForestWorld::Create},
//    {NULL, NULL}
};

int g_totalEntries = sizeof(g_gameEntries) / sizeof(g_gameEntries[0]);