#pragma once

#include <texturebased/texturebased_api.h>
#include <memory>


namespace glHimmel
{
class TimeF;

class TEXTUREBASED_API AbstractHimmel
{
public:
    AbstractHimmel();
    virtual ~AbstractHimmel();

    void assignTime(std::unique_ptr<TimeF> timef);
    TimeF* getTime() const;

    virtual void update();
    void initialize();

    virtual bool isDirty();
    void dirty(bool dirty = true);

protected:
    float timef() const;

protected:
    std::unique_ptr<TimeF> m_timef;
    bool m_initialized;
    bool m_autoUpdateTime;
    bool m_dirty;
    long double m_lastElapsed;
};

} // namespace osgHimmel
