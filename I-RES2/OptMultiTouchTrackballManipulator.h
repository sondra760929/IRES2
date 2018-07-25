#pragma once
#include <osgGA/TrackballManipulator>
namespace osgGA {

	class OptMultiTouchTrackballManipulator : public TrackballManipulator
	{
		typedef TrackballManipulator inherited;

	public:
		OptMultiTouchTrackballManipulator(int flags = DEFAULT_SETTINGS);
		OptMultiTouchTrackballManipulator(const OptMultiTouchTrackballManipulator& tm,
			const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY);

		META_Object(osgGA, OptMultiTouchTrackballManipulator);

		bool handle(const GUIEventAdapter& ea, GUIActionAdapter& us);
		~OptMultiTouchTrackballManipulator();
		bool m_bRotate;

		osg::ref_ptr<GUIEventAdapter> _lastEvent;
	};
}