#include "main.h"

HLClient* g_pClient = NULL;
ISurface* g_pSurface = NULL;
CEntityList* g_pEntityList = NULL;
IPanel* g_pPanel = NULL;
CDebugOverlay* g_pDebugOverlay = NULL;
CEngineClient* g_pEngine = NULL;
CDraw g_Draw;
uintptr_t** ppInstance;
uintptr_t*  pNewVMT, *pOldVMT;
uintptr_t   VMTSize;
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

template<typename T> T CallOriginalPT(int index)
{
	return (T)(pOldVMT[index]);
}
bool W2S ( const Vector &vOrigin, Vector &vScreen )
{
	return ( g_pDebugOverlay->ScreenPosition( vOrigin, vScreen ));
}
typedef void(*oPaintTraverse)(void*,unsigned long long, bool, bool);
void PaintTraverseHook(void* v1, unsigned long long vguiPanel, bool forceRepaint, bool allowForce)
{
    CallOriginalPT<oPaintTraverse>(42)(v1,vguiPanel,forceRepaint,allowForce);

    const char* name = g_pPanel->GetName(vguiPanel);
    if(name && name[0] == 'F' && name[5] == 'O' && name[12] == 'P')
    {
      static bool bDidOnce =  false;
      if(!bDidOnce)
      {
          g_Draw.InitFont(12,"Tahoma");
          bDidOnce = true;
      }
      CBaseEntity* pLocal = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
      if(pLocal)
      for(int i = 0; i < 64; ++i)
      {
          CBaseEntity* pEnt = g_pEntityList->GetClientEntity(i);
          if(!pEnt)
          continue;
          if(pEnt == pLocal)
          continue;
          if(*(bool*)((unsigned long long)pEnt + 0x119)) //Dormant check
          continue;
          if(*(int*)((unsigned long long)pEnt + 0x28B) != 0) //Lifestate check
          continue;
          if(*(int*)((unsigned long long)pEnt + 0x12C) <= 0) //Health check
          continue;
          int r = 255,g = 255,b = 255;
          if(*(int*)((unsigned long long)pEnt + 0x120) == 2) //Esp color by team
          {
              r = 255;
              g = 0;
              b = 0;
          }
          else
          {
              r = 0;
              g = 0;
              b = 255;
          }
          Vector vecOrigin = *(Vector*)((unsigned long long)pEnt + 0x164);
          Vector Screen2D;
          if(!W2S(vecOrigin,Screen2D))
          {
              CEngineClient::player_info_t pInfo;
              g_pEngine->GetPlayerInfo(i,&pInfo);
              g_Draw.DrawString(true,Screen2D.x,Screen2D.y,r,g,b,255,pInfo.name);


          }
          g_Draw.DrawString(true,20,20,0,255,0,255,"Linux Basehook by Atex");
      }

    }
}

void DoPTHook(uintptr_t *Instance)
{
    ppInstance = (*(uintptr_t***)Instance);
	pOldVMT = *ppInstance;
    uintptr_t index = 0;
    for(index = 0; pOldVMT[index]; ++index)
    {
        if(pOldVMT[index] == 0)
        break;
    }

	VMTSize = index;

	if (VMTSize > 0)
    {
	pNewVMT = new uintptr_t[VMTSize];
	memcpy(pNewVMT, pOldVMT, sizeof(uintptr_t) * VMTSize);

	*ppInstance = (pNewVMT);

	Instance = pNewVMT;

    if(pNewVMT && pOldVMT && index <= VMTSize)
        pNewVMT[42] = (uintptr_t)PaintTraverseHook;
    }

}

void inithooks()
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    CreateInterfaceFn Client = NULL;
    CreateInterfaceFn Engine = NULL;
    CreateInterfaceFn VGUI = NULL;
    CreateInterfaceFn VGUI2 = NULL;

    Client = (CreateInterfaceFn)dlsym(dlopen("./csgo/bin/linux64/client_client.so",RTLD_NOW),"CreateInterface");
    Engine = (CreateInterfaceFn)dlsym(dlopen("./bin/linux64/engine_client.so",RTLD_NOW),"CreateInterface");
    VGUI = (CreateInterfaceFn)dlsym(dlopen("./bin/linux64/vguimatsurface_client.so",RTLD_NOW),"CreateInterface");
    VGUI2 = (CreateInterfaceFn)dlsym(dlopen("./bin/linux64/vgui2_client.so",RTLD_NOW),"CreateInterface");

    g_pClient = (HLClient*)Client("VClient017",NULL);

    g_pEntityList = (CEntityList*)Client("VClientEntityList003",NULL);

    g_pEngine = (CEngineClient*)Engine("VEngineClient013",NULL);

    g_pDebugOverlay = (CDebugOverlay*)Engine("VDebugOverlay004",NULL);

    g_pSurface = (ISurface*)VGUI("VGUI_Surface031",NULL);

    g_pPanel = (IPanel*)VGUI2("VGUI_Panel009",NULL);


    unsigned long int uintPanel = reinterpret_cast<unsigned long long>(g_pPanel);
    DoPTHook(&uintPanel);

}

void __attribute__((constructor))
base_main()
{
 std::thread inithack(inithooks);
 inithack.detach();
}
void __attribute__((destructor))
endthisshit()
{


}
