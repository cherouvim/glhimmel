
#include <glhimmel-computed/ComputedHimmel.h>

#include <glhimmel-computed/Earth.h>
#include <glhimmel-base/Timef.h>
#include <glhimmel-computed/Astronomy.h>

#include <glhimmel-computed/AbstractAstronomy.h>
#include <glhimmel-computed/AtmosphereDrawable.h>

/*#include "moongeode.h"
#include "moonglaregeode.h"
#include "starsgeode.h"
#include "starmapgeode.h"
#include "highcloudlayergeode.h"
#include "dubecloudlayergeode.h"

#include <osg/ShapeDrawable>*/

#include <stdlib.h>
#include <assert.h>


namespace glHimmel
{

ComputedHimmel *ComputedHimmel::createWithoutClouds()
{
    // cubeMapFilePaths should contain a questionmark '?' that is replaced
    // by cubemap extensions '_px', '_nx', '_py', etc.
    // e.g. "resources/starmap?.png" points to "resources/milkyway_px.png" etc.

    return new ComputedHimmel(
    //    new StarMapGeode("resources/skymap?.png")
    //,   new MoonGeode("resources/moon?.png")
    //,   new StarsGeode("resources/brightstars")
        new AtmosphereDrawable()
    //,   NULL//new HighCloudLayerGeode()
    //,   NULL//new DubeCloudLayerGeode()
    ,   new Astronomy()
    );
}


//ComputedHimmel *ComputedHimmel::createWithClouds()
//{
//    // cubeMapFilePaths should contain a questionmark '?' that is replaced
//    // by cubemap extensions '_px', '_nx', '_py', etc.
//    // e.g. "resources/starmap?.png" points to "resources/milkyway_px.png" etc.
//
//    return new ComputedHimmel(
//        new StarMapGeode("resources/skymap?.png")
//    ,   new MoonGeode("resources/moon?.png")
//    ,   new StarsGeode("resources/brightstars")
//    ,   new AtmosphereDrawable()
//    ,   new HighCloudLayerGeode()
//    ,   new DubeCloudLayerGeode()
//    ,   new Astronomy()
//    );
//}


ComputedHimmel::ComputedHimmel(
//    StarMapGeode *milkyWay
//,   MoonGeode *moon
//,   StarsGeode *stars
   AtmosphereDrawable *atmosphere
//,   HighCloudLayerGeode *highLayer
//,   DubeCloudLayerGeode *dubeLayer
,   AbstractAstronomy *astronomy)
:   AbstractHimmel()
,   m_altitude(0.2f)
,   m_sunPosition(glm::vec3(0.0, 0.0, 0.0))
,   m_sunRefractedPosition(glm::vec3(0.0, 0.0, 0.0))
,   m_time(0.f)
//,   m_starmap(milkyWay)
//,   m_moon(moon)
//,   m_moonGlare(NULL)
//,   m_stars(stars)
,   m_astronomy(astronomy)
,   m_atmosphere(atmosphere)
//,   m_highLayer(highLayer)
//,   m_dubeLayer(dubeLayer)
{
    assert(m_astronomy);

    /*
    if(m_starmap)
    {
        addChild(m_starmap);
        m_starmap->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }
    if(m_stars)
    {
        addChild(m_stars);
        m_stars->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }
    if(m_moon)
    {
        addChild(m_moon);
        m_moon->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);

        //m_moonGlare = new MoonGlareGeode;
        //addChild(m_moonGlare);
        //m_moonGlare->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);

        //m_moon->addUniformsToVariousStateSate(m_moonGlare->getOrCreateStateSet());
    }

    if(m_highLayer)
    {
        addChild(m_highLayer);
        m_highLayer->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }

    if(m_dubeLayer)
    {
        addChild(m_dubeLayer);
        m_dubeLayer->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }*/
}

ComputedHimmel::~ComputedHimmel()
{
}

void ComputedHimmel::update()
{
    AbstractHimmel::update();

    updateSeed();

    if(isDirty())
    {
        const auto atime = astronomicalTime::fromTimeF(*getTime());
        m_astronomy->update(atime);
        m_sunPosition = m_astronomy->getSunPosition(false);
        m_sunRefractedPosition = m_astronomy->getSunPosition(true);
        m_time = static_cast<float>(getTime()->getf());

        //if(m_starmap)
        //    m_starmap->update(*this);
        //if(m_moon)
        //{
        //    m_moon->update(*this);
        //    //m_moonGlare->update(*this);
        //}
        //if(m_stars)
        //    m_stars->update(*this);
        if(m_atmosphere)
            m_atmosphere->update(*this);
        /*if(m_highLayer)
            m_highLayer->update(*this);

        if(m_dubeLayer)
            m_dubeLayer->update(*this);*/

        dirty(false);
    }
}

void ComputedHimmel::updateSeed()
{
    m_seed = rand();
}

AbstractAstronomy * ComputedHimmel::astronomy() const
{
    return m_astronomy.get();
}

AtmosphereDrawable * ComputedHimmel::atmosphere() const
{
    return m_atmosphere.get();
}

glm::vec3 ComputedHimmel::getSunPosition() const
{
    return m_sunPosition;
}

glm::vec3 ComputedHimmel::getSunPosition(const astronomicalTime &aTime) const
{
    return m_astronomy->getSunPosition(aTime, m_astronomy->getLatitude(), m_astronomy->getLongitude(), false);
}

float ComputedHimmel::setLatitude(const float latitude)
{
    assert(m_astronomy);

    dirty();
    return m_astronomy->setLatitude(latitude);
}

float ComputedHimmel::getLatitude() const
{
    assert(m_astronomy);
    return m_astronomy->getLatitude();
}

float ComputedHimmel::setLongitude(const float longitude)
{
    assert(m_astronomy);

    dirty();
    return m_astronomy->setLongitude(longitude);
}

float ComputedHimmel::getLongitude() const
{
    assert(m_astronomy);
    return m_astronomy->getLongitude();
}


void ComputedHimmel::setAltitude(const float altitude)
{
    m_altitude = altitude;
}

float ComputedHimmel::getAltitude() const
{
    return m_altitude;
}

float ComputedHimmel::defaultAltitude()
{
    return 0.2f;
}

float ComputedHimmel::getSeed() const
{
    return m_seed;
}

void ComputedHimmel::initialize()
{
}

} // namespace glHimmel
