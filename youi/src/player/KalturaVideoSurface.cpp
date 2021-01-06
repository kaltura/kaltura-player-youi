#include "KalturaVideoSurface.h"

KalturaVideoSurface::KalturaVideoSurface(IVideoSurfaceListener *listener)
: CYIVideoSurfacePlatform(CYIVideoSurface::Capabilities::Translate | CYIVideoSurface::Capabilities::Scale | CYIVideoSurface::Capabilities::Opacity)
, m_pListener(listener)
{

}

KalturaVideoSurface::~KalturaVideoSurface()
{
}

void KalturaVideoSurface::SetVideoRectangle(const YI_RECT_REL &rVideoRectangle)
{
    if (m_pListener)
    {
        m_pListener->SetVideoRectangle(rVideoRectangle);
    }
}

void KalturaVideoSurface::BringToFront()
{
    if (m_pListener)
    {
        m_pListener->BringToFront();
    }
}
