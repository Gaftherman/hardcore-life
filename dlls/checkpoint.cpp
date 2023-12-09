// Investigar como funciona env_message para usar titles
// Crear key unica para el switch
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "skill.h"

class CCheckPoint : public CBaseAnimating
{
public:
	void Spawn() override;
	void Think() override;
	void Precache() override;
	bool KeyValue( KeyValueData* pkvd ) override;
	void Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE usetype, float value ) override;
};

void CCheckPoint::Spawn()
{
	Precache();

	SET_MODEL( ENT(pev), "models/checkpoint.mdl" );
	UTIL_SetSize(pev, Vector( -32, -32, -32 ), Vector( 32, 32, 32 ) );

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	pev->health = gSkillData.CheckPointHealth;
    SetUse( &CCheckPoint::Use );
}

void CCheckPoint::Precache()
{
	PRECACHE_MODEL("models/checkpoint.mdl");

	PRECACHE_SOUND("barnacle/bcl_alert2.wav");
}

bool CCheckPoint::KeyValue( KeyValueData* pkvd )
{
	if( FStrEq( pkvd->szKeyName, "m_iMode" ) )
        int m_iMode = atoi( pkvd->szValue );
    else
	    return CBaseAnimating::KeyValue(pkvd);
    return true;
}

void CCheckPoint::Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE usetype, float value)
{
    if( pev->health > 0 )
    {
        pev->health--;
    }
    if( !FStringNull( pev->target ) )
        FireTargets( STRING( pev->target ), pActivator, this, usetype, value );    
}