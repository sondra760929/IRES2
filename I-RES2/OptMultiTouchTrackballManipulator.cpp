#include "stdafx.h"
#include "OptMultiTouchTrackballManipulator.h"

using namespace osg;
using namespace osgGA;

OptMultiTouchTrackballManipulator::OptMultiTouchTrackballManipulator(int flags)
	: inherited(flags)
	, m_bRotate(false)
{
	setVerticalAxisFixed(false);
}

OptMultiTouchTrackballManipulator::OptMultiTouchTrackballManipulator(const OptMultiTouchTrackballManipulator& tm, const CopyOp& copyOp)
	: osg::Callback(tm, copyOp), inherited(tm, copyOp)
	, m_bRotate(false)
{
}

OptMultiTouchTrackballManipulator::~OptMultiTouchTrackballManipulator()
{
}

bool OptMultiTouchTrackballManipulator::handle(const GUIEventAdapter& ea, GUIActionAdapter& us)
{

	bool handled(false);

	switch (ea.getEventType())
	{

	case osgGA::GUIEventAdapter::PUSH:
	case osgGA::GUIEventAdapter::DRAG:
	case osgGA::GUIEventAdapter::RELEASE:
		if (ea.isMultiTouchEvent())
		{
			double eventTimeDelta = 1 / 60.0; //_ga_t0->getTime() - _ga_t1->getTime();
			if (eventTimeDelta < 0.)
			{
				OSG_WARN << "Manipulator warning: eventTimeDelta = " << eventTimeDelta << std::endl;
				eventTimeDelta = 0.;
			}
			osgGA::GUIEventAdapter::TouchData* data = ea.getTouchData();

			// three touches or two taps for home position
			if ((data->getNumTouchPoints() == 3) || ((data->getNumTouchPoints() == 1) && (data->get(0).tapCount >= 2)))
			{
				flushMouseEventStack();
				_thrown = false;
				home(ea, us);
				handled = true;
				m_bRotate = false;
			}
			else if (data->getNumTouchPoints() == 2)
			{
				if ((_lastEvent.valid()) && (_lastEvent->getTouchData()->getNumTouchPoints() >= 2))
				{
					const osg::Vec2 pt_1_now(data->get(0).x, data->get(0).y);
					const osg::Vec2 pt_2_now(data->get(1).x, data->get(1).y);
					const osg::Vec2 pt_1_last(_lastEvent->getTouchData()->get(0).x, _lastEvent->getTouchData()->get(0).y);
					const osg::Vec2 pt_2_last(_lastEvent->getTouchData()->get(1).x, _lastEvent->getTouchData()->get(1).y);

					const float gap_now((pt_1_now - pt_2_now).length());
					const float gap_last((pt_1_last - pt_2_last).length());

					// osg::notify(osg::ALWAYS) << gap_now << " " << gap_last << std::endl;

					const float relativeChange = (gap_last - gap_now) / gap_last;

					// zoom gesture
					fprintf(stderr, "relativeChange  %lf\n", relativeChange);
					if (fabs(relativeChange) > 0.02)
					{
						zoomModel(relativeChange, true);
					}
					else
					{
						// drag gesture
						const osg::Vec2 delta = ((pt_1_last - pt_1_now) + (pt_2_last - pt_2_now)) / 2.0f;

						const float scale = 0.3f * _distance * getThrowScale(eventTimeDelta) * 0.001f;
						//osg::notify(osg::ALWAYS) << "drag: " << delta << " scale: " << scale << std::endl;
						fprintf(stderr, "drag: %lf, %lf, %lf, %lf, scale: %lf\n", delta.x(), delta.y(), _distance, getThrowScale(eventTimeDelta), scale);

						panModel(delta.x() * scale, delta.y() * scale);
					}
				}

				m_bRotate = false;
				handled = true;
			}
			else if (data->getNumTouchPoints() == 1)
			{
				if (m_bRotate && (_lastEvent.valid()))
				{
					const osg::Vec2 pt_1_now(data->get(0).x, data->get(0).y);
					const osg::Vec2 pt_1_last(_lastEvent->getTouchData()->get(0).x, _lastEvent->getTouchData()->get(0).y);
					const osg::Vec2 delta = (pt_1_last - pt_1_now) * -0.001f;
					if (getVerticalAxisFixed())
						rotateWithFixedVertical(delta.x(), delta.y());
					else
						rotateTrackball(_ga_t0->getXnormalized(), _ga_t0->getYnormalized(),
						_ga_t1->getXnormalized(), _ga_t1->getYnormalized(),
						getThrowScale(eventTimeDelta));
				}
				else
				{
					m_bRotate = true;
				}
				handled = true;
			}
			else
			{
				m_bRotate = false;
			}

			_lastEvent = new GUIEventAdapter(ea);

			// check if all touches ended
			unsigned int num_touches_ended(0);
			for (osgGA::GUIEventAdapter::TouchData::iterator i = data->begin(); i != data->end(); ++i)
			{
				if ((*i).phase == osgGA::GUIEventAdapter::TOUCH_ENDED)
					num_touches_ended++;
			}

			if (num_touches_ended == data->getNumTouchPoints())
			{
				_lastEvent = NULL;
			}

		}
		break;
	default:
		break;
	}

	return handled ? handled : TrackballManipulator::handle(ea, us);
}
