#ifndef _KALTURA_VIDEO_SURFACE_H_
#define _KALTURA_VIDEO_SURFACE_H_

#include <player/YiVideoSurfacePlatform.h>

#include "IVideoSurfaceListener.h"

class KalturaVideoSurface : public CYIVideoSurfacePlatform
{
public:
    KalturaVideoSurface(IVideoSurfaceListener *listener);
    virtual ~KalturaVideoSurface();

    virtual void SetVideoRectangle(const YI_RECT_REL &rVideoRectangle);
    
    IVideoSurfaceListener *m_pListener;
};

#endif // _KALTURA_VIDEO_SURFACE_H_
