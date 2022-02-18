/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <cstdio>

#include "GameObject.h"


GameObject::GameObject(const char* cpName)
:  m_sName(cpName),
   m_fPosX(0.0f),
   m_fPosY(0.0f),
   m_fScale(1.0f),
   m_fAngle(0.0f),
   m_opSprite(NULL),
   m_fRadius(0.0f)
{
}



GameObject::~GameObject()
{
}



void GameObject::print()
{
    printf("Object Name: %s - Position: (%f,%f)\n", m_sName.c_str(), m_fPosX, m_fPosY);
}
