#ifndef _VIDEO_SURFACE_LISTENER_H_
#define _VIDEO_SURFACE_LISTENER_H_

class IVideoSurfaceListener
{
public:
    virtual void SetVideoRectangle(const YI_RECT_REL &rVideoRectangle) = 0;
    virtual void BringToFront() = 0;

};

#endif
