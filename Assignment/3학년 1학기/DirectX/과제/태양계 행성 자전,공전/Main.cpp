#include "SolarSystem.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
    // Create the D3D object.
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the D3DDevice
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    // Turn off culling, so we see the front and back of the triangle
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // Turn off D3D lighting, since we are providing our own vertex colors
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------

D3DMATERIAL9 g_pMaterial;

CSolarSystem* solar;
D3DXVECTOR3 vZoom;

HRESULT InitGeometry()
{
    {
        g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
        D3DLIGHT9 light;
        ZeroMemory(&light, sizeof(light));

        light.Type = D3DLIGHT_SPOT;
        light.Diffuse.r = 1.f;
        light.Diffuse.g = 1.f;
        light.Diffuse.b = 1.f;
        light.Diffuse.a = 1.f;

        light.Ambient.r = 1.f;
        light.Ambient.g = 1.f;
        light.Ambient.b = 1.f;

        light.Specular.r = 1.0f;
        light.Specular.g = 1.0f;
        light.Specular.b = 1.0f;

        light.Direction.x = 0.f;
        light.Direction.y = -1.0f;
        light.Direction.z = 1.f;

        light.Position.x = 0.;
        light.Position.y = 10.f;
        light.Position.z = 0.8f;

        light.Attenuation0 = 1.0f;
        light.Range = 4000.f;

        g_pd3dDevice->SetLight(0, &light);
        g_pd3dDevice->LightEnable(0, TRUE);       
    }

    solar = new CSolarSystem();
    CSphere sphere;
    
    sphere.m_Sphere.fRadius = 100;
    sphere.m_color = COLOR(1, 1, 0);
    CPlanet* planet = new CPlanet("Sun", sphere);
    planet->m_Data.Create(g_pd3dDevice, 15, 15);
    
    solar->CreateMoon((char*)"Sun", planet);

    sphere.m_Sphere.fRadius = 10;
    sphere.m_color = COLOR(1, 0.2f, 0);
    planet = new CPlanet("Mercury", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->CreateMoon((char*)"Mercury", planet);

    sphere.m_Sphere.fRadius = 20;
    sphere.m_color = COLOR(1, 0.6f, 0);
    planet = new CPlanet("Venus", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->CreateMoon((char*)"Venus", planet);

    sphere.m_Sphere.fRadius = 20;
    sphere.m_color = COLOR(0, 0.4f, 1);
    planet = new CPlanet("Earth", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->CreateMoon((char*)"Earth", planet);
    solar->m_earth->CreateMoon((char*)"Earth", planet);

    sphere.m_Sphere.fRadius = 5;
    sphere.m_color = COLOR(0.5f, 0.5f, 0);
    planet = new CPlanet("Moon", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->m_earth->CreateMoon((char*)"Moon", planet);

    sphere.m_Sphere.fRadius = 15;
    sphere.m_color = COLOR(1, 0.2f, 0);
    planet = new CPlanet("Mars", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->CreateMoon((char*)"Mars", planet);

    sphere.m_Sphere.fRadius = 45;
    sphere.m_color = COLOR(1, 0.2f, 0.5f);
    planet = new CPlanet("Jupiter", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->CreateMoon((char*)"Jupiter", planet);

    sphere.m_Sphere.fRadius = 60;
    sphere.m_color = COLOR(1, 0.2f, 0.2f);
    planet = new CPlanet("Saturn", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->CreateMoon((char*)"Saturn", planet);

    sphere.m_Sphere.fRadius = 30;
    sphere.m_color = COLOR(0.f, 0.8f, 0.8f);
    planet = new CPlanet("Uranus", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->CreateMoon((char*)"Uranus", planet);

    sphere.m_Sphere.fRadius = 30;
    sphere.m_color = COLOR(0.f, 0.f, 0.2f);
    planet = new CPlanet("Neptune", sphere);
    planet->m_Data.Create(g_pd3dDevice, 10, 10);

    solar->CreateMoon((char*)"Neptune", planet);


    vZoom = D3DXVECTOR3(0, 1500.f, -2500.f);

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if (g_pd3dDevice != NULL)
        g_pd3dDevice->Release();

    if (g_pD3D != NULL)
        g_pD3D->Release();

    delete solar;
}


//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------

VOID SetupMatrices()
{    
    solar->UpData();

    // rotation
    D3DXVECTOR3 vEyePt = vZoom;

    // translate
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);

    // up position
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 2.0f, 2.0f, 10000.0f);
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------

VOID Render()
{
    // Clear the backbuffer to a black color
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0, 0, 0, 1), 1.0f, 0);

    // Begin the scene
    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        // Setup the world, view, and projection matrices
        SetupMatrices();

        g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        solar->Render(g_pd3dDevice, g_pMaterial);
        g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------

POINT pt;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:

        break;

    //case WM_LBUTTONDOWN:
    //{
    //    pt.x = LOWORD(lParam);
    //    pt.y = HIWORD(lParam);

    //    SetCapture(hWnd);
    //}
    //    break;

    //case WM_MOUSEMOVE:
    //{
    //    if (GetCapture() == hWnd)
    //    {
    //        int x = LOWORD(lParam);
    //        int y = HIWORD(lParam);

    //        if (vZoom.x < 180.f && vZoom.x > -180.f)
    //            vZoom.x += (x - pt.x) / 3.6f;   
    //        if (vZoom.y < 180.f && vZoom.y > -180.f)
    //            vZoom.y += (y - pt.y) / 3.6f;

    //        pt.x = x;
    //        pt.y = y;
    //    }
    //}
    //    break;

    //case WM_LBUTTONUP:
    //    ReleaseCapture();
    //    break;

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);

        if (delta > 0)
            vZoom.z += 10.f;
        else
            vZoom.z -= 10.f;
    }
        break;

    case WM_DESTROY:
        Cleanup();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        L"D3D Tutorial", NULL
    };
    RegisterClassEx(&wc);

    // Create the application's window
    HWND hWnd = CreateWindow(L"D3D Tutorial", L"D3D Tutorial 03: Matrices",
        WS_OVERLAPPEDWINDOW, 100, 100, 1024, 512,
        NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (SUCCEEDED(InitD3D(hWnd)))
    {
        // Create the scene geometry
        if (SUCCEEDED(InitGeometry()))
        {
            // Show the window
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);

            // Enter the message loop
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            while (msg.message != WM_QUIT)
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                    Render();
            }
        }
    }

    UnregisterClass(L"D3D Tutorial", wc.hInstance);
    return 0;
}



